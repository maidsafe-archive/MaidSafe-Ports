LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE           := android_port
LOCAL_SRC_FILES        := interface_wrap.cc
LOCAL_CPP_FEATURES     := rtti exceptions

include $(BUILD_SHARED_LIBRARY)
