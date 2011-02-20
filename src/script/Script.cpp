#include "script/Script.h"

#define XP_UNIX
#include "jsapi.h"
#undef XP_UNIX

#include "log.h"

static JSRuntime* rt = NULL;

static JSClass scriptClassGlobal = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
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
    LOGI("amity_log %i", argc);

    JSString* message;
    if (!JS_ConvertArguments(ctx, argc, JS_ARGV(ctx, vp), "S", &message)) {
        return JS_FALSE;
    }

    LOGI("amity.log() -> %s", JS_EncodeString(ctx, message));

    return JS_TRUE;
}

static JSClass scriptClassAmity = {
    "Amity", 0,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
};
static JSFunctionSpec scriptClassAmity_staticMethods[] = {
    // JS_TN?
    JS_FS("log", amity_log, 1, 0),
    JS_FS_END
};

static void scriptInitClasses (JSContext* ctx, JSObject* global)
{
    JSObject* amity = JS_NewObject(ctx, &scriptClassAmity, NULL, global);
    JS_DefineProperty(ctx, global, "$amity", OBJECT_TO_JSVAL(amity), NULL, NULL, 0);
    JS_DefineFunctions(ctx, amity, scriptClassAmity_staticMethods);
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
        rt = JS_NewRuntime(8L * 1024L * 1024L);
        if (rt == NULL) {
            return 1;
        }
    }

    _ctx = JS_NewContext(rt, 8192);
    if (_ctx == NULL) {
        return 1;
    }

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

    JSBool success = JS_EvaluateScript(_ctx, global, source, strlen(source),
        filename, 0, NULL);

    return !success;
}

void Script::onEnterFrame (float dt)
{
    // TODO: Call into js
}
