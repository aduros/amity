#pragma once

struct JSContext;
struct JSFunction;

class Script
{
public:
    Script () : _ctx(0), _onEnterFrame(0) { }
    ~Script();

    int parse (const char* filename, const char* source);

    void onEnterFrame (unsigned int dt);
    void setOnEnterFrame (JSFunction* fn) {
        _onEnterFrame = fn;
    }

protected:
    JSContext* _ctx;
    JSFunction* _onEnterFrame;
};
