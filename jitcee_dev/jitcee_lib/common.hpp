#pragma once

#include <stdexcept>
#include <cassert>

#define throw_on_error( X ) \
if ((X) != 0 ) \
{\
    throw std::runtime_error{""};\
}

#define assert_no_error( X ) \
assert ((X) == 0 );

using f_void = int();
using f_ptr = int(void*);
using f_ptr2 = int(void**);
using f_ptr_int = int(void*, int);
using f_ptr_ptr_char_ptr2 = int(void*, void*, const char*, void**);
using f_ptr_ptr2 = int(void*, void**);
using f_ptr_ptr_ptr2 = int(void*, void*,void**);
using f_ptr_char_ptr2 = int(void*, const char*, void**);
using f_ptr_ptr = int(void*, void*);
using f_ptr_char_ptr = int(void*, const char*, void*);