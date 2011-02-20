#pragma once

struct JSContext;

class Script
{
public:
    Script () : _ctx(0) { }
    ~Script();

    int parse (const char* filename, const char* source);

    void onEnterFrame (float dt);

protected:
    JSContext* _ctx;
};
