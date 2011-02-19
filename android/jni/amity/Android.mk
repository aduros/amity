LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_C_INCLUDES := \
    jni/SDL/include \
    jni/SDL_image \
    jni/SDL_mixer

# Add your application source files here...
LOCAL_SRC_FILES := main.cpp \
    ../SDL/src/main/android/SDL_android_main.cpp

LOCAL_SHARED_LIBRARIES := SDL SDL_image SDL_mixer

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
