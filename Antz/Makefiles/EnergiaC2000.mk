#
# embedXcode
# ----------------------------------
# Embedded Computing on Xcode
#
# Copyright © Rei VILO, 2010-2015
# http://embedxcode.weebly.com
# All rights reserved
#
#
# Last update: Jul 05, 2014 release 165

#/Applications/Energia.app/Contents/Resources/Java/hardware/tools/c2000/bin/cl2000 -v28 -ml -mt -g --include_path=/Applications/Energia.app/Contents/Resources/Java/hardware/c2000/cores/c2000 --include_path=/Applications/Energia.app/Contents/Resources/Java/hardware/c2000/variants/launchpad --include_path=/Applications/Energia.app/Contents/Resources/Java/hardware/c2000/../tools/c2000/include --gcc --define=ENERGIA=12 --define=F_CPU=60000000L --define=ARDUINO=101 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency=Builds/main.pp main.cpp --output_file=Builds/main.cpp.o   

# Energia LaunchPad MSP430 and FR5739 specifics
# ----------------------------------
#
APPLICATION_PATH := $(ENERGIA_PATH)
ENERGIA_RELEASE  := $(shell tail -c2 $(APPLICATION_PATH)/lib/version.txt)
ARDUINO_RELEASE  := $(shell head -c4 $(APPLICATION_PATH)/lib/version.txt | tail -c3)

ifeq ($(shell if [[ '$(ENERGIA_RELEASE)' -ge '13' ]] ; then echo 1 ; else echo 0 ; fi ),0)
    $(error Energia release 13 required.)
endif

PLATFORM         := Energia
BUILD_CORE       := c2000
PLATFORM_TAG      = ENERGIA=$(ENERGIA_RELEASE) ARDUINO=$(ARDUINO_RELEASE) EMBEDXCODE=$(RELEASE_NOW) $(filter __%__ ,$(GCC_PREPROCESSOR_DEFINITIONS))

UPLOADER          = mspdebug
MSPDEBUG_PATH     = $(APPLICATION_PATH)/hardware/tools/msp430/mspdebug
MSPDEBUG          = $(MSPDEBUG_PATH)/mspdebug
MSPDEBUG_PROTOCOL = $(call PARSE_BOARD,$(BOARD_TAG),upload.protocol)
MSPDEBUG_OPTS     = $(MSPDEBUG_PROTOCOL) --force-reset

# FraunchPad MSP430FR5739 requires a specific command
#
ifeq ($(BOARD_TAG), lpmsp430fr5739)
    MSPDEBUG_COMMAND = load
else
    MSPDEBUG_COMMAND = prog
endif

APP_TOOLS_PATH   := $(APPLICATION_PATH)/hardware/tools/c2000/bin
CORE_LIB_PATH    := $(APPLICATION_PATH)/hardware/c2000/cores/c2000
APP_LIB_PATH     := $(APPLICATION_PATH)/hardware/c2000/libraries
BOARDS_TXT       := $(APPLICATION_PATH)/hardware/c2000/boards.txt

# Sketchbook/Libraries path
# wildcard required for ~ management
# ?ibraries required for libraries and Libraries
#
ifeq ($(USER_PATH)/Library/Energia/preferences.txt,)
    $(error Error: run Energia once and define the sketchbook path)
endif

ifeq ($(wildcard $(SKETCHBOOK_DIR)),)
    SKETCHBOOK_DIR = $(shell grep sketchbook.path $(wildcard ~/Library/Energia/preferences.txt) | cut -d = -f 2)
endif
ifeq ($(wildcard $(SKETCHBOOK_DIR)),)
    $(error Error: sketchbook path not found)
endif
USER_LIB_PATH  = $(wildcard $(SKETCHBOOK_DIR)/?ibraries)


# Rules for making a c++ file from the main sketch (.pde)
#
PDEHEADER      = \\\#include \"Energia.h\"  


# Tool-chain names
#
CC      = $(APP_TOOLS_PATH)/msp430-gcc
CXX     = $(APP_TOOLS_PATH)/msp430-g++
AR      = $(APP_TOOLS_PATH)/msp430-ar
OBJDUMP = $(APP_TOOLS_PATH)/msp430-objdump
OBJCOPY = $(APP_TOOLS_PATH)/msp430-objcopy
SIZE    = $(APP_TOOLS_PATH)/msp430-size
NM      = $(APP_TOOLS_PATH)/msp430-nm
GDB     = $(APP_TOOLS_PATH)/msp430-gdb

BOARD          = $(call PARSE_BOARD,$(BOARD_TAG),board)
#LDSCRIPT = $(call PARSE_BOARD,$(BOARD_TAG),ldscript)
VARIANT        = $(call PARSE_BOARD,$(BOARD_TAG),build.variant)
VARIANT_PATH   = $(APPLICATION_PATH)/hardware/msp430/variants/$(VARIANT)

ifeq ($(MAKECMDGOALS),debug)
    OPTIMISATION   = -O0 -ggdb
endif

MCU_FLAG_NAME  = mmcu
EXTRA_LDFLAGS  =
#EXTRA_LDFLAGS = -T$(CORE_LIB_PATH)/$(LDSCRIPT)
EXTRA_CPPFLAGS = $(addprefix -D, $(PLATFORM_TAG)) -I$(VARIANT_PATH)

