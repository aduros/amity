include $(call all-makefiles-under, ../external)
include $(CLEAR_VARS)

LOCAL_PATH := ..
LOCAL_MODULE := amity

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/external/SDL/include \
    $(LOCAL_PATH)/external/SDL_image \
    $(LOCAL_PATH)/external/SDL_mixer

# These don't expect to be prefixed by LOCAL_PATH, but includes do (?)
LOCAL_SRC_FILES := \
    src/main.cpp \
    external/SDL/src/main/android/SDL_android_main.cpp

LOCAL_SHARED_LIBRARIES := SDL SDL_image SDL_mixer

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
