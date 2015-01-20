/*
 * jni_md.h header file
 */

#ifndef _JAVASOFT_JNI_MD_H_
#define _JAVASOFT_JNI_MD_H_
  
#define JNIEXPORT __declspec(dllexport)
#define JNIIMPORT __declspec(dllimport)
#define JNICALL __stdcall
  
/* stuff and such */
typedef long jint;
typedef __int64 jlong;
typedef signed char jbyte;
  
#endif
