//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#pragma once

#include "script/util.h"

#define DEFINE_HANDLE_JSCLASS(objectType, name) \
    JSClass objectType::jsClass = { \
        name, JSCLASS_HAS_PRIVATE, \
        JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, \
        JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, objectType::finalize, \
    };

/**
 * A C++ object associated with a JSObject. The C++ destructor will be called when the JSObject
 * is garbage collected. See TextureHandle for example usage.
 */
template <class T> // T extends Handle
class Handle
{
public:
    static void init (JSContext* jsCtx) {
        JS_InitClass(jsCtx, JS_GetGlobalObject(jsCtx), NULL, &T::jsClass,
            NULL, 0, T::jsProperties, NULL, NULL, NULL);
    }

    /**
     * Create a new JSObject and Handle and pair them together.
     */
    static T* create (JSContext* jsCtx) {
        JSObject* obj = JS_NewObject(jsCtx, &T::jsClass, NULL, NULL);
        T* priv = new T();
        priv->setJSObject(obj);
        JS_SetPrivate(jsCtx, obj, priv);
        return priv;
    }

    /**
     * Get the handle associated with the given JSObject.
     */
    static T* getFrom (JSContext* jsCtx, JSObject* obj) {
        return static_cast<T*>(JS_GetInstancePrivate(jsCtx, obj, &T::jsClass, NULL));
    }

    /**
     * A JSFinalizeOp that deletes this Handle.
     */
    static void finalize (JSContext* jsCtx, JSObject* obj) {
        delete static_cast<T*>(JS_GetPrivate(jsCtx, obj));
    }

    template <JSBool (T::*fn)(JSContext*, uintN, jsval*)>
    static SCRIPT_FUNCTION (bindFunction, jsCtx, argc, vp) {
        T* self = static_cast<T*>(JS_GetPrivate(jsCtx, JS_THIS_OBJECT(jsCtx, vp)));
        return (self->*fn)(jsCtx, argc, vp);
    }

    template <JSBool (T::*fn)(JSContext*, JSObject*, jsid, jsval*)>
    static SCRIPT_PROPERTY (bindProperty, jsCtx, obj, id, vp) {
        T* self = static_cast<T*>(JS_GetPrivate(jsCtx, obj));
        return (self->*fn)(jsCtx, obj, id, vp);
    }

    inline JSObject* getJSObject () const {
        return _jsObj;
    }

    inline JSObject* setJSObject (JSObject* jsObj) {
        _jsObj = jsObj;
    }

protected:
    Handle () { }

    // The JSObject associated with this Handle
    JSObject* _jsObj;
};
