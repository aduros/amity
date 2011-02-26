#include "script/Script.h"

#define XP_UNIX
#include "jsapi.h"
#undef XP_UNIX

#include "AmityContext.h"
#include "log.h"

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

static JSBool amity_onEnterFrame_set (JSContext* jsCtx, JSObject* obj, jsid id, jsval *vp)
{
    JSFunction* fn = JS_ValueToFunction(jsCtx, vp[0]);
    if (fn == NULL) {
        return JS_FALSE;
    }

    Script* script = static_cast<Script*>(JS_GetContextPrivate(jsCtx));
    script->setOnEnterFrame(fn);

    return JS_TRUE;
}

static JSBool amity_canvas_save (JSContext* jsCtx, uintN argc, jsval* vp)
{
    Script* script = static_cast<Script*>(JS_GetContextPrivate(jsCtx));
    script->getAmityCtx()->canvas.save();
    return JS_TRUE;
}

static JSBool amity_canvas_restore (JSContext* jsCtx, uintN argc, jsval* vp)
{
    Script* script = static_cast<Script*>(JS_GetContextPrivate(jsCtx));
    script->getAmityCtx()->canvas.restore();
    return JS_TRUE;
}

static JSBool amity_canvas_rotate (JSContext* jsCtx, uintN argc, jsval* vp)
{
    jsval* argv = JS_ARGV(jsCtx, vp);
    double angle;
    if (!JS_ValueToNumber(jsCtx, argv[0], &angle)) {
        return JS_FALSE;
    }

    Script* script = static_cast<Script*>(JS_GetContextPrivate(jsCtx));
    script->getAmityCtx()->canvas.rotate(angle);
    return JS_TRUE;
}

static JSBool amity_canvas_translate (JSContext* jsCtx, uintN argc, jsval* vp)
{
    jsval* argv = JS_ARGV(jsCtx, vp);
    double x, y;
    if (!JS_ValueToNumber(jsCtx, argv[0], &x) ||
        !JS_ValueToNumber(jsCtx, argv[1], &y)) {
        return JS_FALSE;
    }

    Script* script = static_cast<Script*>(JS_GetContextPrivate(jsCtx));
    script->getAmityCtx()->canvas.translate(x, y);
    return JS_TRUE;
}

static JSBool amity_canvas_drawTestImage (JSContext* jsCtx, uintN argc, jsval* vp)
{
    jsval* argv = JS_ARGV(jsCtx, vp);
    double x, y;
    if (!JS_ValueToNumber(jsCtx, argv[0], &x) ||
        !JS_ValueToNumber(jsCtx, argv[1], &y)) {
        return JS_FALSE;
    }

    Texture fakeTexture = { 0, 60, 60 };
    Script* script = static_cast<Script*>(JS_GetContextPrivate(jsCtx));
    script->getAmityCtx()->canvas.drawImage(&fakeTexture, x, y);
    return JS_TRUE;
}

static void initAmityClasses (JSContext* jsCtx, JSObject* global)
{
    JSObject* amity = JS_NewObject(jsCtx, NULL, NULL, global);
    JSFunctionSpec amityFunctions[] = {
        JS_FS("log", amity_log, 1, 0),
        JS_FS_END
    };
    JS_DefineFunctions(jsCtx, amity, amityFunctions);

    JS_DefineProperty(jsCtx, amity, "onEnterFrame", JSVAL_NULL,
        JS_PropertyStub, amity_onEnterFrame_set, 0);

    JSObject* canvas = JS_NewObject(jsCtx, NULL, NULL, NULL);
    JSFunctionSpec canvasFunctions[] = {
        JS_FS("save", amity_canvas_save, 0, 0),
        JS_FS("restore", amity_canvas_restore, 0, 0),
        JS_FS("rotate", amity_canvas_rotate, 1, 0),
        JS_FS("translate", amity_canvas_translate, 2, 0),
        JS_FS("drawTestImage", amity_canvas_drawTestImage, 2, 0),
        JS_FS_END
    };
    JS_DefineFunctions(jsCtx, canvas, canvasFunctions);
    JS_DefineProperty(jsCtx, amity, "canvas", OBJECT_TO_JSVAL(canvas), NULL, NULL, 0);

    JS_DefineProperty(jsCtx, global, "$amity", OBJECT_TO_JSVAL(amity), NULL, NULL, 0);
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
    initAmityClasses(_jsCtx, global);

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
