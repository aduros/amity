LOCAL_PATH:= $(call my-dir)/SDL_image

include $(CLEAR_VARS)

LOCAL_MODULE := SDL_image

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../SDL/include $(LOCAL_PATH)/../../png/png
LOCAL_CFLAGS := -DLOAD_PNG -include $(LOCAL_PATH)/../../png/png/scripts/pnglibconf.h.prebuilt

LOCAL_SRC_FILES := $(notdir $(filter-out %/showimage.c, $(wildcard $(LOCAL_PATH)/*.c)))

LOCAL_SHARED_LIBRARIES := SDL

LOCAL_STATIC_LIBRARIES := png

LOCAL_LDLIBS := -lz

include $(BUILD_SHARED_LIBRARY)
