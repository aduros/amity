LOCAL_PATH:= $(call my-dir)/png

include $(CLEAR_VARS)

LOCAL_MODULE:= png
LOCAL_SRC_FILES := \
    png.c \
    pngerror.c \
    pngget.c \
    pngmem.c \
    pngpread.c \
    pngread.c \
    pngrio.c \
    pngrtran.c \
    pngrutil.c \
    pngset.c \
    pngtest.c \
    pngtrans.c \
    pngvalid.c \
    pngwio.c \
    pngwrite.c \
    pngwtran.c \
    pngwutil.c

LOCAL_CFLAGS := -include $(LOCAL_PATH)/scripts/pnglibconf.h.prebuilt

include $(BUILD_STATIC_LIBRARY)
