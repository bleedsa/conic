#include <jni.h>
#include <string>

#include "net.h"

#define jcstr(x) (env->GetStringUTFChars(x, nullptr))
#define free_jcstr(str, chrs) (env->ReleaseStringUTFChars(str, chrs))

extern "C" JNIEXPORT jstring JNICALL
Java_bleed_conic_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_bleed_conic_Http_get_1(
        JNIEnv *env, jobject self, jstring addr_, jstring path_, jstring query_
        ) {
    auto addr = jcstr(addr_), path = jcstr(path_), query = jcstr(query_);
    auto http = Net::Http(addr);
    auto r = http.get(path, query);

    free_jcstr(addr_, addr);free_jcstr(path_, path);free_jcstr(query_, query);
}