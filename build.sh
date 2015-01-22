#!/bin/bash
#*******************************************************************************
#
# This will build ALL variants!
#
# This expects the following environment variables set:
#
# COMPILE_OS      - linux, macosx, windows
# COMPILE_OS_ARCH - 32, 64, arm
#
#
# possible packages you might need in order to compile:
#  gcc-4.8 gcc-4.8-multilib g++-4.8-multilib gdb make libgtk2.0-dev libxtst-dev libc6-dev-i386 mingw-w64
#  ccache libssl0.9.8
#
# Then, in gcc_apple_amd64 dir: sudo dpkg -i *.deb (this is 64bit gcc to build macosx binaries)
#
# Because we fake the 32bit/arm library calls during compile time (the libs are
# loaded during run time) we can build 32bit/arm builds on a 64bit system without needing multi-arch
# GTK libraries (fyi: there aren't any... and DO NOT install the i386 ones, since they will REMOVE what
# you already have - buggering your system!)
#
#
#
# TODO: clean up below, verify arm builds.
#  # arm:  gcc-arm-linux-gnueabi gcc-4.8-arm-linux-gnueabihf
# hf or no hf? beaglebone, rasbpi, ODROID-U3, other ARM devices
#
# ARMv7sf needs gcc-arm-linux-gnueabi, ARMv7hf needs gcc-arm-linux-gnueabihf
#
# To compile for the ARMv7 (BeagleBone Black), follow:
# http://www.michaelhleonard.com/cross-compile-for-beaglebone-black/
#
# for raspi/BBB
# http://www.michaelhleonard.com/cross-compile-for-beaglebone-black/
# http://derekmolloy.ie/beaglebone/setting-up-eclipse-on-the-beaglebone-for-c-development/
# http://stackoverflow.com/questions/9324772/cross-compiling-static-c-hello-world-for-android-using-arm-linux-gnueabi-gcc?rq=1
#
#
#for ANY OTHER OS/ARCH, the easiest, and most straight forward method is to use
# crosstool-ng. http://crosstool-ng.org/
# howto's:
# http://raspberrypi.stackexchange.com/questions/1/how-do-i-build-a-gcc-4-7-toolchain-for-cross-compiling
# http://www.bootc.net/archives/2012/05/26/how-to-build-a-cross-compiler-for-your-raspberry-pi/
# http://www.ps3devwiki.com/wiki/Cross_Compiling
#
#
######################################################################
# as per: http://stackoverflow.com/questions/8937492/what-is-upxs-best-compression-method
# it is NOT a good idea to compress exe's. WHY? Because they run slower, and HDD space is no longer a concern.
# especially since the executable is ~ 500k.
######################################################################
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

readonly true=1 yes=1 no=0 false=0


BUILD_ENV=`uname -s`-`uname -m`


if [[ "$BUILD_ENV" != "Linux-x86_64" ]]; then
  echo " ********"
  echo " ******** Can ONLY build on [[ $BUILD_ENV ]]"
  echo " ********"
  return false
fi

echo " ********"
echo " ******** Build Environment: [[ $BUILD_ENV ]]"
echo " ********"

export PROGRAM_DEPLOY=true

#############
#core function that will build our targets
#############
build() {
  BUILD_TYPE=$1
  export COMPILE_OS=$2
  export COMPILE_OS_ARCH=$3

  echo " ********"
  echo " ******** Building: ${COMPILE_OS}""_""${COMPILE_OS_ARCH}"
  echo " ********"

  # compile
  make -f $BUILD_TYPE all
}


####################################################################
####################################################################
####################################################################
####################################################################
# DONE WITH FUNCTIONS
####################################################################
####################################################################
####################################################################
BUILD_ALL=false

if [ -z "$1" ]; then
  BUILD_ALL=true
elif [[ "$1" == "all" ]]; then
  BUILD_ALL=true
fi


# No args will build all.
if [[ $BUILD_ALL == true ]]; then
  if [ -z "$1" ]; then
    echo " ******** Building linux (32/64), macosx (32/64), windows (32/64)"
    echo " ******** Press enter to continue... "
    read VARNAME
  fi

  # LINUX
  build "make_linux.mak" linux 32
  build "make_linux.mak" linux 64


  # WINDOWS
  build "make_windows.mak" windows 32
  build "make_windows.mak" windows 64


  # MAC
  build "make_macosx.mak" macosx 32
  build "make_macosx.mak" macosx 64
else
  # LINUX
  if [[ "$1" == "linux" ]]; then
      echo " ******** Building linux (32/64)"

      build "make_linux.mak" linux 32
      build "make_linux.mak" linux 64
  elif [[ "$1" == "linux_32" ]]; then
      build "make_linux.mak" linux 32
  elif [[ "$1" == "linux_64" ]]; then
      build "make_linux.mak" linux 64

  # WINDOWS
  elif [[ "$1" == "windows" ]]; then
      #all
      echo " ******** Building windows (32/64)"

      build "make_windows.mak" windows 32
      build "make_windows.mak" windows 64

  elif [[ "$1" == "windows_32" ]]; then
      build "make_windows.mak" windows 32
  elif [[ "$1" == "windows_64" ]]; then
      build "make_windows.mak" windows 64

  # MAC
  elif [[ "$1" == "macosx" ]]; then
      #all
      echo " ******** Building macosx (32/64)"

      build "make_macosx.mak" macosx 32
      build "make_macosx.mak" macosx 64
  elif [[ "$1" == "macosx_32" ]]; then
      build "make_macosx.mak" macosx 32
  elif [[ "$1" == "macosx_64" ]]; then
      build "make_macosx.mak" macosx 64

  else
    echo "Please one of the following:"
    echo "   [all] - builds all of them"
    echo ""
    echo "linux"
    echo "   linux [all]    - builds 32 and 64 bit linux"
    echo "   linux_32 [all] - builds 32 bit linux"
    echo "   linux_64 [all] - builds 64 bit linux"
    echo ""
    echo "mac"
    echo "   macosx [all]    - builds 32 and 64 bit mac"
    echo "   macosx_32 [all] - builds 32 bit mac"
    echo "   macosx_64 [all] - builds 64 bit mac"
    echo ""
    echo "windows"
    echo "   windows [all]    - builds 32 and 64 bit windows"
    echo "   windows_32 [all] - builds 32 bit windows"
    echo "   windows_64 [all] - builds 64 bit windows"
    exit 0
  fi
fi

