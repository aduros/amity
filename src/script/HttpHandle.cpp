//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#include "script/HttpHandle.h"

#include "SDL.h"

#include "script/Script.h"
#include "script/util.h"
#include "log.h"

JSPropertySpec HttpHandle::jsProperties[] = { NULL };
JSFunctionSpec HttpHandle::jsFunctions[] = {
    JS_FS("setHeader", HttpHandle::bindFunction<&HttpHandle::setHeader>, 2, 0),
    JS_FS("send", HttpHandle::bindFunction<&HttpHandle::send>, 1, 0),
    JS_FS_END
};
DEFINE_HANDLE_JSCLASS(HttpHandle, "Http");

struct HttpThreadData {
    HttpHandle* handle;
    JSContext* jsCtx;
};

static int threadSend (HttpThreadData* data)
{
    HttpHandle* handle = data->handle;
    JSContext* jsCtx = data->jsCtx;

    bool success = handle->http.send();
    AmityEvent* event = new AmityEvent();
    const char* param;

    JS_BeginRequest(jsCtx);
    if (success) {
        event->handler = getInternedId(jsCtx, "onComplete");
        param = handle->http.getData();
    } else {
        event->handler = getInternedId(jsCtx, "onError");
        param = handle->http.getErrorMessage();
    }
    event->param = STRING_TO_JSVAL(JS_NewStringCopyZ(jsCtx, param));
    event->obj = handle->getJSObject();
    postEvent(jsCtx, event);
    JS_EndRequest(jsCtx);

    delete data;
}

SCRIPT_FUNCTION (HttpHandle::setHeader, jsCtx, argc, vp)
{
    LOGI("TODO: HttpHandle.setHeader");
    return JS_TRUE;
}

SCRIPT_FUNCTION (HttpHandle::send, jsCtx, argc, vp)
{
    jsval val = JS_ARGV(jsCtx, vp)[0];
    if (JSVAL_IS_STRING(val)) {
        JSString* str = JSVAL_TO_STRING(val);
        char* postData = JS_EncodeString(jsCtx, str);
        int length = JS_GetStringEncodingLength(jsCtx, str);
        http.setPostData(postData, length);
        JS_free(jsCtx, postData);
    } else {
        http.setPostData(NULL, 0);
    }

    HttpThreadData* threadData = new HttpThreadData();
    threadData->handle = this;
    threadData->jsCtx = jsCtx;

    SDL_Thread* thread = SDL_CreateThread((SDL_ThreadFunction) threadSend, threadData);
    if (thread == NULL) {
        LOGW("Could not create HTTP thread: %s", SDL_GetError());
        // Do it on the main thread if we have to
        threadSend(threadData);
    }

    return JS_TRUE;
}

SCRIPT_FUNCTION (amity_net_createHttpRequest, jsCtx, argc, vp)
{
    JSString* url;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "S", &url)) {
        return JS_FALSE;
    }

    HttpHandle* handle = HttpHandle::create(jsCtx);
    char* str = JS_EncodeString(jsCtx, url);
    handle->http.setUrl(str);
    JS_free(jsCtx, str);
    JS_SET_RVAL(jsCtx, vp, OBJECT_TO_JSVAL(handle->getJSObject()));

    return JS_TRUE;
}
