#include "script/Script.h"

#include "SDL.h"

#include "AmityContext.h"
#include "log.h"
#include "assets.h"

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

static JSRuntime* rt = NULL;
static JSClass scriptClassGlobal = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

static void scriptReportError (JSContext* jsCtx, const char* message, JSErrorReport* report)
{
    LOGW("%s:%u:%s\n",
        report->filename ? report->filename : "<no filename>",
        (unsigned int) report->lineno,
        message);
}

static JSBool amity_log (JSContext* jsCtx, uintN argc, jsval* vp)
{
    JSString* message;
    if (!JS_ConvertArguments(jsCtx, argc, JS_ARGV(jsCtx, vp), "S", &message)) {
        return JS_FALSE;
    }

    LOGI("amity.log() -> %s", JS_EncodeString(jsCtx, message));

    return JS_TRUE;
}

SCRIPT_PROPERTY (Script::setOnEnterFrame, jsCtx, obj, id, vp)
{
    JSFunction* fn = JS_ValueToFunction(jsCtx, vp[0]);
    if (fn == NULL) {
        return JS_FALSE;
    }

    _onEnterFrame = fn;
    return JS_TRUE;
}

SCRIPT_PROPERTY (Script::setOnMouseMove, jsCtx, obj, id, vp)
{
    JSFunction* fn = JS_ValueToFunction(jsCtx, vp[0]);
    if (fn == NULL) {
        return JS_FALSE;
    }

    _onMouseMove = fn;
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::canvasSave, jsCtx, argc, vp)
{
    _amityCtx->canvas.save();
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::canvasRestore, jsCtx, argc, vp)
{
    _amityCtx->canvas.restore();
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::canvasRotate, jsCtx, argc, vp)
{
    jsval* argv = JS_ARGV(jsCtx, vp);
    double angle;
    if (!JS_ValueToNumber(jsCtx, argv[0], &angle)) {
        return JS_FALSE;
    }

    _amityCtx->canvas.rotate(angle);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::canvasTranslate, jsCtx, argc, vp)
{
    jsval* argv = JS_ARGV(jsCtx, vp);
    double x, y;
    if (!JS_ValueToNumber(jsCtx, argv[0], &x) ||
        !JS_ValueToNumber(jsCtx, argv[1], &y)) {
        return JS_FALSE;
    }

    _amityCtx->canvas.translate(x, y);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::canvasScale, jsCtx, argc, vp)
{
    jsval* argv = JS_ARGV(jsCtx, vp);
    double x, y;
    if (!JS_ValueToNumber(jsCtx, argv[0], &x) ||
        !JS_ValueToNumber(jsCtx, argv[1], &y)) {
        return JS_FALSE;
    }

    _amityCtx->canvas.scale(x, y);
    return JS_TRUE;
}

SCRIPT_FUNCTION (Script::canvasDrawTestImage, jsCtx, argc, vp)
{
    jsval* argv = JS_ARGV(jsCtx, vp);
    double x, y;
    if (!JS_ValueToNumber(jsCtx, argv[0], &x) ||
        !JS_ValueToNumber(jsCtx, argv[1], &y)) {
        return JS_FALSE;
    }

    static Texture* testTexture = Texture::fromAsset(SDCARD("tentacle.bmp"));

    _amityCtx->canvas.drawImage(testTexture, x, y);
    return JS_TRUE;
}

SCRIPT_PROPERTY (Script::setAlpha, jsCtx, obj, id, vp)
{
    double alpha;
    if (!JS_ValueToNumber(jsCtx, vp[0], &alpha)) {
        return JS_FALSE;
    }

    _amityCtx->canvas.setAlpha(alpha);
    return JS_TRUE;
}

