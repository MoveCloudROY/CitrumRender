#pragma once

#include "core/tools.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <filesystem>

namespace EG {

class Log {
public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
    static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }

private:
    static std::shared_ptr<spdlog::logger> coreLogger;
    static std::shared_ptr<spdlog::logger> clientLogger;
};

} // namespace EG

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector) {
    return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix) {
    return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion) {
    return os << glm::to_string(quaternion);
}

// Core log macros
#define EG_CORE_TRACE(...) ::EG::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EG_CORE_INFO(...) ::EG::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EG_CORE_WARN(...) ::EG::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EG_CORE_ERROR(...) ::EG::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EG_CORE_CRITICAL(...) ::EG::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define EG_TRACE(...) ::EG::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EG_INFO(...) ::EG::Log::GetClientLogger()->info(__VA_ARGS__)
#define EG_WARN(...) ::EG::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EG_ERROR(...) ::EG::Log::GetClientLogger()->error(__VA_ARGS__)
#define EG_CRITICAL(...) ::EG::Log::GetClientLogger()->critical(__VA_ARGS__)



#ifdef EG_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#    define EG_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
        {                                                  \
            if (!(check)) {                                \
                EG##type##ERROR(msg, __VA_ARGS__);         \
                EG_DEBUGBREAK();                           \
            }                                              \
        }
#    define EG_INTERNAL_ASSERT_WITH_MSG(type, check, ...) EG_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#    define EG_INTERNAL_ASSERT_NO_MSG(type, check) EG_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", EG_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#    define EG_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#    define EG_INTERNAL_ASSERT_GET_MACRO(...) EG_EXPAND_MACRO(EG_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, EG_INTERNAL_ASSERT_WITH_MSG, EG_INTERNAL_ASSERT_NO_MSG))

// Currently accepts at least the condition and one additional parameter (the message) being optional
#    define EG_ASSERT(...) EG_EXPAND_MACRO(EG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#    define EG_CORE_ASSERT(...) EG_EXPAND_MACRO(EG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#    define EG_ASSERT(...)
#    define EG_CORE_ASSERT(...)
#endif
