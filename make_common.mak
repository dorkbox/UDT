################################################################################
# Makefile for creating the app launcher program.
#
# This + parent makefiles expect the following environment variables set:
#
# COMPILE_OS      - linux, macosx, windows
# COMPILE_OS_ARCH - 32, 64, arm
#
################################################################################

# Note that we are ONLY updating the JNI binaries, NOT THE JAVA CODE. So you will want to replace the appropriate files
#      inside the bundle jar.
CORE_NAME=libbarchart-udt-core-2.3.1.1
TARGET_PATH=../../resources/Dependencies/barchart-udt
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
-isystem$(JVM)/include \
-isystem$(JVM)/include/$(COMPILE_OS) \
-Isrc/

JNI_OBJS = com_barchart_udt_CCC.o com_barchart_udt_SocketUDT.o JNICCC.o JNICCCFactory.o JNIHelpers.o
UDT_OBJS = api.o buffer.o cache.o ccc.o channel.o udtCommon.o core.o epoll.o list.o md5.o packet.o queue.o window.o

$(JNI_OBJS): %.o: src/jni/%.cpp src/jni/%.h
	@$(CPP) $(CCFLAGS) $< -c

$(UDT_OBJS): %.o: src/%.cpp src/%.h src/udt.h
	@$(CPP) $(CCFLAGS) $< -c

lib: $(UDT_OBJS) $(JNI_OBJS)
	@$(CPP) $(CCFLAGS) $(LDFLAGS) -o $(DIST_NAME) $^ $(LIBS)

udt: lib

dist-clean: clean
	@rm -f *.dll *.so *.dylib  

clean:
	@rm -f *.o *.a

