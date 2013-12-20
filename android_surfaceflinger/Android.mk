LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=		\
	surfaceflinger.cpp	\
	demo.c

LOCAL_MODULE_TAGS:=optional

LOCAL_MODULE:= surfaceflinger_demo

LOCAL_SHARED_LIBRARIES := libui libutils libgui libcutils libutils libandroidfw

LOCAL_C_INCLUDE := $(call include_path_for, corecg graphics)

include $(BUILD_EXECUTABLE)
