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

include make_common.mak

DIST_NAME=$(CORE_NAME).so
DIST_PATH=windows_$(COMPILE_OS_ARCH)

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
	@mv "$(DIST_NAME)" "$(TARGET_PATH)/$(DIST_OS_NAME)"
	@# now cleanup
	@$(MAKE) -s -f make_common.mak clean

