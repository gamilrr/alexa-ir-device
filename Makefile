.PHONY: monitor

#Compiler Conditionals, comment to disable
#_DEBUG_ENABLE=1 	#enable debug 
#_DIO_ENABLE=1      #enable dual io serial flash mode, qio by default

RTOS_ROOT_DIR=./platform/esp-open-rtos
ESPPORT= /dev/ttyS7

PROGRAM=alexaIR

PROGRAM_SRC_DIR= ./src 
PROGRAM_INC_DIR= ./include

EXTRA_COMPONENTS=${RTOS_ROOT_DIR}/extras/cpp_support

#debug enable 
ifdef _DEBUG_ENABLE
EXTRA_CFLAGS += -DAO_DEBUG
endif

#dio enable
ifdef _DIO_ENABLE
FLASH_MODE=dio #for CP210x USB_UART Bridge
endif 

include ${RTOS_ROOT_DIR}/common.mk

monitor: 
	picocom ${ESPPORT} -b 115200
