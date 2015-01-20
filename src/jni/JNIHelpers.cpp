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

#include "../jni/JNIHelpers.h"

#include "jni.h"

// ### JNI

#define JNI_UPDATE 0 // jni object release with copy
//

// ### JDK references

// JDK classes

jclass jdk_Boolean; // java.lang.Boolean
jclass jdk_Integer; // java.lang.Integer
jclass jdk_Long; // java.lang.Long

jclass jdk_InetAddress; // java.net.InetAddress
jclass jdk_InetSocketAddress; // java.net.InetSocketAddress

jclass jdk_SocketException; // java.net.SocketException

jclass jdk_Set; // java.util.Set
jclass jdk_Iterator; // java.util.Iterator

// JDK methods

jmethodID jdk_Boolean_init; // new Boolean(boolean x)
jmethodID jdk_Integer_init; // new Integer(int x)
jmethodID jdk_Long_init; // new Long(long x)

jmethodID jdk_InetAddress_getAddress; // byte[] getAddress()
jmethodID jdk_InetAddress_getByAddress; // static InetAddress getByAddress(byte[])

jmethodID jdk_InetSocketAddress_init; // new InetSocketAddress(InetAddress x)
jmethodID jdk_InetSocketAddress_getAddress; //
jmethodID jdk_InetSocketAddress_getPort; //

jmethodID jdk_Set_iterator; // Iterator set.iterator()
jmethodID jdk_Set_add; // boolean set.add(Object)
jmethodID jdk_Set_contains; // boolean set.contains(Object)

jmethodID jdk_Iterator_hasNext; // boolean iterator.hasNext()
jmethodID jdk_Iterator_next; // Object iterator.next()

// UDT classes

jclass udt_FactoryInterfaceUDT; //com.barchart.udt.FactoryInterfaceUDT

// UDT methods

jfieldID udt_CCC_fld_nativeHandleID;

void X_InitClassReference(JNIEnv *env, jclass *classReference,
		const char *className) {
	CHK_NUL_RET_RET(env, "env");
	CHK_NUL_RET_RET(className, "className");
	CHK_NUL_RET_RET(classReference, "classReference");
	jclass klaz = env->FindClass(className);
	CHK_NUL_RET_RET(klaz, "klaz");
	*classReference = static_cast<jclass>(env->NewGlobalRef((jobject) klaz));
	CHK_NUL_RET_RET(*classReference, "*classReference");
}

void X_FreeClassReference(JNIEnv *env, jclass* globalRef) {
	CHK_NUL_RET_RET(env, "env");
	CHK_NUL_RET_RET(globalRef, "globalRef");
	env->DeleteGlobalRef(*globalRef);
	*globalRef = NULL;
}

// use native bool parameter
jobject X_NewBoolean(JNIEnv *env, bool value) {
	CHK_NUL_RET_NUL(env, "env");
	return env->NewObject(jdk_Boolean, jdk_Boolean_init,
			BOOLEAN(value) );
}

// use native 32 bit int parameter
jobject X_NewInteger(JNIEnv *env, int value) {
	CHK_NUL_RET_NUL(env, "env");
	return env->NewObject(jdk_Integer, jdk_Integer_init, (jint) value);
}

// use native 64 bit long parameter
jobject X_NewLong(JNIEnv* env, int64_t value) {
	CHK_NUL_RET_NUL(env, "env");
	return env->NewObject(jdk_Long, jdk_Long_init, (jlong) value);
}

// NOTE: ipv4 only
int X_InitSockAddr(sockaddr* sockAddr) {
	CHK_NUL_RET_ERR(sockAddr, "sockAddr");
	sockaddr_in* sockAddrIn = (sockaddr_in*) sockAddr;
	sockAddrIn->sin_family = AF_INET;
	memset(&(sockAddrIn->sin_zero), '\0', 8);
	return JNI_OK;
}

// NOTE: ipv4 only
int X_ConvertInetAddressToInteger( //
		JNIEnv* env, //
		jobject objInetAddress, //
		jint* address //
		) {

	CHK_NUL_RET_ERR(env, "env");
	CHK_NUL_RET_ERR(objInetAddress, "objInetAddress");
	CHK_NUL_RET_ERR(address, "address");

	const jbyteArray objArray = (jbyteArray) env->CallObjectMethod(
			objInetAddress, jdk_InetAddress_getAddress);
	CHK_NUL_RET_ERR(objArray, "objArray");

	const int sizeArray = env->GetArrayLength(objArray);

	switch (sizeArray) {
	case 4:
		// IPV4
		break;
	case 16:
		// IPV6
	default:
		CHK_LOG("unsupported address size", "sizeArray");
		return JNI_ERR;
	}

	jbyte* refArray = env->GetByteArrayElements(objArray, NULL);

	int value = 0;
	value |= (0xFF000000 & (refArray[0] << 24));
	value |= (0x00FF0000 & (refArray[1] << 16));
	value |= (0x0000FF00 & (refArray[2] << 8));
	value |= (0x000000FF & (refArray[3]));

	env->ReleaseByteArrayElements(objArray, refArray, 0);

	*address = value;

	return JNI_OK;

}

