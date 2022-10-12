#ifndef __MACRO_H__
#define __MACRO_H__

#include <stdexcept>

#define ASSERT(x, w) \
    if (!(x)) { \
        throw std::runtime_error(w); \
    }

#define BUFFER_OFFSET(i) ((void*)(i))

#endif // __MACRO_H__
