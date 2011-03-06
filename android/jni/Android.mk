# TODO: Look into using NDK r5's module system instead of this macro
#include $(call all-makefiles-under, ../external)
include ../external/SDL/Android.mk
include ../external/mozjs/Android.mk

include $(CLEAR_VARS)

LOCAL_PATH := ..
LOCAL_MODULE := amity

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/src \
    $(LOCAL_PATH)/external/SDL/include \
    $(LOCAL_PATH)/external/SDL_image \
    $(LOCAL_PATH)/external/SDL_mixer \
    $(LOCAL_PATH)/external/mozjs/android-build/js/dist/include

# These don't expect to be prefixed by LOCAL_PATH, but includes do (?)
# TODO: Why doesn't $(wildcard) work here?
LOCAL_SRC_FILES := \
    src/main.cpp \
    src/assets.cpp \
    src/canvas/CanvasContext.cpp \
    src/canvas/Texture.cpp \
    src/script/Script.cpp \
    src/script/TextureObject.cpp \
    external/SDL/src/main/android/SDL_android_main.cpp

LOCAL_SHARED_LIBRARIES := SDL nspr4 plc4 plds4 mozjs

LOCAL_LDLIBS := -lGLESv1_CM -llog -lstdc++ -ldl
#-L obj/local/armeabi -ljs_static -lnspr4 -lplc4 -lplds4 -ldl

# Seems like this might be required for JIT on ARMv6+ ?
#LOCAL_DISABLE_NO_EXECUTE := true

include $(BUILD_SHARED_LIBRARY)
