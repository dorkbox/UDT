#*******************************************************************************
# Makefile for creating the GTK launcher program.
#
# This makefile expects the utility "pkg-config" to be in the PATH.
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

DIST_NAME=$(CORE_NAME).so
DIST_PATH=linux_$(COMPILE_OS_ARCH)

CPP=g++
STRIP=strip

LDFLAGS = -shared
LIBS = -lpthread -ldl -lm 

CCFLAGS += \
-DLINUX \
-isystem/usr/include \
-pthread \

all: dist-clean udt
	@echo "\nDONE WITH COMPILE...."
	$(STRIP) --strip-debug --strip-unneeded $(DIST_NAME)
	@mkdir -p "$(TARGET_PATH)/$(DIST_OS_NAME)"
	mv "$(DIST_NAME)" "$(TARGET_PATH)/$(DIST_OS_NAME)"
	@# now cleanup
	@$(MAKE) -s -f make_common.mak clean

