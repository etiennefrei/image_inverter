/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class Image */

#ifndef _Included_Image
#define _Included_Image
#ifdef __cplusplus
extern "C" {
#endif
	/*
	 * Class:     Image
	 * Method:    inverting
	 * Signature: (II[I)V
	 */
	JNIEXPORT void JNICALL Java_Image_inverting
	(JNIEnv*, jclass, jint, jint, jintArray);

	/*
	 * Class:     Image
	 * Method:    filtering
	 * Signature: (II[I[I[I)V
	 */
	JNIEXPORT void JNICALL Java_Image_filtering
	(JNIEnv*, jclass, jint, jint, jintArray, jintArray, jintArray);

	static jint getFilterCoeff(jint* filter, jint size, jint i, jint j);
	static void getRGB(jint* pixels, int width, int u, int v, jint* rgb);
	static void setRGB(jint* pixels, int width, int u, int v, jint* rgb);
	static jint clamp(jint v);

#ifdef __cplusplus
}
#endif
#endif

