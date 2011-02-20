LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := mozjs
LOCAL_SRC_FILES := android-build/js/libmozjs.so

# TODO: This is supposed to publish the includes up to the main build. It would be sweet if it
# actually worked.
LOCAL_EXPORT_C_INCLUDES := \
    mozilla-central/src/js \
    android-build/js

include $(PREBUILT_SHARED_LIBRARY)
