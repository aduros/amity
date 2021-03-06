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

    Http http;

protected:
    SCRIPT_FUNCTION (setHeader, jsCtx, argc, vp);
    SCRIPT_FUNCTION (send, jsCtx, argc, vp);
};

SCRIPT_FUNCTION (amity_net_createHttpRequest, jsCtx, argc, vp);
