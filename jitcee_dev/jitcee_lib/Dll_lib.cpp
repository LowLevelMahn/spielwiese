#include "Dll_lib.hpp"

#include <stdexcept>

#ifdef __linux__
#include <dlfcn.h>
#elif _WIN32
#include <windows.h>
#else
    #error not supported OS
#endif  

Dll_lib::Dll_lib(const char* library_name_) 
{
    load_library( library_name_ );
}

Dll_lib::~Dll_lib()
{
    unload_library();
}

void Dll_lib::load_library( const char* library_name_ )
{
#ifdef __linux__
    m_handle = dlopen(library_name_, RTLD_LAZY);
#elif _WIN32
    m_handle = LoadLibrary(library_name_);
#endif
    if ( !m_handle )
    {
        throw std::runtime_error{"cannot load dll: " + std::string{library_name_} };
    }
}   

void* Dll_lib::get_function_address_internal( const char* function_name_ ) const
{
    void* func_ptr{};
#ifdef __linux__
    func_ptr = (void*)dlsym(libray_handle_, function_name_);
#elif _WIN32
    func_ptr = GetProcAddress(m_handle, function_name_);
#endif

    if ( !func_ptr )
    {
        throw std::runtime_error{"cannot get address of function " + std::string{function_name_} };
    }
    return func_ptr;
}
    
void Dll_lib::unload_library()
{
#ifdef __linux__
    int res = dlclose(libray_handle_);
#elif _WIN32
    BOOL res = FreeLibrary(m_handle);
#endif
    if ( res == FALSE )
    {
        throw std::runtime_error{"cannot unload dll" };
    }
}
