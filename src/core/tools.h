#pragma once

#include "core/PlatformDetection.h"

#define EG_EXPAND_MACRO(x) x
#define EG_STRINGIFY_MACRO(x) #x

#define CONCAT(x, y) x##y

#define BIT(x) (1 << (x))

#define EG_MEMBER_FUNC_BIND(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }


#ifdef EG_DEBUG
#    if defined(EG_PLATFORM_WINDOWS)
#        define EG_DEBUGBREAK() __debugbreak()
#    elif defined(EG_PLATFORM_LINUX)
#        include <signal.h>
#        define EG_DEBUGBREAK() raise(SIGTRAP)
#    else
#        error "Platform doesn't support debugbreak yet!"
#    endif
#    define EG_ENABLE_ASSERTS
#else
#    define EG_DEBUGBREAK()
#endif