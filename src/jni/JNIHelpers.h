/**
 * =================================================================================
 *
 * BSD LICENCE (http://en.wikipedia.org/wiki/BSD_licenses)
 *
 * Copyright (C) 2009-2013, Barchart, Inc. (http://www.barchart.com/)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *     * Neither the name of the Barchart, Inc. nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Developers: Andrei Pozolotin, CCob
 *
 * =================================================================================
 */

#ifndef JNIHELPERS_H_
#define JNIHELPERS_H_

#include "udt.h"

#include <cassert>
#include <climits>
#include <cstring>
#include <vector>
#include <algorithm>

#include "jni.h"

// ### JNI

#define JNI_UPDATE 0 // jni object release with copy
//

extern "C"{

// ### JDK references

// JDK classes

extern jclass jdk_Boolean; // java.lang.Boolean
extern jclass jdk_Integer; // java.lang.Integer
extern jclass jdk_Long; // java.lang.Long

extern jclass jdk_InetAddress; // java.net.InetAddress
extern jclass jdk_InetSocketAddress; // java.net.InetSocketAddress

extern jclass jdk_SocketException; // java.net.SocketException
extern jclass jdk_Set; // java.util.Set
extern jclass jdk_Iterator; // java.util.Iterator

// JDK methods

extern jmethodID jdk_Boolean_init; // new Boolean(boolean x)
extern jmethodID jdk_Integer_init; // new Integer(int x)
extern jmethodID jdk_Long_init; // new Long(long x)

extern jmethodID jdk_InetAddress_getAddress; // byte[] getAddress()
extern jmethodID jdk_InetAddress_getByAddress; // static InetAddress getByAddress(byte[])

extern jmethodID jdk_InetSocketAddress_init; // new InetSocketAddress(InetAddress x)
extern jmethodID jdk_InetSocketAddress_getAddress; // InetAddress InetSocketAddress.getAddress()
extern jmethodID jdk_InetSocketAddress_getPort; // int InetSocketAddress.getPort()

extern jmethodID jdk_Set_iterator; // Iterator Set.iterator()
extern jmethodID jdk_Set_add; // boolean Set.add(Object)
extern jmethodID jdk_Set_contains; // boolean Set.contains(Object)

extern jmethodID jdk_Iterator_hasNext; // boolean Iterator.hasNext()
extern jmethodID jdk_Iterator_next; // Object Iterator.next()

// UDT classes

extern jclass udt_FactoryInterfaceUDT;

// UDT methods

extern jfieldID udt_CCC_fld_nativeHandleID;

//#define EOL "\n"

// special UDT method return value
#define UDT_TIMEOUT 0

// TODO make more portable mingw / msvc
#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

//
// null pointer safety
//
#define CHK_LOG(title,comment) printf ("%s function: %s comment: %s", title, __func__, comment);
//
#define CHK_NUL_RET_RET(reference,comment) if ((reference) == NULL) \
	{ CHK_LOG("CHK_NUL_RET_RET;",comment); return; }
//
#define CHK_NUL_RET_NUL(reference,comment) if ((reference) == NULL) \
	{ CHK_LOG("CHK_NUL_RET_NUL;",comment); return NULL; }
//
#define CHK_NUL_RET_FLS(reference,comment) if ((reference) == NULL) \
	{ CHK_LOG("CHK_NUL_RET_FLS;",comment); return false; }
//
#define CHK_NUL_RET_ERR(reference,comment) if ((reference) == NULL) \
	{ CHK_LOG("CHK_NUL_RET_ERR;",comment); return JNI_ERR; }
//
// c++ <-> java, bool <-> boolean conversions
#define BOOL(x) (((x) == JNI_TRUE) ? true : false) // from java to c++
#define BOOLEAN(x) ((jboolean) ( ((x) == true) ? JNI_TRUE : JNI_FALSE )) // from c++ to java
//
// free/malloc convenience
#define FREE(x) if ((x) != NULL) { free(x); x = NULL; }
#define MALLOC(var,type,size) type* var = (type*) malloc(sizeof(type) * size);

// declare unused explicitly or else see compiler warnings
#define UNUSED(x) ((void)(x))

void X_InitClassReference(JNIEnv *env, jclass *classReference,
		const char *className);

void X_FreeClassReference(JNIEnv *env, jclass* globalRef);

jobject X_NewBoolean(JNIEnv *env, bool value);

jobject X_NewInteger(JNIEnv *env, int value);

jobject X_NewLong(JNIEnv* env, int64_t value);

int X_InitSockAddr(sockaddr* sockAddr);

inline bool X_IsInRange(jlong min, jlong var, jlong max) {
	if (min <= var && var <= max) {
		return true;
	} else {
		return false;
	}
}

inline void X_ConvertMillisIntoTimeValue(const jlong millisTimeout,
		timeval* timeValue) {
	if (millisTimeout < 0) { // infinite wait
		timeValue->tv_sec = INT_MAX;
		timeValue->tv_usec = 0;
	} else if (millisTimeout > 0) { // finite wait
		timeValue->tv_sec = millisTimeout / 1000; // msvc C4244
		timeValue->tv_usec = (millisTimeout % 1000) * 1000;
	} else { // immediate return (not less the UDT event slice of 10 ms)
		timeValue->tv_sec = 0;
		timeValue->tv_usec = 0;
	}
}

// NOTE: ipv4 only
int X_ConvertInetSocketAddressToSockaddr(JNIEnv* env,
		jobject objInetSocketAddress, sockaddr* sockAddr);

// NOTE: only ipv4
jobject X_NewInetAddress(JNIEnv* env, jint address);

// NOTE: ipv4 only
jobject X_NewInetSocketAddress(JNIEnv* env, sockaddr* sockAddr);

// NOTE: ipv4 only
bool X_IsSockaddrEqualsInetSocketAddress(JNIEnv* env, sockaddr* sockAddr,
		jobject socketAddress);


void UDT_ThrowExceptionUDT_Message(JNIEnv* env, const jint socketID,
		const char *comment);

}//extern "C"




#endif /* JNIHELPERS_H_ */
