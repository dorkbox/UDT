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
#
#  Copyright 2015 dorkbox, llc
# 
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
######################################################################


include make_common.mak

DIST_NAME=$(CORE_NAME).dylib
DIST_PATH=macosx_$(COMPILE_OS_ARCH)

CPP=i686-apple-darwin10-g++
STRIP=i686-apple-darwin10-strip

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
	@mkdir -p "$(TARGET_PATH)/$(DIST_OS_NAME)"
	mv "$(DIST_NAME)" "$(TARGET_PATH)/$(DIST_OS_NAME)"
	@# now cleanup
	@$(MAKE) -s -f make_common.mak clean

