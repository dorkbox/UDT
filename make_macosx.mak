#**********************************************************************
# Makefile for creating the Carbon eclipse launcher program.
#
# This makefile expects the following environment variables set:
#
# cross compile libs from : https://launchpad.net/~flosoft/+archive/cross-apple/+packages
#
# This makefile expect the following environment variables set:
#
# COMPILE_OS      - linux, macosx, windows
# COMPILE_OS_ARCH - 32, 64, arm
#*******************************************************************************

include make_common.mak

DIST_NAME=libbarchart-udt-core-2.3.1-mod.dylib
DIST_PATH=windows_$(COMPILE_OS_ARCH)

CPP=i686-apple-darwin10-g++
CC=i686-apple-darwin10-gcc

LDFLAGS = -shared -dynamiclib -compatibility_version 1.0 -current_version 1.0 -mmacosx-version-min=10.5
LIBS = -lpthread -lm

#setup architecture parameters
ifeq ($(COMPILE_OS_ARCH),32)
  CCFLAGS += -arch i386
else
  MACOSX_IS_64BIT=true
  CCFLAGS += -arch x86_64
endif

CCFLAGS += -DMACOSX

all: dist-clean udt
	@echo "\nDONE WITH COMPILE...."
	@mkdir -p "bin/$(DIST_OS_NAME)"
	@mv "$(DIST_NAME)" "bin/$(DIST_OS_NAME)"
	@# now cleanup
	@$(MAKE) -s -f make_common.mak clean

