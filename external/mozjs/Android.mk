LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := mozjs
LOCAL_SRC_FILES := android-build/js/libmozjs.so

# TODO: This is supposed to publish the includes up to the main build. It doesn't seem to work
# though, possibly because we're not using NDK r5's module system?
LOCAL_EXPORT_C_INCLUDES := \
    mozilla-central/src/js \
    android-build/js

include $(PREBUILT_SHARED_LIBRARY)

# Also include mozjs' dependencies

include $(CLEAR_VARS)
LOCAL_MODULE := nspr4
LOCAL_SRC_FILES := android-build/nspr/dist/lib/libnspr4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := plc4
LOCAL_SRC_FILES := android-build/nspr/dist/lib/libplc4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := plds4
LOCAL_SRC_FILES := android-build/nspr/dist/lib/libplds4.so
include $(PREBUILT_SHARED_LIBRARY)
