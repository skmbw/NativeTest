// NativeTest.cpp : Defines the exported functions for the DLL application.
//
#pragma execution_character_set("utf-8")
#include "NativeTest.h"
#include <iostream>
#include <opencv2\opencv.hpp>
#include <vector>


using namespace std;
using namespace cv;


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
	// char* 或者char[]数组 可以直接转换成c++ string类
	string str = cs;
	printf("C++ OUTPUT %s \n", cs);
	printf("Java_com_vteba_test_NativeTest_testByteString\r\n");
	// c++ string 类，可以通过函数c_str()转成char*
	const char* cstr = str.c_str();

	// 转换成jstring，也就是java的string
	//jstring jsresult = env->NewStringUTF(cstr);

	// char* 也可以直接强转成jbyte* 因为char是8位int不会越界
	jbyte* middleByteArray = (jbyte*)cstr;

	// UTF8字符的长度其实就是char*的长度，也就是jbyte*的长度
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
	// 获得数组长度
	jsize theArrayLeng = env->GetArrayLength(jbyteArrayInput);
	cout << "the length of input array=[" << theArrayLeng << endl;
	// 获得数组元素，这个是一个指针，后面需要释放
	jbyte *jbyteArrayData = env->GetByteArrayElements(jbyteArrayInput, JNI_FALSE);
	vector<char> imageData(jbyteArrayData, &jbyteArrayData[theArrayLeng]);
	// 和上面直接赋值等价
	//for (auto i = 0; i != theArrayLeng; i++) {
	//	imageData.push_back(jbyteArrayData[i]);
	//}
	cout << "the length of imageData=[" << theArrayLeng << endl;
	// 可以直接强转为char*数组（这个就是C++的字节数组）
	//const char *jbyteArrayString = (const char *)jbyteArrayData;
	//cout << "C++将 [*jbyteArrayData 转 const char *]SUCCESS" << endl;
	//string strTemp = jbyteArrayString;
	//cout << "C++将 [const char * 转string]SUCCESS " << strTemp.length() << endl;
	
	//std::vector<char> data(strTemp.begin(), strTemp.end());
	Mat img = imdecode(imageData, cv::IMREAD_GRAYSCALE);
	bool r = imwrite("E:\\images\\zjt2.jpg", img);
	printf("cv imwrite result %d\n", r);

	jbyteArray resultBytes = env->NewByteArray(theArrayLeng);

	env->SetByteArrayRegion(resultBytes, 0, theArrayLeng, jbyteArrayData);

	/*for (long i = 0; i < theArrayLeng; i++) {
		resultBytes[i] = (jbyte)jbyteArrayString[i];
	}*/

	env->ReleaseByteArrayElements(jbyteArrayInput, jbyteArrayData, 0);
	// 常量中有换行符（解决办法）
	//（1）全部用英文编码，不要用中文
	//（2）偶数中文 或 结尾加英文的符号，如"."
	//（3）将文件编码进行一个手动（如记事本）转换，改成UTF - 8格式
	string ret = "wojiao125#$a尹雷啊aa.";
	const char *c_ret = ret.c_str();
	const jbyte *j_ret = (const jbyte *)c_ret;
	auto len = ret.length();
	jbyteArray ret_jbyteArray = env->NewByteArray(len);
	env->SetByteArrayRegion(ret_jbyteArray, 0, len, j_ret);

	return ret_jbyteArray;
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
* 将char*转成jstring
* @param env jni环境
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

//将jstring类型转换成char*
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
		rtn[alen] = 0; // 多一位赋值0，是结束标志吗
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	
	return rtn;
}

