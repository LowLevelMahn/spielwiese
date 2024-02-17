#pragma once

#include <stdexcept>
#include <string>

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
        ~Dll_lib();

        template<typename FunctionType>
        FunctionType* get_function_address( const std::string& function_name_ ) const
        {
            return static_cast<FunctionType*>( get_function_address_internal( function_name_.c_str() ) );
        }

    private:
        Dll_lib(Dll_lib const&) = delete;
        Dll_lib& operator=(Dll_lib const&) = delete;

        void load_library( const char* library_name_ );
        void unload_library();

        void* get_function_address_internal( const char* function_name_ ) const;
    
#ifdef __linux__
        void*    m_handle
#elif _WIN32
        HMODULE m_handle;
#endif
};
