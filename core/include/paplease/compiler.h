#ifndef PAPLEASE_COMPILER_H
#define PAPLEASE_COMPILER_H 

#include <stdexcept>
#include <string>

[[noreturn]] inline void unreachable()
{
	// Uses compiler specific extensions if possible.
	// Even if no extension is used, undefined behavior is still raised by
	// an empty function body and the noreturn attribute.
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
	__assume(false);
#else // GCC, Clang
	__builtin_unreachable();
#endif
}


#define TODO(msg) \
    throw std::logic_error( \
        std::string(__FILE__ ":") + std::to_string(__LINE__) + " - " + (msg))

#endif // PAPLEASE_COMPILER_H 
