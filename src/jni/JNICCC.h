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

#ifndef JNICCC_H_
#define JNICCC_H_

#include "ccc.h"
#include <jni.h>

class JNICCCFactory;

class JNICCC: public CCC {

	friend JNIEXPORT void JNICALL Java_com_barchart_udt_CCC_initNative (JNIEnv *env, jobject self);

private:

	jobject _objCCC;
	JavaVM* _javaVM;

	jmethodID _methodInit;
	jmethodID _methodClose;
	jmethodID _methodOnACK;
	jmethodID _methodOnLoss;
	jmethodID _methodTimeout;
	jmethodID _methodOnPktSent;
	jmethodID _methodProcessCustomMsg;

public:

	virtual ~JNICCC();

	jobject getJavaCCC();

	//we need to override the visibility
	//of the setXXX calls since we are
	//emulating class inheritance from Java
	void setACKTimer(const int& msINT);

	void setACKInterval(const int& pktINT);

	void setRTO(const int& usRTO);

	void setPacketSndPeriod(const double sndPeriod);

	void setCWndSize(const double cWndSize);

	const CPerfMon* getPerfInfo();

private:

	//we don't want to allow any overrides on this
	//class since it will be emulated in the Java layer

	JNICCC(JNIEnv* env, jobject objCCC);

	void init();

	void close();

	void onACK(const int32_t&);

	void onLoss(const int32_t*, const int&);

	void onTimeout();

	void onPktSent(const CPacket*);

	void onPktReceived(const CPacket*);

	void processCustomMsg(const CPacket*);

};

#endif /* JNICCC_H_ */
