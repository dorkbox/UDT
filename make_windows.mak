#*******************************************************************************
# Makefile for creating the launcher program.
#
# This makefile expects the following environment variables set:
#
# Allow for cross-compiling under linux.
# USES  mingw-w64 !!!! (NOT mingw32, which is deprecated)
#
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

DIST_NAME=$(CORE_NAME).dll
DIST_PATH=windows_$(COMPILE_OS_ARCH)

ifeq ($(COMPILE_OS_ARCH),32)
  MINGVER   = i686
  #mingw flags
  CCFLAGS +=-DWINVER=0x0501

  #32 bit support is XP+
  CCFLAGS +=-D_WIN32_WINNT=0x0501
else
  MINGVER   = x86_64
  #mingw flags
  CCFLAGS +=-DWINVER=0x0600

  #64 bit support is vista+ ONLY
  CCFLAGS +=-D_WIN32_WINNT=0x0600
endif

CPP    = $(shell which $(MINGVER)-w64-mingw32-g++)
STRIP  = $(shell which $(MINGVER)-w64-mingw32-strip)

 
LDFLAGS = -shared -static -static-libgcc -static-libstdc++ -Wl,--kill-at
LIBS	  = -lkernel32 -luser32 -lws2_32

CCFLAGS += \
-isystem/usr/$(MINGVER)-w64-mingw32/include \
-DWINDOWS \

all: dist-clean udt
	@echo "\nDONE WITH COMPILE...."
	@# now cleanup
	@# now strip the DLL of unneeded, since it can be HUGE
	$(STRIP) --strip-debug --strip-unneeded $(DIST_NAME)
	@mkdir -p "$(TARGET_PATH)/$(DIST_OS_NAME)"
	mv "$(DIST_NAME)" "$(TARGET_PATH)/$(DIST_OS_NAME)"
	@$(MAKE) -s -f make_common.mak clean

