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

#include "../jni/com_barchart_udt_CCC.h"

#include "../jni/JNICCC.h"
#include "../jni/JNIHelpers.h"

extern "C" {

JNICCC* getNativeJNICCC(JNIEnv* env, jobject javaCCC){

    CHK_NUL_RET_NUL(env, "env");
    CHK_NUL_RET_NUL(env, "javaCCC");

    JNICCC* jniCCC = reinterpret_cast<JNICCC*>( (intptr_t) env->GetLongField(javaCCC,udt_CCC_fld_nativeHandleID)) ;

    if(jniCCC==NULL){
        UDT_ThrowExceptionUDT_Message(env,0,"illegal state, nativeHandle not set to instance of native JNICCC* class");
        return NULL;
    }

    return jniCCC;
}

/*
 * Class:     com_barchart_udt_CCC
 * Method:    initNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_barchart_udt_CCC_initNative (JNIEnv *env, jobject self){
    JNICCC* jniCCC = new JNICCC(env, self);

    if(jniCCC==NULL)
        UDT_ThrowExceptionUDT_Message(env,0,"failed to allocate native JNICCC* class");

    return;
}


/*
 * Class:     com_barchart_udt_CCC
 * Method:    setACKTimer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_barchart_udt_CCC_setACKTimer
  (JNIEnv * env, jobject obj, jint msINT){

    JNICCC* jniCCC = getNativeJNICCC(env,obj);

    if(jniCCC==NULL){
        return;
    }

    jniCCC->setACKTimer(msINT);

}


/*
 * Class:     com_barchart_udt_CCC
 * Method:    setACKInterval
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_barchart_udt_CCC_setACKInterval
  (JNIEnv * env, jobject obj, jint pktINT){

    JNICCC* jniCCC = getNativeJNICCC(env,obj);

    if(jniCCC==NULL)
        return;

    jniCCC->setACKInterval(pktINT);

}


/*
 * Class:     com_barchart_udt_CCC
 * Method:    setRTO
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_barchart_udt_CCC_setRTO
  (JNIEnv * env, jobject obj, jint usRTO){

    JNICCC* jniCCC = getNativeJNICCC(env,obj);

    if(jniCCC==NULL)
        return;

    jniCCC->setRTO(usRTO);

}


/*
 * Class:     com_barchart_udt_CCC
 * Method:    setPacketSndPeriod
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_com_barchart_udt_CCC_setPacketSndPeriod
  (JNIEnv *env, jobject obj, jdouble sndPeriod){

    JNICCC* jniCCC = getNativeJNICCC(env,obj);

    if(jniCCC==NULL)
        return;

    jniCCC->setPacketSndPeriod(sndPeriod);

}

/*
 * Class:     com_barchart_udt_CCC
 * Method:    setCWndSize
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_com_barchart_udt_CCC_setCWndSize
  (JNIEnv *env, jobject obj, jdouble cWndSize){

    JNICCC* jniCCC = getNativeJNICCC(env,obj);

    if(jniCCC==NULL)
        return;

    jniCCC->setCWndSize(cWndSize);

}


/*
 * Class:     com_barchart_udt_CCC
 * Method:    getPerfInfo
 * Signature: ()Lcom/barchart/udt/MonitorUDT;
 */
JNIEXPORT jobject JNICALL Java_com_barchart_udt_CCC_getPerfInfo
  (JNIEnv *env, jobject obj){

    JNICCC* jniCCC = getNativeJNICCC(env,obj);

    const CPerfMon* perfMon = jniCCC->getPerfInfo();

    UNUSED(perfMon);

    UDT_ThrowExceptionUDT_Message(env,0,"TODO: implement CPerfMon -> MonitorUDT mapping");

    return NULL;

}

} // __cplusplus defined.
