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
    JS_FS("start", HttpHandle::bindFunction<&HttpHandle::start>, 1, 0),
    JS_FS_END
};
DEFINE_HANDLE_JSCLASS(HttpHandle, "Http");

SCRIPT_FUNCTION (HttpHandle::setHeader, jsCtx, argc, vp)
{
    LOGI("TODO: HttpHandle.setHeader");
    return JS_TRUE;
}

SCRIPT_FUNCTION (HttpHandle::start, jsCtx, argc, vp)
{
    jsval val = JS_ARGV(jsCtx, vp)[0];
    if (JSVAL_IS_STRING(val)) {
        JSString* str = JSVAL_TO_STRING(val);
        char* postData = JS_EncodeString(jsCtx, str);
        int length = JS_GetStringEncodingLength(jsCtx, str);
        _http.setPostData(postData, length);
        JS_free(jsCtx, postData);
    } else {
        _http.setPostData(NULL, 0);
    }

    // TODO(bruno): Do this on another thread
    AmityEvent* event = new AmityEvent();
    const char* param;
    if (_http.send()) {
        event->handler = getInternedId(jsCtx, "onComplete");
        param = _http.getData();
    } else {
        event->handler = getInternedId(jsCtx, "onError");
        param = _http.getErrorMessage();
    }
    event->param = STRING_TO_JSVAL(JS_NewStringCopyZ(jsCtx, param));
    event->obj = getJSObject();
    postEvent(jsCtx, event);

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
    handle->_http.setUrl(str);
    JS_free(jsCtx, str);
    JS_SET_RVAL(jsCtx, vp, OBJECT_TO_JSVAL(handle->getJSObject()));

    return JS_TRUE;
}
