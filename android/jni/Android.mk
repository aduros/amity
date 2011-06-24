# TODO: Look into using NDK r5's module system instead of this macro
#include $(call all-makefiles-under, ../external)
include ../external/mozjs/Android.mk
include ../external/png/Android.mk
include ../external/SDL/Android.mk
include ../external/SDL_image/Android.mk

LOCAL_PATH := ..

include $(CLEAR_VARS)
LOCAL_MODULE := curl
LOCAL_SRC_FILES  := external/build/android/lib/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_PATH := ..
LOCAL_MODULE := amity

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/src \
    $(LOCAL_PATH)/external/SDL/include \
    $(LOCAL_PATH)/external/SDL_image/SDL_image \
    $(LOCAL_PATH)/external/SDL_mixer/SDL_mixer \
    $(LOCAL_PATH)/external/build/android/include \
    $(LOCAL_PATH)/external/build/android/include/js

# These don't expect to be prefixed by LOCAL_PATH, but includes do (?)
# TODO: Why doesn't $(wildcard) work here?
LOCAL_SRC_FILES := \
    src/assets.cpp \
    src/canvas/CanvasContext.cpp \
    src/canvas/Texture.cpp \
    src/main.cpp \
    src/net/Http.cpp \
    src/script/HttpHandle.cpp \
    src/script/Script.cpp \
    src/script/TextureHandle.cpp \
    src/script/util.cpp \
    external/SDL/src/main/android/SDL_android_main.cpp

LOCAL_SHARED_LIBRARIES := SDL SDL_image nspr4 plc4 plds4 mozjs
LOCAL_STATIC_LIBRARIES := curl

LOCAL_LDLIBS := -lGLESv1_CM -llog -lstdc++ -ldl

# Seems like this might be required for JIT on ARMv6+ ?
#LOCAL_DISABLE_NO_EXECUTE := true

include $(BUILD_SHARED_LIBRARY)
