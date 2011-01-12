/* 
 * BNDM.cpp
 * 
 * Created on 08.12.2003.
 *
 * eaio: StringSearch - high-performance pattern matching algorithms in Java
 * Copyright (c) 2003, 2004 Johann Burkard (jb@eaio.com) http://eaio.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#include <jni.h>
#include "com_eaio_stringsearch_BNDM.h"

#define INDEX(idx) (idx < 0) ? 256 + idx : idx

static inline int bndmSearchBytes(char* text, int textStart, int textEnd, char* pattern, int patternLength, int* b) {
    
    int d, j, pos, last;
    pos = textStart;
    while (pos <= textEnd - patternLength) {
        j = patternLength - 1;
        last = patternLength;
        d = -1;
        while (d != 0) {
            d &= b[INDEX(text[pos + j])];
            if (d != 0) {
                if (j == 0) {
                    return pos;
                }
                last = j;
            }
            --j;
            d <<= 1;
        }
        pos += last;
    }
    
    return -1;
    
}

/*
 * Class:     com_eaio_stringsearch_BNDM
 * Method:    nativeSearchBytes
 * Signature: ([BII[BLjava/lang/Object;)I
 */
JNIEXPORT jint JNICALL Java_com_eaio_stringsearch_BNDM_nativeSearchBytes
(JNIEnv *env, jobject obj, jbyteArray t, jint textStart, jint textEnd, jbyteArray p, jobject o) {

    int* d = (int*) env->GetPrimitiveArrayCritical((jintArray) o, JNI_FALSE);
    
    if (d == NULL) {
        return JNI_ENOMEM;
    }
    
    char* text = (char*) env->GetPrimitiveArrayCritical(t, JNI_FALSE);
    
    if (text == NULL) {
        env->ReleasePrimitiveArrayCritical((jintArray) o, d, JNI_ABORT);
        return JNI_ENOMEM;
    }
    
    char* pattern = (char*) env->GetPrimitiveArrayCritical(p, JNI_FALSE);
    
    if (pattern == NULL) {
        env->ReleasePrimitiveArrayCritical(t, text, JNI_ABORT);
        env->ReleasePrimitiveArrayCritical((jintArray) o, d, JNI_ABORT);
        return JNI_ENOMEM;
    }
    
    jint patternLength = env->GetArrayLength(p);
    jint textLength = env->GetArrayLength(t);

    textStart = (textStart < textLength ? textStart : textLength);
    textEnd = (textEnd < textLength ? textEnd : textLength);
    
    int pos = bndmSearchBytes(text, textStart, textEnd, pattern, patternLength, d);
    
    env->ReleasePrimitiveArrayCritical(p, pattern, JNI_ABORT);
    env->ReleasePrimitiveArrayCritical(t, text, JNI_ABORT);
    env->ReleasePrimitiveArrayCritical((jintArray) o, d, JNI_ABORT);
    
    return pos;

}
