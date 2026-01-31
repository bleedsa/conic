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
#define inl [[clang::always_inline]] inline
/* sizeof alias */
#define Z(x) (sizeof(x))

/* format a string using stringstream << oprs */
#define str_fmt(f...) ({ \
    std::stringstream ss; \
    ss << f;           \
    ss.str(); \
})

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
#define log_err(tag, f...) {fprintf(stderr, f);putc('\n', stderr);}
#define log_info(tag, f...){printf(f);putc('\n', stdout);}
#endif

/* fatal error */
#define fatal(tag, f...) {log_err(tag, "[FATAL]: " f);exit(-1);}

using S = size_t;

namespace U {
    extern const char *VERSION;
}

#endif
