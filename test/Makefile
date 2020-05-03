
project_name := alexa_ir_device_test

EXTRA_COMPONENT_DIRS := test_framework unit_test

EXTRA_CXXFLAGS := $(foreach d, $(ESP_XTENSA_HEADERS), -isystem$d)

include $(IDF_PATH)/make/project.mk
