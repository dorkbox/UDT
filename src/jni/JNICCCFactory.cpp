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
#include "../jni/JNICCCFactory.h"

#include "../jni/JNICCC.h"
#include "../jni/JNIHelpers.h"

jmethodID 	JNICCCFactory::_udt_clsFactoryInterfaceUDT_create = NULL;
jmethodID 	JNICCCFactory::_udt_clsFactoryInterfaceUDT_cloneFactory = NULL;

bool JNICCCFactory::initJNITypes(JNIEnv* env){

	if(_udt_clsFactoryInterfaceUDT_create != NULL)
		return true;

	if(udt_FactoryInterfaceUDT==NULL)
		return false;

	_udt_clsFactoryInterfaceUDT_cloneFactory = env->GetMethodID(udt_FactoryInterfaceUDT, "cloneFactory", "()Lcom/barchart/udt/FactoryInterfaceUDT;");
	_udt_clsFactoryInterfaceUDT_create = env->GetMethodID(udt_FactoryInterfaceUDT, "create", "()Lcom/barchart/udt/CCC;");

	return true;
}

JNIEnv* JNICCCFactory::AttachToJVM()
{
	JNIEnv* env = NULL;

	jint result = _javaVM->GetEnv(reinterpret_cast<void**>(&env),JNI_VERSION_1_4);

	if(result == JNI_EDETACHED)
		_javaVM->AttachCurrentThread(reinterpret_cast<void**>(&env),NULL);

	return env;
}


JNICCCFactory::JNICCCFactory(JNIEnv* env, jobject factoryUDT) {

	initJNITypes(env);

	if(factoryUDT==NULL){
		;//TODO create new exception
	}

	env->GetJavaVM(&_javaVM);

	//We need to create a global JNI ref since
	//we are storing the jobject as a member variable
	//for use later on
	_factoryUDT = env->NewGlobalRef(factoryUDT);

}

JNICCCFactory::~JNICCCFactory(){

	JNIEnv* env = AttachToJVM();

	if(_factoryUDT != NULL){
		env->DeleteLocalRef(_factoryUDT);
	}

}

CCC* JNICCCFactory::create(){

	JNIEnv* env = AttachToJVM();

	jobject objCCC = env->CallObjectMethod(_factoryUDT,_udt_clsFactoryInterfaceUDT_create);

	if(objCCC==NULL && !env->ExceptionCheck() ){
		UDT_ThrowExceptionUDT_Message(env, 0, "failed to allocate CCC class via com.barchart.udt.FactoryInterfaceUDT");
	}

	return reinterpret_cast<CCC*> ((intptr_t) env->GetLongField(objCCC,udt_CCC_fld_nativeHandleID));
}

CCCVirtualFactory* JNICCCFactory::clone(){

	JNIEnv* env = AttachToJVM();

	jobject objFactoryUDT = env->CallObjectMethod(_factoryUDT,_udt_clsFactoryInterfaceUDT_cloneFactory);

	return new JNICCCFactory(env,objFactoryUDT);

}

