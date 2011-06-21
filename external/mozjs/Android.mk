LOCAL_PATH := $(call my-dir)/../build/android/lib

include $(CLEAR_VARS)
LOCAL_MODULE := nspr4
LOCAL_SRC_FILES := libnspr4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := plc4
LOCAL_SRC_FILES := libplc4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := plds4
LOCAL_SRC_FILES := libplds4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := mozjs
LOCAL_SRC_FILES := libmozjs.so
include $(PREBUILT_SHARED_LIBRARY)
