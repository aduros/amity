LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := nspr4
LOCAL_SRC_FILES := android-build/nspr/dist/bin/libnspr4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := plc4
LOCAL_SRC_FILES := android-build/nspr/dist/bin/libplc4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := plds4
LOCAL_SRC_FILES := android-build/nspr/dist/bin/libplds4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := mozjs
LOCAL_SRC_FILES := android-build/js/dist/lib/libmozjs.so
include $(PREBUILT_SHARED_LIBRARY)
