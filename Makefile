#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := alexa_ir_device

EXTRA_COMPONENT_DIRS := src

EXTRA_CXXFLAGS := $(foreach d, $(ESP_XTENSA_HEADERS), -isystem$d)

include $(IDF_PATH)/make/project.mk
