#include <jni.h>
#include <string>
#include <iostream>

#include "u.h"
#include "net.h"

#define jcstr(x) (env->GetStringUTFChars(x, nullptr))
#define free_jcstr(str, chrs) (env->ReleaseStringUTFChars(str, chrs))

extern "C" JNIEXPORT jstring JNICALL
Java_bleed_conic_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jobject JNICALL
Java_bleed_conic_Http_get_1(
        JNIEnv *env, jobject self, jstring addr_, jstring path_, jstring query_
        ) {
    constexpr const char *TAG = "Http.get";

    try {
        auto addr = jcstr(addr_), path = jcstr(path_), query = jcstr(query_);
        log_info(TAG, "addr: %s, path: %s, query: %s", addr, path, query);
        auto http = Net::Http(addr);

        auto r = http.get(path, query);
        if (!r) {
            log_err(TAG, "%s", r.err.c_str());

        }
        free_jcstr(addr_, addr);free_jcstr(path_, path);free_jcstr(query_, query);
    } catch (std::string &e) {
        log_err(TAG, "%s", e.c_str());
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_bleed_conic_UKt_VERSION(JNIEnv *env, jclass clazz) {
    return env->NewStringUTF(U::VERSION);
}