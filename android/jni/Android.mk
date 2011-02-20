include $(call all-makefiles-under, ../external)

include $(CLEAR_VARS)

LOCAL_PATH := ..
LOCAL_MODULE := amity

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/external/SDL/include \
    $(LOCAL_PATH)/external/SDL_image \
    $(LOCAL_PATH)/external/SDL_mixer \
    $(LOCAL_PATH)/external/mozjs/android-build/js \
    $(LOCAL_PATH)/external/mozjs/mozilla-central/js/src

# These don't expect to be prefixed by LOCAL_PATH, but includes do (?)
# TODO: Why doesn't $(wildcard) work here?
LOCAL_SRC_FILES := \
    src/main.cpp \
    src/canvas/CanvasContext.cpp \
    external/SDL/src/main/android/SDL_android_main.cpp

LOCAL_SHARED_LIBRARIES := SDL SDL_image SDL_mixer mozjs

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
