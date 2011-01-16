LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_C_INCLUDES := jni/SDL/include

# Add your application source files here...
LOCAL_SRC_FILES := main.cpp \
    ../SDL/src/main/android/SDL_android_main.cpp

LOCAL_SHARED_LIBRARIES := SDL

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
