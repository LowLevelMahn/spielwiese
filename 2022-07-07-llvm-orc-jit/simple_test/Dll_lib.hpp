#pragma once

#include <stdexcept>

#ifdef __linux__
#include <dlfcn.h>
#elif _WIN32
#include <windows.h>
#else
    #error not supported OS
#endif  

class Dll_lib 
{
  public:
    Dll_lib(const char* library_name_);

    template<typename FunctionType>
    FunctionType* get_function_address( const char* function_name_ )
    {
        return static_cast<FunctionType*>( get_function_address_internal( function_name_ ) );
    }

  private:
    void load_library( const char* library_name_ );
    void unload_library();

    void* get_function_address_internal( const char* function_name_ );
    
#ifdef __linux__
    void*    m_handle
#elif _WIN32
    HMODULE m_handle;
#endif
};
