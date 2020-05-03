#
# Component Makefile
#

COMPONENT_SRCDIRS := .
COMPONENT_ADD_INCLUDEDIRS := include

COMPONENT_REQUIRES := spi_flash idf_test

ifeq ($(GCC_NOT_5_2_0), 1)
unity.o: CFLAGS += -Wno-unused-const-variable
endif
