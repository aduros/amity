#pragma once

class JSContext;
class JSFunction;
class AmityContext;

class Script
{
public:
    Script (AmityContext* amityCtx) : _amityCtx(amityCtx), _jsCtx(0), _onEnterFrame(0) { }
    ~Script();

    int parse (const char* filename, const char* source);

    void onEnterFrame (unsigned int dt);
    inline void setOnEnterFrame (JSFunction* fn) {
        _onEnterFrame = fn;
    };

    inline AmityContext* getAmityCtx () {
        return _amityCtx;
    };

protected:
    AmityContext* _amityCtx;
    JSContext* _jsCtx;
    JSFunction* _onEnterFrame;
};
