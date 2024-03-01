#include "Jit.hpp"
#include "Compiler.hpp"
#include "Dll_lib.hpp"

using namespace jitcee;

Jit::~Jit()
{
    for ( void* resource_tracker_handle : m_resource_tracker_handles )
    {
        assert_no_error( m_jitcee_resource_tracker_free( resource_tracker_handle ) );
    }
    assert_no_error( m_jitcee_jit_free( m_jit_handle ) );
}

void Jit::set_external_symbols(const std::vector<External_symbol>& symbols_) 
{
    void* external_symbols_handle{};
    throw_on_error( m_jitcee_external_symbols_create( &external_symbols_handle ) );
    for( const auto& symbol : symbols_ )
    {
        throw_on_error( m_jitcee_external_symbols_add( external_symbols_handle, symbol.name.c_str(), symbol.function_ptr ) );
    }
    throw_on_error( m_jitcee_jit_apply_external_symbols( m_jit_handle, external_symbols_handle ) );
    throw_on_error( m_jitcee_external_symbols_free( external_symbols_handle ) );
}

void Jit::add_compile_result( const Compile_result& result_ ) 
{
    void* resource_tracker_handle;
    throw_on_error( m_jitcee_jit_add_thread_safe_module( m_jit_handle, result_.m_compile_result_handle, &resource_tracker_handle ) );
    m_resource_tracker_handles.push_back(resource_tracker_handle);
}

Jit::Jit( const Dll_lib& handle_ ) :         
    m_jitcee_resource_tracker_free{handle_.get_function_address<f_ptr>("jitcee_resource_tracker_free" ) },
    m_jitcee_jit_free{ handle_.get_function_address<f_ptr>("jitcee_jit_free" ) },
    m_jitcee_jit_add_thread_safe_module{ handle_.get_function_address<f_ptr_ptr_ptr2>("jitcee_jit_add_thread_safe_module" ) },
    m_jitcee_jit_create{ handle_.get_function_address<f_ptr2>("jitcee_jit_create" ) },
    m_jitcee_external_symbols_create{ handle_.get_function_address<f_ptr>("jitcee_external_symbols_create" ) },
    m_jitcee_external_symbols_add{ handle_.get_function_address<f_ptr_char_ptr>("jitcee_external_symbols_add" ) },
    m_jitcee_jit_apply_external_symbols{ handle_.get_function_address<f_ptr_ptr>("jitcee_jit_apply_external_symbols" ) },
    m_jitcee_external_symbols_free{ handle_.get_function_address<f_ptr>("jitcee_external_symbols_free" ) },
    m_jitcee_jit_lookup{ handle_.get_function_address<f_ptr_char_ptr2>("jitcee_jit_lookup" ) }
{
    throw_on_error( m_jitcee_jit_create( &m_jit_handle ) );
}

void* Jit::lookup(const std::string& name_) const 
{
    void* function_ptr{};
    throw_on_error( m_jitcee_jit_lookup(m_jit_handle, name_.c_str(), &function_ptr) );
    return function_ptr;
}
