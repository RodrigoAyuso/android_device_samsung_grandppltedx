#
# Copyright (C) 2023-2026 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := audio_shim.cpp

LOCAL_MODULE := libshim_audio
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MULTILIB := 32

LOCAL_HEADER_LIBRARIES := \
    libaudio_system_headers

include $(BUILD_SHARED_LIBRARY)