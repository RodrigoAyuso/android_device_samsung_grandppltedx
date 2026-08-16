LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := unwindstack_shim.cpp

LOCAL_MODULE := libshim_unwindstack
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MULTILIB := 32

include $(BUILD_SHARED_LIBRARY)