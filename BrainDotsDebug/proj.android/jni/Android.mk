LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path, $(LOCAL_PATH))

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

CPP_FILES := $(shell find $(LOCAL_PATH)/../../Classes -name *.cpp)
C_FILES := $(shell find $(LOCAL_PATH)/../../Classes -name *.c)
CC_FILES := $(shell find $(LOCAL_PATH)/../../Classes -name *.cc)
LOCAL_SRC_FILES := hellocpp/main.cpp $(CPP_FILES:$(LOCAL_PATH)/%=%) $(C_FILES:$(LOCAL_PATH)/%=%) $(CC_FILES:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/SoundManager \
					$(LOCAL_PATH)/../../Classes/Gif \
					$(LOCAL_PATH)/../../Classes/Clipper \
					$(LOCAL_PATH)/../../Classes/B2DebugDraw \
					$(LOCAL_PATH)/../../Classes/Gif/gif_lib \
					$(LOCAL_PATH)/../../Classes/MenuScene
# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
