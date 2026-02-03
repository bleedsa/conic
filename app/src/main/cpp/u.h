//
// Created by skye on 1/28/26.
//

#ifndef CONIC_U_H
#define CONIC_U_H

#include <cctype>
#include <cstdint>
#include <cstddef>

#ifdef ANDROID
#include <android/log.h>
#endif

/* always inline functions */
#ifndef DBG
#define inl [[clang::always_inline]] inline
#else
#define inl inline
#endif

/* sizeof alias */
#define Z(x) (sizeof(x))

/* debug print and return */
#define _(f, x...) ({log_info(TAG, f, ({x;}));x;})

/* format a string using stringstream << oprs */
#define str_fmt(f...) ({ \
    std::stringstream ss; \
    ss << f;           \
    ss.str(); \
})

#define eprintf(f...) (fprintf(stderr, f))

#ifdef ANDROID
/* log an error to android with a tag and a printf formatted string */
#define log_err(tag, f...) { \
    auto _tag = str_fmt("[C++]: " << tag); \
    __android_log_print(ANDROID_LOG_ERROR, _tag.c_str(), f); \
}

/* log info to android with a tag */
#define log_info(tag, f...) { \
    auto _tag = str_fmt("[C++]: " << tag); \
    __android_log_print(ANDROID_LOG_INFO, _tag.c_str(), f); \
}
#else
#define log_err(tag, f...) {eprintf("[%s]: ", tag);eprintf(f);std::cerr<<std::endl;}
#define log_info(tag, f...){printf("[%s]: ", tag);printf(f);std::cout<<std::endl;}
#endif

/* fatal error */
#define fatal(tag, f...) {log_err(tag, "[FATAL]: " f);exit(-1);}

using S = size_t;

namespace U {
    extern const char *VERSION;
}

#endif
