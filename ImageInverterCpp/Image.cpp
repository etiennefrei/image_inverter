#include "Image.h"
#include <iostream>
using namespace std;

JNIEXPORT void JNICALL Java_Image_inverting(JNIEnv* env, jclass, jint width, jint height, jintArray pixels) {
	jboolean isCopy;
	jintArray pix = pixels;
	jsize len = 3*width*height;
	jint* a = env->GetIntArrayElements(pix, &isCopy);
	jint* b = a;
	for (int i = 0; i < len; i++) {
		*b = ~*b;
		b = b + 1;
	}
	if (isCopy == JNI_TRUE) {
		env->ReleaseIntArrayElements(pix, a, 0);
	}
	
}


static jint getFilterCoeff(jint* filter, jint size, jint i, jint j) {
	return filter[j * size + i];
}

static void getRGB(jint* pixels, int width, int u, int v, jint* rgb) {
	const jint idx = 3 * (v * width + u);

	*rgb = *(pixels + idx + 2);
	*(rgb+1) = *(pixels + idx + 1);
	*(rgb+2) = *(pixels + idx + 0);
}

static void setRGB(jint* pixels, int width, int u, int v, jint* rgb) {
	const jint idx = 3*(v*width + u);

	pixels[idx + 2] = *rgb;
	pixels[idx + 1] = *(rgb+1);
	pixels[idx + 0] = *(rgb+2);
}

static jint clamp(jint v) {
	return (v == (v & 0xFF)) ? v : (v < 0) ? 0 : 255;
}

JNIEXPORT void JNICALL Java_Image_filtering(JNIEnv* env, jclass obj, jint width, jint height, jintArray _filter, jintArray _pixelsIn, jintArray _pixelsOut) {
	jboolean filter_isCopy, pixelsIn_isCopy, pixelsOut_isCopy;

	jint* filter = env->GetIntArrayElements(_filter, &filter_isCopy);
	jint* pixelsIn = env->GetIntArrayElements(_pixelsIn, &pixelsIn_isCopy);
	jint* pixelsOut = env->GetIntArrayElements(_pixelsOut, &pixelsOut_isCopy);

	jsize _pixelsInLength = env->GetArrayLength(_pixelsIn);
	jsize _filterLength = env->GetArrayLength(_filter);


	const jint fsize = (jint)sqrt(_filterLength);
	const jint fsizeD2 = fsize / 2;

	constexpr jsize size = 3;
	jint rgb[size] = { 0,0,0 };
	

	for (jint v = 0; v < height; v++) {
		for (jint u = 0; u < width; u++) {
			jint sum[size] = { 0,0,0 };
			for (jint j = 0; j < fsize; j++) {
				jint v0 = v + j - fsizeD2;
				if (v0 < 0) {
					v0 = -v0;
				}
				else if (v0 >= height) {
					v0 = 2 * height - v0 - 1;
				}

				for (jint i = 0; i < fsize; i++) {
					const jint fc = getFilterCoeff(filter, fsize, i, j);
					jint u0 = u + i - fsizeD2;
					if (u0 < 0) {
						u0 = -u0;
					}
					else if (u0 >= width) {
						u0 = 2 * width - u0 - 1;

					}

					getRGB(pixelsIn, width, u0, v0, rgb);
					for (int c = 0; c < 3; c++) {
						sum[c] += rgb[c] * fc;
					}
				}
			}

			for (int c = 0; c < 3; c++) {
				jint temp = (jint)128 + sum[c];
				sum[c] = clamp(temp);
			}
			setRGB(pixelsOut, width, u, v, sum);
		}
	}

	if (filter_isCopy == JNI_TRUE) {
		env->ReleaseIntArrayElements(_filter, filter, 0);
	}

	if (pixelsIn_isCopy == JNI_TRUE) {
		env->ReleaseIntArrayElements(_pixelsIn, pixelsIn, 0);
	}

	if (pixelsOut_isCopy == JNI_TRUE) {
		env->ReleaseIntArrayElements(_pixelsOut, pixelsOut, 0);
	}
}
