################################################################################
# Makefile for creating the app launcher program.
#
# This + parent makefiles expect the following environment variables set:
#
# COMPILE_OS      - linux, macosx, windows
# COMPILE_OS_ARCH - 32, 64, arm
#
################################################################################
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


CORE_NAME=libudt-2.3.2
TARGET_PATH=dist
JVM=jvm


#setup architecture and path parameters
ifeq ($(COMPILE_OS_ARCH),32)
  CCFLAGS +=-DI386
  M_ARCH=-m32
  DIST_OS_NAME=$(COMPILE_OS)_32
else ifeq ($(COMPILE_OS_ARCH),64) 
  CCFLAGS +=-DAMD64
  M_ARCH=-m64
  DIST_OS_NAME=$(COMPILE_OS)_64
else ifeq ($(COMPILE_OS_ARCH),ARM) 
  #todo finish filling this out  
  CCFLAGS +=-DARM
endif


# mingw doesn't support the M_ARCH flag.
ifeq ($(COMPILE_OS),windows)
  M_ARCH =
endif

CCFLAGS +=${M_ARCH}

#Makes it small AND FAST
CCFLAGS +=-O2

ifneq ($(COMPILE_OS),macosx)
  CCFLAGS +=-flto
endif

# warnings and speed/space tricks   
FLAGS += \
-P -ftrapv -funroll-loops -fno-strict-aliasing \
-fvisibility=hidden -fvisibility-inlines-hidden -finline-functions \
-Wextra -Wshadow -Wpointer-arith -Wcast-align \
-Wstrict-overflow=5 -Wsign-compare

ifneq ($(COMPILE_OS),windows)
  CCFLAGS +=-fPIC
endif


#common path includes
CCFLAGS += \
-Isrc/

UDT_OBJS = api.o buffer.o cache.o ccc.o channel.o udtCommon.o core.o epoll.o list.o md5.o packet.o queue.o window.o

$(UDT_OBJS): %.o: src/%.cpp src/%.h src/udt.h
	@$(CPP) $(CCFLAGS) $< -c

lib: $(UDT_OBJS)
	@$(CPP) $(CCFLAGS) $(LDFLAGS) -o $(DIST_NAME) $^ $(LIBS)

udt: lib

dist-clean: clean
	@rm -f *.dll *.so *.dylib  

clean:
	@rm -f *.o *.a

