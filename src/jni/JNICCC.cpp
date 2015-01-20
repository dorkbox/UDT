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

#include "../jni/JNICCC.h"

#include "../jni/JNICCCFactory.h"
#include "../jni/JNIHelpers.h"

JNIEnv* AttachToJVM(JavaVM* vm)
{
	JNIEnv* env = NULL;

	jint result = vm->GetEnv(reinterpret_cast<void**>(&env),JNI_VERSION_1_4);

	if(result == JNI_EDETACHED)
		vm->AttachCurrentThread(reinterpret_cast<void**>(&env),NULL);

	return env;
}

JNICCC::JNICCC(JNIEnv* env, jobject objCCC) :
				_objCCC( env->NewGlobalRef(objCCC)){

	env->GetJavaVM(&_javaVM);

	if(_objCCC != NULL){
		jclass cccClass = env->GetObjectClass(_objCCC);

		//We cannot statically store the methodid's as the objCCC
		//class maybe a subclass of CCC which means the method id's would differ
		_methodInit = env->GetMethodID(cccClass, "init", "()V" );
		_methodClose = env->GetMethodID(cccClass, "close", "()V" );
		_methodOnACK = env->GetMethodID(cccClass, "onACK", "(I)V" );
		_methodOnLoss = env->GetMethodID(cccClass, "onLoss", "([I)V" );
		_methodTimeout = env->GetMethodID(cccClass, "onTimeout", "()V" );
		_methodOnPktSent = NULL;
		_methodProcessCustomMsg = NULL;

		env->SetLongField(objCCC,udt_CCC_fld_nativeHandleID,(jlong)(intptr_t)this);
	}
}

JNICCC::~JNICCC() {

	JNIEnv* env = AttachToJVM(_javaVM);

	if(_objCCC)
		env->DeleteGlobalRef(_objCCC);
}

jobject JNICCC::getJavaCCC(){
	return _objCCC;
}

void JNICCC::init(){

	CCC::init();

	if(_objCCC){
		JNIEnv* env = AttachToJVM(_javaVM);
		env->CallVoidMethod(_objCCC, _methodInit);
	}
}

void JNICCC::close(){

	CCC::close();

	if(_objCCC){
		JNIEnv* env = AttachToJVM(_javaVM);
		env->CallVoidMethod(_objCCC, _methodClose);
	}
}

void JNICCC::onACK(const int32_t& ack){

	CCC::onACK(ack);

	if(_objCCC){
		JNIEnv* env = AttachToJVM(_javaVM);
		env->CallVoidMethod(_objCCC, _methodOnACK, ack);
	}
}

void JNICCC::onLoss(const int32_t* losslist, const int& size){
	//TODO: map to Java CCC class;
	CCC::onLoss(losslist,size);
}

void JNICCC::onTimeout(){

	CCC::onTimeout();

	if(_objCCC){
		JNIEnv* env = AttachToJVM(_javaVM);
		env->CallVoidMethod(_objCCC, _methodTimeout);
	}
}

void JNICCC::onPktSent(const CPacket* pkt){
	CCC::onPktSent(pkt);
}

void JNICCC::onPktReceived(const CPacket* pkt){
	CCC::onPktReceived(pkt);
}

void JNICCC::processCustomMsg(const CPacket* pkt){
	CCC::processCustomMsg(pkt);
}

void JNICCC::setACKTimer(const int& msINT){
	CCC::setACKTimer(msINT);
}

void JNICCC::setACKInterval(const int& pktINT){
	CCC::setACKInterval(pktINT);
}

void JNICCC::setRTO(const int& usRTO){
	CCC::setRTO(usRTO);
}

void JNICCC::setPacketSndPeriod(const double sndPeriod){
	m_dPktSndPeriod = sndPeriod;
}

void JNICCC::setCWndSize(const double cWndSize){
	m_dCWndSize = cWndSize;
}

const CPerfMon* JNICCC::getPerfInfo(){
	return CCC::getPerfInfo();
}



