#include "script/Script.h"

#include "SDL.h"

#include "AmityContext.h"
#include "assets.h"
#include "log.h"
#include "script/TextureHandle.h"

static JSRuntime* rt = NULL;

static JSClass classGlobal = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

template <class T, JSBool (T::*fn)(JSContext*, uintN, jsval*)>
static JSBool bindFunction (JSContext* jsCtx, uintN argc, jsval* vp)
{
    T* self = static_cast<T*>(JS_GetContextPrivate(jsCtx));
    return (self->*fn)(jsCtx, argc, vp);
}

template <class T, JSBool (T::*fn)(JSContext*, JSObject*, jsid, jsval*)>
static JSBool bindProperty (JSContext* jsCtx, JSObject* obj, jsid id, jsval* vp)
{
    T* self = static_cast<T*>(JS_GetContextPrivate(jsCtx));
    return (self->*fn)(jsCtx, obj, id, vp);
}

static void scriptReportError (JSContext* jsCtx, const char* message, JSErrorReport* report)
{
    LOGW("%s:%u %s\n",
        report->filename ? report->filename : "<no filename>",
        (unsigned int) report->lineno,
        message);
}

static JSBool amity_log (JSContext* jsCtx, uintN argc, jsval* vp)
{
    const char* message;
    if (!JS_ConvertArguments(jsCtx, argc, JS_ARGV(jsCtx, vp), "s", &message)) {
        return JS_FALSE;
    }

    LOGI("--> %s", message);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::amity_canvas_save, jsCtx, argc, vp)
{
    _amityCtx->canvas.save();
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::amity_canvas_restore, jsCtx, argc, vp)
{
    _amityCtx->canvas.restore();
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::amity_canvas_rotate, jsCtx, argc, vp)
{
    jsdouble angle;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "d", &angle)) {
        return JS_FALSE;
    }

    _amityCtx->canvas.rotate(angle);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::amity_canvas_translate, jsCtx, argc, vp)
{
    jsdouble x, y;
    if (!JS_ConvertArguments(jsCtx, 2, JS_ARGV(jsCtx, vp), "dd", &x, &y)) {
        return JS_FALSE;
    }

    _amityCtx->canvas.translate(x, y);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::amity_canvas_scale, jsCtx, argc, vp)
{
    jsdouble x, y;
    if (!JS_ConvertArguments(jsCtx, 2, JS_ARGV(jsCtx, vp), "dd", &x, &y)) {
        return JS_FALSE;
    }

    _amityCtx->canvas.scale(x, y);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::amity_canvas_drawTexture, jsCtx, argc, vp)
{
    JSObject* textureObj;
    jsdouble x, y;
    if (!JS_ConvertArguments(jsCtx, 3, JS_ARGV(jsCtx, vp), "odd", &textureObj, &x, &y)) {
        return JS_FALSE;
    }

    TextureHandle* textureHandle = TextureHandle::getFrom(jsCtx, textureObj);
    if (textureHandle == NULL) {
        return JS_FALSE;
    }
    _amityCtx->canvas.drawTexture(textureHandle->getTexture(), x, y);
    return JS_TRUE;
}

SCRIPT_PROPERTY (Script::amity_canvas_setAlpha, jsCtx, obj, id, vp)
{
    jsdouble alpha;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "d", &alpha)) {
        return JS_FALSE;
    }

    _amityCtx->canvas.setAlpha(alpha);
    return JS_TRUE;
}

static jsid getInternedId (JSContext* jsCtx, const char* name)
{
    JSString* str = JS_InternString(jsCtx, name);
    return INTERNED_STRING_TO_JSID(JS_InternString(jsCtx, name));
}

void Script::initAmityClasses ()
{
    JSObject* global = JS_GetGlobalObject(_jsCtx);

    JSObject* amity = JS_DefineObject(_jsCtx, global, "__amity", NULL, NULL, 0);
    JSFunctionSpec amityFunctions[] = {
        JS_FS("log", amity_log, 1, 0),
        JS_FS("createTexture", amity_createTexture, 1, 0),
        JS_FS_END
    };
    JS_DefineFunctions(_jsCtx, amity, amityFunctions);

    _eventObj = JS_DefineObject(_jsCtx, amity, "events", NULL, NULL, 0);
    _onEnterFrame = getInternedId(_jsCtx, "onEnterFrame");
    _onMouseDown = getInternedId(_jsCtx, "onMouseDown");
    _onMouseMove = getInternedId(_jsCtx, "onMouseMove");
    _onMouseUp = getInternedId(_jsCtx, "onMouseUp");

    JSObject* canvas = JS_DefineObject(_jsCtx, amity, "canvas",
        NULL, NULL, JSPROP_READONLY | JSPROP_PERMANENT);
    int screenWidth, screenHeight;
    SDL_GetWindowSize(_amityCtx->window, &screenWidth, &screenHeight);
    JSConstDoubleSpec amityConstants[] = {
        { screenWidth, "WIDTH", 0, { 0, 0, 0 } },
        { screenHeight, "HEIGHT", 0, { 0, 0, 0 } },
        { 0, NULL, 0, { 0, 0, 0 } },
    };
    JS_DefineConstDoubles(_jsCtx, canvas, amityConstants);
    JSFunctionSpec canvasFunctions[] = {
        JS_FS("save", (bindFunction<Script, &Script::amity_canvas_save>), 1, 0),
        JS_FS("restore", (bindFunction<Script, &Script::amity_canvas_restore>), 1, 0),
        JS_FS("rotate", (bindFunction<Script, &Script::amity_canvas_rotate>), 1, 0),
        JS_FS("translate", (bindFunction<Script, &Script::amity_canvas_translate>), 2, 0),
        JS_FS("scale", (bindFunction<Script, &Script::amity_canvas_scale>), 2, 0),
        JS_FS("drawTexture", (bindFunction<Script, &Script::amity_canvas_drawTexture>), 3, 0),
        JS_FS_END
    };
    JS_DefineFunctions(_jsCtx, canvas, canvasFunctions);
    JS_DefineProperty(_jsCtx, canvas, "alpha", JSVAL_NULL,
        JS_PropertyStub, bindProperty<Script, &Script::amity_canvas_setAlpha>, 0);

    // Init all handles
    TextureHandle::init(_jsCtx);
}

