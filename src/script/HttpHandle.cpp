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
    JSBool post;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "b", &post)) {
        return JS_FALSE;
    }
    _http.setPost(post);

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
    const char* url;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "s", &url)) {
        return JS_FALSE;
    }

    HttpHandle* handle = HttpHandle::create(jsCtx);
    handle->_http.setUrl(url);
    JS_SET_RVAL(jsCtx, vp, OBJECT_TO_JSVAL(handle->getJSObject()));

    return JS_TRUE;
}
