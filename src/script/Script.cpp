#include "script/Script.h"

#define XP_UNIX
#include "jsapi.h"
#undef XP_UNIX

#include "log.h"

#include "canvas/CanvasContext.h"

extern CanvasContext canvas;

static JSRuntime* rt = NULL;
static JSClass scriptClassGlobal = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

static void scriptReportError (JSContext* _ctx, const char* message, JSErrorReport* report)
{
    LOGW("%s:%u:%s\n",
        report->filename ? report->filename : "<no filename>",
        (unsigned int) report->lineno,
        message);
}

static JSBool amity_log (JSContext* ctx, uintN argc, jsval* vp)
{
    JSString* message;
    if (!JS_ConvertArguments(ctx, argc, JS_ARGV(ctx, vp), "S", &message)) {
        return JS_FALSE;
    }

    LOGI("amity.log() -> %s", JS_EncodeString(ctx, message));

    return JS_TRUE;
}

static JSBool amity_onEnterFrame_set (JSContext* ctx, JSObject* obj, jsid id, jsval *vp)
{
    JSFunction* fn = JS_ValueToFunction(ctx, vp[0]);
    if (fn == NULL) {
        return JS_FALSE;
    }

    Script* script = static_cast<Script*>(JS_GetContextPrivate(ctx));
    script->setOnEnterFrame(fn);

    return JS_TRUE;
}

static JSBool amity_canvas_save (JSContext* ctx, uintN argc, jsval* vp)
{
    canvas.save();
    return JS_TRUE;
}

static JSBool amity_canvas_restore (JSContext* ctx, uintN argc, jsval* vp)
{
    canvas.restore();
    return JS_TRUE;
}

static JSBool amity_canvas_rotate (JSContext* ctx, uintN argc, jsval* vp)
{
    jsval* argv = JS_ARGV(ctx, vp);
    double angle;
    if (!JS_ValueToNumber(ctx, argv[0], &angle)) {
        return JS_FALSE;
    }

    canvas.rotate(angle);
    return JS_TRUE;
}

static JSBool amity_canvas_translate (JSContext* ctx, uintN argc, jsval* vp)
{
    jsval* argv = JS_ARGV(ctx, vp);
    double x, y;
    if (!JS_ValueToNumber(ctx, argv[0], &x) ||
        !JS_ValueToNumber(ctx, argv[1], &y)) {
        return JS_FALSE;
    }

    canvas.translate(x, y);
    return JS_TRUE;
}

static JSBool amity_canvas_drawTestImage (JSContext* ctx, uintN argc, jsval* vp)
{
    jsval* argv = JS_ARGV(ctx, vp);
    double x, y;
    if (!JS_ValueToNumber(ctx, argv[0], &x) ||
        !JS_ValueToNumber(ctx, argv[1], &y)) {
        return JS_FALSE;
    }

    Texture fakeTexture = { 0, 60, 60 };
    canvas.drawImage(&fakeTexture, x, y);
    return JS_TRUE;
}

static void initAmityClasses (JSContext* ctx, JSObject* global)
{
    JSObject* amity = JS_NewObject(ctx, NULL, NULL, global);
    JSFunctionSpec amityFunctions[] = {
        JS_FS("log", amity_log, 1, 0),
        JS_FS_END
    };
    JS_DefineFunctions(ctx, amity, amityFunctions);

    JS_DefineProperty(ctx, amity, "onEnterFrame", JSVAL_NULL,
        JS_PropertyStub, amity_onEnterFrame_set, 0);

    JSObject* canvas = JS_NewObject(ctx, NULL, NULL, NULL);
    JSFunctionSpec canvasFunctions[] = {
        JS_FS("save", amity_canvas_save, 0, 0),
        JS_FS("restore", amity_canvas_restore, 0, 0),
        JS_FS("rotate", amity_canvas_rotate, 1, 0),
        JS_FS("translate", amity_canvas_translate, 2, 0),
        JS_FS("drawTestImage", amity_canvas_drawTestImage, 2, 0),
        JS_FS_END
    };
    JS_DefineFunctions(ctx, canvas, canvasFunctions);
    JS_DefineProperty(ctx, amity, "canvas", OBJECT_TO_JSVAL(canvas), NULL, NULL, 0);

    JS_DefineProperty(ctx, global, "$amity", OBJECT_TO_JSVAL(amity), NULL, NULL, 0);
}

Script::~Script ()
{
    if (_ctx != NULL) {
        JS_DestroyContext(_ctx);
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

    _ctx = JS_NewContext(rt, 8192);
    if (_ctx == NULL) {
        return 1;
    }

    // Attach 'this' to the context so we can look it up in bound functions
    JS_SetContextPrivate(_ctx, this);

    // TODO: Re-enable JSOPTION_JIT and JSOPTION_METHODJIT when they stop crashing my emulator
    JS_SetOptions(_ctx,
        JSOPTION_STRICT |
        JSOPTION_WERROR |
        JSOPTION_VAROBJFIX |
        JSOPTION_NO_SCRIPT_RVAL);
    JS_SetVersion(_ctx, JSVERSION_ECMA_5);
    JS_SetErrorReporter(_ctx, scriptReportError);

    JSObject* global = JS_NewGlobalObject(_ctx, &scriptClassGlobal);
    if (global == NULL) {
       return 1;
    }

    if (!JS_InitStandardClasses(_ctx, global)) {
       return 1;
    }
    initAmityClasses(_ctx, global);

    JSBool success = JS_EvaluateScript(_ctx, global, source, strlen(source),
        filename, 0, NULL);

    return !success;
}

void Script::onEnterFrame (unsigned int dt)
{
    if (_onEnterFrame != NULL) {
        jsval argv[] = { INT_TO_JSVAL(dt) };
        jsval rval;
        JS_CallFunction(_ctx, JS_GetGlobalObject(_ctx), _onEnterFrame, 1, argv, &rval);
    }

    // TODO: Put this in a separate method?
    JS_MaybeGC(_ctx);
}
