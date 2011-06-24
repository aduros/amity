//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#include "script/Script.h"

#include "SDL.h"

#include "AmityContext.h"
#include "assets.h"
#include "log.h"
#include "script/HttpHandle.h"
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

SCRIPT_FUNCTION (Script::amity_canvas_drawImage, jsCtx, argc, vp)
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

    Texture* texture = textureHandle->getTexture();
    jsdouble sx, sy, w, h;
    if (argc >= 7) {
        // Convert the optional arguments
        if (!JS_ConvertArguments(jsCtx, 4, JS_ARGV(jsCtx, vp)+3, "dddd", &sx, &sy, &w, &h)) {
            return JS_FALSE;
        }
    } else {
        sx = 0;
        sy = 0;
        w = texture->getWidth();
        h = texture->getHeight();
    }
    _amityCtx->canvas.drawImage(texture, x, y, sx, sy, w, h);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::amity_canvas_drawPattern, jsCtx, argc, vp)
{
    JSObject* textureObj;
    jsdouble x, y, w, h;
    if (!JS_ConvertArguments(jsCtx, 5, JS_ARGV(jsCtx, vp), "odddd", &textureObj, &x, &y, &w, &h)) {
        return JS_FALSE;
    }

    TextureHandle* textureHandle = TextureHandle::getFrom(jsCtx, textureObj);
    if (textureHandle == NULL) {
        return JS_FALSE;
    }
    _amityCtx->canvas.drawPattern(textureHandle->getTexture(), x, y, w, h);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::amity_canvas_fillRect, jsCtx, argc, vp)
{
    uint32 color;
    jsdouble x, y, width, height;
    if (!JS_ConvertArguments(jsCtx, 5, JS_ARGV(jsCtx, vp),
            "udddd", &color, &x, &y, &width, &height)) {
        return JS_FALSE;
    }

    _amityCtx->canvas.fillRect(color, x, y, width, height);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::amity_canvas_multiplyAlpha, jsCtx, argc, vp)
{
    jsdouble factor;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "d", &factor)) {
        return JS_FALSE;
    }
    _amityCtx->canvas.multiplyAlpha(factor);
    return JS_TRUE;
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

    JSObject* net = JS_DefineObject(_jsCtx, amity, "net",
        NULL, NULL, JSPROP_READONLY | JSPROP_PERMANENT);
    JSFunctionSpec netFunctions[] = {
        JS_FS("createHttpRequest", amity_net_createHttpRequest, 1, 0),
        JS_FS_END
    };
    JS_DefineFunctions(_jsCtx, net, netFunctions);

    JSObject* canvas = JS_DefineObject(_jsCtx, amity, "canvas",
        NULL, NULL, JSPROP_READONLY | JSPROP_PERMANENT);
    int screenWidth, screenHeight;
    _amityCtx->canvas.getSize(&screenWidth, &screenHeight);
    JSConstDoubleSpec amityConstants[] = {
        { screenWidth, "WIDTH", 0, { 0, 0, 0 } },
        { screenHeight, "HEIGHT", 0, { 0, 0, 0 } },
        { 0, NULL, 0, { 0, 0, 0 } },
    };
    JS_DefineConstDoubles(_jsCtx, canvas, amityConstants);
    JSFunctionSpec canvasFunctions[] = {
        JS_FS("drawImage", (bindFunction<Script, &Script::amity_canvas_drawImage>), 3, 0),
        JS_FS("drawPattern", (bindFunction<Script, &Script::amity_canvas_drawPattern>), 5, 0),
        JS_FS("fillRect", (bindFunction<Script, &Script::amity_canvas_fillRect>), 5, 0),
        JS_FS("multiplyAlpha", (bindFunction<Script, &Script::amity_canvas_multiplyAlpha>), 1, 0),
        JS_FS("restore", (bindFunction<Script, &Script::amity_canvas_restore>), 1, 0),
        JS_FS("rotate", (bindFunction<Script, &Script::amity_canvas_rotate>), 1, 0),
        JS_FS("save", (bindFunction<Script, &Script::amity_canvas_save>), 1, 0),
        JS_FS("scale", (bindFunction<Script, &Script::amity_canvas_scale>), 2, 0),
        JS_FS("translate", (bindFunction<Script, &Script::amity_canvas_translate>), 2, 0),
        JS_FS_END
    };
    JS_DefineFunctions(_jsCtx, canvas, canvasFunctions);

    // Init all handles
    TextureHandle::init(_jsCtx);
    HttpHandle::init(_jsCtx);
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

    uint32 options = JSOPTION_WERROR |
        JSOPTION_VAROBJFIX |
        JSOPTION_NO_SCRIPT_RVAL;

    // JIT produces strange bugs on webOS, do not enable it there for now
    #ifndef __WEBOS__
    options |= JSOPTION_JIT | JSOPTION_METHODJIT;
    #endif

    JS_SetOptions(_jsCtx, options);
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
    AmityEvent amityEvent = {
        _eventObj,
        _onEnterFrame,
        INT_TO_JSVAL(dt)
    };
    consumeEvent(_jsCtx, &amityEvent);

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
    if (event->type == SDL_USEREVENT) {
        if (event->user.code == AMITY_EVENT_SDL_CODE) {
            AmityEvent* amityEvent = static_cast<AmityEvent*>(event->user.data1);
            JS_RemoveObjectRoot(_jsCtx, &amityEvent->obj);
            JS_RemoveValueRoot(_jsCtx, &amityEvent->param);
            consumeEvent(_jsCtx, amityEvent);
            delete amityEvent;
        } else {
            LOGW("Unrecognized SDL_UserEvent with code %d", event->user.code);
        }
        return; // Return early
    }

    jsid handler;
    JSObject* param;

    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
            handler = (event->type == SDL_MOUSEBUTTONDOWN) ? _onMouseDown : _onMouseUp;
            param = createMouseEvent(_jsCtx, event->button.x, event->button.y);
            JS_DefineProperty(_jsCtx, param, "button", INT_TO_JSVAL(event->button.button),
                JS_PropertyStub, JS_PropertyStub, 0);
            break;
        case SDL_MOUSEMOTION:
            handler = _onMouseMove;
            param = createMouseEvent(_jsCtx, event->motion.x, event->motion.y);
            JS_DefineProperty(_jsCtx, param, "deltaX", INT_TO_JSVAL(event->motion.xrel),
                JS_PropertyStub, JS_PropertyStub, 0);
            JS_DefineProperty(_jsCtx, param, "deltaY", INT_TO_JSVAL(event->motion.xrel),
                JS_PropertyStub, JS_PropertyStub, 0);
            break;
        default:
            return; // Unhandled event
    }

    AmityEvent amityEvent = {
        _eventObj,
        handler,
        OBJECT_TO_JSVAL(param)
    };
    consumeEvent(_jsCtx, &amityEvent);
}
