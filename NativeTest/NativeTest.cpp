// NativeTest.cpp : Defines the exported functions for the DLL application.
//

#include "NativeTest.h"
#include <iostream>

using namespace std;

/*
* Class:     com_vteba_test_NativeTest
* Method:    testString
* Signature: (Ljava/lang/String;)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_com_vteba_test_NativeTest_testString(JNIEnv *env, jclass jc, jstring js) {
	printf("Java_com_vteba_test_NativeTest_testString\r\n");
	return js;
}

/*
* Class:     com_vteba_test_NativeTest
* Method:    testByteString
* Signature: (Ljava/lang/String;)[B
*/
JNIEXPORT jbyteArray JNICALL Java_com_vteba_test_NativeTest_testByteString(JNIEnv *env, jclass jc, jstring js) {
	const char * cs = env->GetStringUTFChars(js, JNI_FALSE);
	// char* ����char[]���� ����ֱ��ת����c++ string��
	string str = cs;
	printf("C++ OUTPUT %s \n", cs);
	printf("Java_com_vteba_test_NativeTest_testByteString\r\n");
	// c++ string �࣬����ͨ������c_str()ת��char*
	const char* cstr = str.c_str();
	// jstring jsresult = env->NewStringUTF(cstr);

	// char* Ҳ����ֱ��ǿת��jbyte* ��Ϊchar��8λint����Խ��
	jbyte* middleByteArray = (jbyte*)cstr;

	// UTF8�ַ��ĳ�����ʵ����char*�ĳ��ȣ�Ҳ����jbyte*�ĳ���
	jsize jlen = env->GetStringUTFLength(js);
	
	jbyteArray jbArrayResult = env->NewByteArray(jlen);
	env->SetByteArrayRegion(jbArrayResult, 0, jlen, middleByteArray);
	
	env->ReleaseStringUTFChars(js, cs);
	

	return jbArrayResult;
}

/*
* Class:     com_vteba_test_NativeTest
* Method:    testBytes
* Signature: ([B)[B
*/
JNIEXPORT jbyteArray JNICALL Java_com_vteba_test_NativeTest_testBytes(JNIEnv *env, jclass jc, jbyteArray jbyteArrayInput) {
	printf("Java_com_vteba_test_NativeTest_testBytes\n");

	jsize theArrayLeng = env->GetArrayLength(jbyteArrayInput);

	jbyte * jbyteArrayData = env->GetByteArrayElements(jbyteArrayInput, JNI_FALSE);

	char * jbyteArrayString = (char *)jbyteArrayData;
	printf("C++ OUTPUT %s\n", jbyteArrayString);

	env->ReleaseByteArrayElements(jbyteArrayInput, jbyteArrayData, 0);

	return jbyteArrayInput;
}

/*
* Class:     com_vteba_test_NativeTest
* Method:    testInteger
* Signature: (I)I
*/
JNIEXPORT jint JNICALL Java_com_vteba_test_NativeTest_testInteger(JNIEnv *env, jclass jc, jint ji) {
	printf("Java_com_vteba_test_NativeTest_testInteger\n");
	return ji;
}

/*
* ��char*ת��jstring
* @param env jni����
* @param 
*/
jstring charToJstring(JNIEnv* env, const char* pat) {
	jclass strClass = env->FindClass("Ljava/lang/String;");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
	jstring encoding = env->NewStringUTF("utf-8");
	return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

//��jstring����ת����char*
char* jstringToChar(JNIEnv* env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0; // ��һλ��ֵ0���ǽ�����־��
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	
	return rtn;
}
