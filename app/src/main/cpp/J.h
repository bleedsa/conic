#ifndef CONIC_J_H
#define CONIC_J_H

#include <jni.h>
#include <string>

#include "u.h"

inline auto mk_jstr(JNIEnv *env, std::string &x) -> jstring {
    return env->NewStringUTF(x.c_str());
}

inline auto mk_jstr(JNIEnv *env, const char *x) -> jstring {
    return env->NewStringUTF(x);
}

#endif