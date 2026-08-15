LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libshim_ssl
LOCAL_SRC_FILES := SSLCompat.cpp

LOCAL_SHARED_LIBRARIES := \
    libssl

LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)