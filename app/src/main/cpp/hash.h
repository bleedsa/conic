#ifndef CONIC_HASH_H
#define CONIC_HASH_H

namespace Hash {
    template<typename X>
    struct Std;

    template<>
    struct Std<Str::A> {
        inl auto operator()(const Str::A &str) const -> S {
            return std::hash<std::string>()(std::string(str.ptr));
        }
    };
}

#endif
