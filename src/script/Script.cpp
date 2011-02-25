#include "script/Script.h"

#define XP_UNIX
#include "jsapi.h"
#undef XP_UNIX

#include "log.h"

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
    JSFunction* fn = JS_ValueToFunction(ctx, *vp);
    if (fn == NULL) {
        return JS_FALSE;
    }

    Script* script = static_cast<Script*>(JS_GetContextPrivate(ctx));
    script->setOnEnterFrame(fn);

    return JS_TRUE;
}

static JSClass classAmity = {
    "Amity", 0,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
};
static JSFunctionSpec classAmity_functions[] = {
    // JS_TN?
    JS_FS("log", amity_log, 1, 0),
    JS_FS_END
};

static void scriptInitClasses (JSContext* ctx, JSObject* global)
{
    JSObject* amity = JS_NewObject(ctx, &classAmity, NULL, global);
    JS_DefineProperty(ctx, global, "$amity", OBJECT_TO_JSVAL(amity), NULL, NULL, 0);
    JS_DefineFunctions(ctx, amity, classAmity_functions);

    JS_DefineProperty(ctx, amity, "onEnterFrame", JSVAL_NULL,
        JS_PropertyStub, amity_onEnterFrame_set, 0);
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

    JS_SetOptions(_ctx,
        JSOPTION_STRICT |
        JSOPTION_WERROR |
        JSOPTION_VAROBJFIX |
        JSOPTION_NO_SCRIPT_RVAL |
        JSOPTION_JIT |
        JSOPTION_METHODJIT);
    JS_SetVersion(_ctx, JSVERSION_ECMA_5);
    JS_SetErrorReporter(_ctx, scriptReportError);

    JSObject* global = JS_NewGlobalObject(_ctx, &scriptClassGlobal);
    if (global == NULL) {
       return 1;
    }

    if (!JS_InitStandardClasses(_ctx, global)) {
       return 1;
    }
    scriptInitClasses(_ctx, global);

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
