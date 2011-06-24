//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#pragma once

#include "script/Handle.h"
#include "script/util.h"
#include "net/Http.h"

class HttpHandle : public Handle<HttpHandle>
{
public:
    static JSPropertySpec jsProperties[];
    static JSFunctionSpec jsFunctions[];
    static JSClass jsClass;

protected:
    SCRIPT_FUNCTION (setHeader, jsCtx, argc, vp);
    SCRIPT_FUNCTION (start, jsCtx, argc, vp);

    friend SCRIPT_FUNCTION (amity_net_createHttpRequest, jsCtx, argc, vp);

    Http _http;
};

SCRIPT_FUNCTION (amity_net_createHttpRequest, jsCtx, argc, vp);