// NOTE: ipv4 only
int X_ConvertInetSocketAddressToSockaddr( //
		JNIEnv* env, //
		jobject objInetSocketAddress, //
		sockaddr* sockAddr //
		) {

	CHK_NUL_RET_ERR(env, "env");
	CHK_NUL_RET_ERR(sockAddr, "sockAddr");
	CHK_NUL_RET_ERR(objInetSocketAddress, "objInetSocketAddress");

	const jobject objInetAddress = env->CallObjectMethod(objInetSocketAddress,
			jdk_InetSocketAddress_getAddress);
	CHK_NUL_RET_ERR(objInetAddress, "objInetAddress");

	jint address = 0;
	jint port = env->CallIntMethod(objInetSocketAddress,
			jdk_InetSocketAddress_getPort);

	const int rv = X_ConvertInetAddressToInteger(env, objInetAddress, &address);
	if (rv == JNI_ERR) {
		return JNI_ERR;
	}

	sockaddr_in* sockAddrIn = (sockaddr_in*) sockAddr;

	sockAddrIn->sin_addr.s_addr = htonl(address);
	sockAddrIn->sin_port = htons(port);

	return JNI_OK;

}

// NOTE: only ipv4
jobject X_NewInetAddress( //
		JNIEnv* env, //
		const jint address //
		) {

	CHK_NUL_RET_NUL(env, "env");

	char valArray[4];
	valArray[0] = (address & 0xFF000000) >> 24;
	valArray[1] = (address & 0x00FF0000) >> 16;
	valArray[2] = (address & 0x0000FF00) >> 8;
	valArray[3] = (address & 0x000000FF);

	const jbyteArray objArray = env->NewByteArray(4);

	env->SetByteArrayRegion(objArray, (jint) 0, (jint) 4, (jbyte*) valArray);

	const jobject objInetAddress = env->CallStaticObjectMethod(
			jdk_InetAddress, jdk_InetAddress_getByAddress, objArray);
	CHK_NUL_RET_NUL(objInetAddress, "objInetAddress");

	return objInetAddress;

}

// NOTE: ipv4 only
jobject X_NewInetSocketAddress( //
		JNIEnv* env, //
		sockaddr* sockAddr //
		) {

	CHK_NUL_RET_NUL(env, "env");
	CHK_NUL_RET_NUL(sockAddr, "sockAddr");

	sockaddr_in* sockAddrIn = (sockaddr_in*) sockAddr;
	const jint address = ntohl(sockAddrIn->sin_addr.s_addr);
	const jint port = ntohs(sockAddrIn->sin_port);

	const jobject objInetAddress = X_NewInetAddress(env, address);
	CHK_NUL_RET_NUL(objInetAddress, "objInetAddress");

	const jobject objInetSocketAddress = env->NewObject(
			jdk_InetSocketAddress, jdk_InetSocketAddress_init,
			objInetAddress, port);
	CHK_NUL_RET_NUL(objInetSocketAddress, "objInetSocketAddress");

	return objInetSocketAddress;

}

bool X_IsSockaddrEqualsInetSocketAddress( //
		JNIEnv* env, //
		sockaddr* sockAddr, //
		jobject objSocketAddress //
		) {

	CHK_NUL_RET_FLS(env, "env");
	CHK_NUL_RET_FLS(sockAddr, "sockAddr");
	CHK_NUL_RET_FLS(objSocketAddress, "objSocketAddress");

	sockaddr_in* sockAddrIn = (sockaddr_in*) sockAddr;

	jint address1 = ntohl(sockAddrIn->sin_addr.s_addr);
	jint port1 = ntohs(sockAddrIn->sin_port);

	jint address2 = 0;
	jint port2 = env->CallIntMethod(objSocketAddress,
			jdk_InetSocketAddress_getPort);

	jobject objInetAddress = env->CallObjectMethod(objSocketAddress,
			jdk_InetSocketAddress_getAddress);
	CHK_NUL_RET_ERR(objInetAddress, "objInetAddress");

	const int rv = X_ConvertInetAddressToInteger(env, objInetAddress,
			&address2);
	if (rv == JNI_ERR) {
		return false;
	}

	if (address1 == address2 && port1 == port2) {
		return true;
	}

	return false;

}
