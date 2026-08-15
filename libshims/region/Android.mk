LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libshim_region

LOCAL_SRC_FILES := \
    RegionLegacy.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) \
    frameworks/native/include \
    frameworks/native/libs/ui/include \
    system/core/libutils/include

LOCAL_SHARED_LIBRARIES := \
    libbase \
    liblog \
    libutils \
    libui

LOCAL_LDFLAGS += \
    -Wl,--version-script,$(LOCAL_PATH)/exports.map \
    -Wl,-Bsymbolic

LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
