LOCAL_PATH := $(call my-dir)

## Also include mozjs' dependencies

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

#include $(CLEAR_VARS)

#LOCAL_MODULE := mozjs
#LOCAL_SRC_FILES := android-build/js/libmozjs.so
#LOCAL_SHARED_LIBRARIES := nspr4 plc4 plds4

# TODO: This is supposed to publish the includes up to the main build. It doesn't seem to work
# though, possibly because we're not using NDK r5's module system?
LOCAL_EXPORT_C_INCLUDES := \
    mozilla-central/src/js \
    android-build/js

#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := nspr4
#LOCAL_SRC_FILES := android-build/nspr/dist/lib/libnspr4.a
#include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := plc4
#LOCAL_SRC_FILES := android-build/nspr/dist/lib/libplc4.a
#include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := plds4
#LOCAL_SRC_FILES := android-build/nspr/dist/lib/libplds4.a
#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := mozjs
LOCAL_STATIC_LIBRARIES := nspr4 plc4 plds4 libstdc++
LOCAL_SRC_FILES := android-build/js/libjs_static.a
include $(PREBUILT_STATIC_LIBRARY)