Script::~Script ()
{
    if (_jsCtx != NULL) {
        JS_DestroyContext(_jsCtx);
    }
}

int Script::parse (const char* filename, const char* source)
{
    if (rt == NULL) {
        rt = JS_NewRuntime(8L * 1024L * 1024L);
        if (rt == NULL) {
            return 1;
        }
    }

    _jsCtx = JS_NewContext(rt, 8192);
    if (_jsCtx == NULL) {
        return 1;
    }

    // Attach 'this' to the context so we can look it up in bound functions
    JS_SetContextPrivate(_jsCtx, this);

    JS_SetOptions(_jsCtx,
        JSOPTION_WERROR |
        JSOPTION_VAROBJFIX |
        JSOPTION_JIT | JSOPTION_METHODJIT |
        JSOPTION_NO_SCRIPT_RVAL);
    JS_SetVersion(_jsCtx, JSVERSION_ECMA_5);
    JS_SetErrorReporter(_jsCtx, scriptReportError);

    JSObject* global = JS_NewGlobalObject(_jsCtx, &classGlobal);
    if (global == NULL) {
       return 1;
    }

    if (!JS_InitStandardClasses(_jsCtx, global)) {
       return 1;
    }
    initAmityClasses();

    JSBool success = JS_EvaluateScript(_jsCtx, global, source, strlen(source),
        filename, 0, NULL);

    return !success;
}

void Script::onEnterFrame (unsigned int dt)
{
    jsval fval;
    if (!JS_GetPropertyById(_jsCtx, _eventObj, _onEnterFrame, &fval)) {
        return;
    }

    jsval argv[] = { INT_TO_JSVAL(dt) };
    jsval rval;
    JS_CallFunctionValue(_jsCtx, _eventObj, fval, sizeof(argv), argv, &rval);

    // TODO: Do we need to call this every frame?
    JS_MaybeGC(_jsCtx);
}

static JSObject* createMouseEvent (JSContext* jsCtx, int x, int y)
{
    JSObject* event = JS_NewObject(jsCtx, NULL, NULL, JS_GetGlobalObject(jsCtx));
    JS_DefineProperty(jsCtx, event, "x", INT_TO_JSVAL(x),
        JS_PropertyStub, JS_PropertyStub, 0);
    JS_DefineProperty(jsCtx, event, "y", INT_TO_JSVAL(y),
        JS_PropertyStub, JS_PropertyStub, 0);
    return event;
}

void Script::onEvent (const SDL_Event* event)
{
    jsid handler;
    JSObject* obj;

    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
            handler = (event->type == SDL_MOUSEBUTTONDOWN) ? _onMouseDown : _onMouseUp;
            obj = createMouseEvent(_jsCtx, event->button.x, event->button.y);
            JS_DefineProperty(_jsCtx, obj, "button", INT_TO_JSVAL(event->button.button),
                JS_PropertyStub, JS_PropertyStub, 0);
            break;
        case SDL_MOUSEMOTION:
            handler = _onMouseMove;
            obj = createMouseEvent(_jsCtx, event->motion.x, event->motion.y);
            JS_DefineProperty(_jsCtx, obj, "deltaX", INT_TO_JSVAL(event->motion.xrel),
                JS_PropertyStub, JS_PropertyStub, 0);
            JS_DefineProperty(_jsCtx, obj, "deltaY", INT_TO_JSVAL(event->motion.xrel),
                JS_PropertyStub, JS_PropertyStub, 0);
            break;
        default:
            return; // Unhandled event
    }

    jsval fval;
    if (!JS_GetPropertyById(_jsCtx, _eventObj, handler, &fval)) {
        return; // Mising handler
    }

    jsval argv[] = { OBJECT_TO_JSVAL(obj) };
    jsval rval;
    JS_CallFunctionValue(_jsCtx, _eventObj, fval, sizeof(argv), argv, &rval);
}
