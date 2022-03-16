#ifndef IL2CPP_UTILS_EXCEPTIONS
#define IL2CPP_UTILS_EXCEPTIONS

#include <exception>
#include <string_view>
#include <string>
#include <stdexcept>
#include "utils-functions.h"

struct Il2CppException;
struct MethodInfo;

namespace il2cpp_utils {
    // Returns a legible string from an Il2CppException*
    ::std::string ExceptionToString(Il2CppException* exp) noexcept;

    #ifdef UNITY_2019
    /// @brief Raises the provided Il2CppException to be used within il2cpp.
    /// @param exp The exception instance to throw
    [[noreturn]] void raise(const Il2CppException* exp);
    #endif

    #if __has_feature(cxx_exceptions)
    struct Il2CppUtilsException : std::runtime_error {
        std::string context;
        std::string msg;
        std::string func = "unknown";
        std::string file = "unknown";
        int line = -1;
        Il2CppUtilsException(std::string_view msg_) : std::runtime_error(CreateMessage(msg_.data())), msg(msg_.data()) {}
        Il2CppUtilsException(std::string_view context_, std::string_view msg_) : std::runtime_error(CreateMessage(msg_.data(), context_.data())), context(context_.data()), msg(msg_.data()) {}
        Il2CppUtilsException(std::string_view msg_, std::string_view func_, std::string_view file_, int line_)
            : std::runtime_error(CreateMessage(msg_.data(), "", func_.data(), file_.data(), line_)), msg(msg_.data()), func(func_.data()), file(file_.data()), line(line_) {}
        Il2CppUtilsException(std::string_view context_, std::string_view msg_, std::string_view func_, std::string_view file_, int line_)
            : std::runtime_error(CreateMessage(msg_.data(), context_.data(), func_.data(), file_.data(), line_)), context(context_.data()), msg(msg_.data()), func(func_.data()), file(file_.data()), line(line_) {}

        std::string CreateMessage(std::string msg, std::string context = "", std::string func = "unknown", std::string file = "unknown", int line = -1) {
            return ((context.size() > 0 ? ("(" + context + ") ") : "") + msg + " in: " + func + " " + file + ":" + std::to_string(line));
        }
    };
    struct RunMethodException : std::runtime_error {
        constexpr static uint16_t STACK_TRACE_SIZE = 256;

        const Il2CppException* ex;
        const MethodInfo* info;
        void* stacktrace_buffer[STACK_TRACE_SIZE];
        uint16_t stacktrace_size;

        RunMethodException(std::string_view msg, const MethodInfo* inf) : std::runtime_error(msg.data()), ex(nullptr), info(inf) {}
        RunMethodException(Il2CppException* exp, const MethodInfo* inf) : std::runtime_error(ExceptionToString(exp).c_str()), ex(exp), info(inf) {
            // Skip 2 frames because we don't want to include this constructor
            stacktrace_size = backtrace_helpers::captureBacktrace(stacktrace_buffer, STACK_TRACE_SIZE, 2);
        }
        // TODO: Add a logger argument here so we could better write out to a targetted buffer.
        // For now, we will stick to using the UtilsLogger.
        // It will be our caller's responsibility to determine what to do AFTER the backtrace is logged-- whether it be to terminate or rethrow.
        // Logs the backtrace with the Logging::ERROR level, using the global logger instance.
        void log_backtrace() const;
        [[noreturn]] void rethrow() const {
            #ifdef UNITY_2019
            il2cpp_utils::raise(ex);
            #else
            #warning "The exception being rethrown like this is unlikely to behave correctly!"
            throw Il2CppExceptionWrapper(ex);
            #endif
        }
    };
    #endif
}

// Implements a try-catch handler which will first attempt to run the provided body.
// If there is an uncaught RunMethodException, it will first attempt to log the backtrace.
// If it holds a valid C# exception, it will attempt to raise it, such that it is caught in the il2cpp domain.
// If an exception is thrown that is otherwise what-able is caught, it will attempt to call the what() method
// and then rethrow the exception to the il2cpp domain.
// If an unknown exception is caught, it will terminate explicitly, as opposed to letting it be thrown across the il2cpp domain.
// All logs that occur as a result of this function will be under the core beatsaber-hook global logger.
#define IL2CPP_CATCH_HANDLER(...) try { \
    __VA_ARGS__ \
} catch (::il2cpp_utils::RunMethodException const& exc) { \
    ::Logger::get().error("Uncaught RunMethodException! what(): %s", exc.what()); \
    exc.log_backtrace(); \
    if (exc.ex) { \
        exc.rethrow(); \
    } \
    SAFE_ABORT(); \
} catch (::std::exception const& exc) { \
    ::Logger::get().error("Uncaught C++ exception! what(): %s", exc.what()); \
    ::il2cpp_utils::raise(exc); \
} catch (...) { \
    ::Logger::get().error("Uncaught, unknown C++ exception with no what()!"); \
    SAFE_ABORT(); \
}

#endif