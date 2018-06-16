LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_MODULE := libfontdecode
LOCAL_SRC_FILES := src/native-fun.cpp \
                   src/path-extractor.cpp \
                   src/fthelper.cpp

LOCAL_LDLIBS    := -llog -landroid

LOCAL_LDFLAGS := $(LOCAL_PATH)/lib/libfreetype.a

LOCAL_CFLAGS += -I$(LOCAL_PATH)/include \
                -I$(LOCAL_PATH)/include/freetype2 \
                -I$(LOCAL_PATH)/include/freetype2/freetype \
                -I$(LOCAL_PATH)/include/freetype2/freetype/config \
                -I$(LOCAL_PATH)/include/freetype2/freetype/internal

LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/include \
                  -I$(LOCAL_PATH)/include/freetype2 \
                  -I$(LOCAL_PATH)/include/freetype2/freetype \
                  -I$(LOCAL_PATH)/include/freetype2/freetype/config \
                  -I$(LOCAL_PATH)/include/freetype2/freetype/internal

include $(BUILD_SHARED_LIBRARY)