void Script::initAmityClasses ()
{
    JSObject* global = JS_GetGlobalObject(_jsCtx);

    JSObject* amity = JS_NewObject(_jsCtx, NULL, NULL, global);
    JSFunctionSpec amityFunctions[] = {
        JS_FS("log", amity_log, 1, 0),
        JS_FS_END
    };
    JS_DefineFunctions(_jsCtx, amity, amityFunctions);

    JS_DefineProperty(_jsCtx, amity, "onEnterFrame", JSVAL_NULL,
        JS_PropertyStub, bindProperty<Script, &Script::setOnEnterFrame>, 0);
    JS_DefineProperty(_jsCtx, amity, "onMouseMove", JSVAL_NULL,
        JS_PropertyStub, bindProperty<Script, &Script::setOnMouseMove>, 0);

    JSObject* canvas = JS_NewObject(_jsCtx, NULL, NULL, NULL);
    JSFunctionSpec canvasFunctions[] = {
        JS_FS("save", (bindFunction<Script, &Script::canvasSave>), 1, 0),
        JS_FS("restore", (bindFunction<Script, &Script::canvasRestore>), 1, 0),
        JS_FS("rotate", (bindFunction<Script, &Script::canvasRotate>), 1, 0),
        JS_FS("translate", (bindFunction<Script, &Script::canvasTranslate>), 2, 0),
        JS_FS("scale", (bindFunction<Script, &Script::canvasScale>), 2, 0),
        JS_FS("drawTestImage", (bindFunction<Script, &Script::canvasDrawTestImage>), 2, 0),
        JS_FS_END
    };
    JS_DefineFunctions(_jsCtx, canvas, canvasFunctions);
    JS_DefineProperty(_jsCtx, canvas, "alpha", JSVAL_NULL,
        JS_PropertyStub, bindProperty<Script, &Script::setAlpha>, 0);
    JS_DefineProperty(_jsCtx, amity, "canvas", OBJECT_TO_JSVAL(canvas), NULL, NULL, 0);

    JS_DefineProperty(_jsCtx, global, "$amity", OBJECT_TO_JSVAL(amity), NULL, NULL, 0);
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
        LOGI("Creating runtime");
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

    // TODO: Re-enable JSOPTION_JIT and JSOPTION_METHODJIT when they stop crashing my emulator
    JS_SetOptions(_jsCtx,
        JSOPTION_STRICT |
        JSOPTION_WERROR |
        JSOPTION_VAROBJFIX |
        JSOPTION_NO_SCRIPT_RVAL);
    JS_SetVersion(_jsCtx, JSVERSION_ECMA_5);
    JS_SetErrorReporter(_jsCtx, scriptReportError);

    JSObject* global = JS_NewGlobalObject(_jsCtx, &scriptClassGlobal);
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
    if (_onEnterFrame != NULL) {
        jsval argv[] = { INT_TO_JSVAL(dt) };
        jsval rval;
        JS_CallFunction(_jsCtx, JS_GetGlobalObject(_jsCtx), _onEnterFrame, 1, argv, &rval);
    }

    // TODO: Put this in a separate method?
    JS_MaybeGC(_jsCtx);
}

void Script::onEvent (const SDL_Event* event)
{
    JSFunction* handler;
    JSObject* obj;

    switch (event->type) {
        case SDL_MOUSEMOTION:
            handler = _onMouseMove;
            // TODO: Make a TouchEvent JS class
            obj = JS_NewObject(_jsCtx, NULL, NULL, JS_GetGlobalObject(_jsCtx));
            JS_DefineProperty(_jsCtx, obj, "x", INT_TO_JSVAL(event->motion.x),
                JS_PropertyStub, JS_PropertyStub, 0);
            JS_DefineProperty(_jsCtx, obj, "y", INT_TO_JSVAL(event->motion.y),
                JS_PropertyStub, JS_PropertyStub, 0);
            break;
        default:
            // Unhandled event
            return;
    }

    jsval argv = OBJECT_TO_JSVAL(obj);
    jsval rval;
    JS_CallFunction(_jsCtx, JS_GetGlobalObject(_jsCtx), handler, 1, &argv, &rval);
}
