#*******************************************************************************
# Makefile for creating the launcher program.
#
# This makefile expects the following environment variables set:
#
# Allow for cross-compiling under linux.
# USES  mingw-w64 !!!! (NOT mingw32, which is deprecated)
#
#*******************************************************************************

include make_common.mak

DIST_NAME=libbarchart-udt-core-2.3.1-mod.dll
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
	@mkdir -p "bin/$(DIST_OS_NAME)"
	@mv "$(DIST_NAME)" "bin/$(DIST_OS_NAME)"
	@$(MAKE) -s -f make_common.mak clean

