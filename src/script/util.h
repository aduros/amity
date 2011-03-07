#pragma once

#define XP_UNIX
#include "jsapi.h"
#undef XP_UNIX

#define SCRIPT_FUNCTION(fn,jsCtx,argc,vp) JSBool fn (JSContext* jsCtx, uintN argc, jsval* vp)
#define SCRIPT_PROPERTY(fn,jsCtx,obj,id,vp) JSBool fn (JSContext* jsCtx, JSObject* obj, jsid id, jsval* vp)
