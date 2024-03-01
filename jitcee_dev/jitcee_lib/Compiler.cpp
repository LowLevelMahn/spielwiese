#include "Compiler.hpp"
#include "Dll_lib.hpp"

using namespace jitcee;

Compile_result::~Compile_result() 
{
    m_jitcee_compile_result_free(m_compile_result_handle);
}

Compile_result::Compile_result(const Dll_lib& handle_, void* compile_result_handle_) :
    m_jitcee_compile_result_free( handle_.get_function_address<f_ptr>("jitcee_compile_result_free")), 
    m_compile_result_handle(compile_result_handle_) 
{
}

Compiler::~Compiler() 
{
    assert_no_error( m_jitcee_compiler_free( m_compiler_handle ) );
}

void Compiler::diagnostic_color(bool show_color_) const 
{
    throw_on_error( m_jitcee_compiler_diagnostic_option_show_color_set(m_compiler_handle, show_color_ ? 1 : 0 ) );
}

Compile_result Compiler::compile(const Compiler_instance& compiler_instance_, const std::string& code_) const 
{
    void* compile_result_handle{};
    throw_on_error( m_jitcee_compiler_compile(m_compiler_handle, compiler_instance_.m_compiler_instance_handle, code_.c_str(), &compile_result_handle ) );
    return Compile_result{ m_dll_handle, compile_result_handle };
}

Compiler_instance Compiler::create_instance() const 
{
    void* compiler_instance_handle{};
    throw_on_error( m_jitcee_compiler_instance_create(m_compiler_handle, &compiler_instance_handle ) );
    return Compiler_instance{ m_dll_handle, compiler_instance_handle };
}

Compiler::Compiler(const Dll_lib& handle_) :
    m_dll_handle(handle_), 
    m_jitcee_compiler_create( handle_.get_function_address<f_ptr2>( "jitcee_compiler_create" ) ), 
    m_jitcee_compiler_free( handle_.get_function_address<f_ptr>( "jitcee_compiler_free" ) ),
    m_jitcee_compiler_diagnostic_option_show_color_set( handle_.get_function_address<f_ptr_int>( "jitcee_compiler_diagnostic_option_show_color_set" ) ),
    m_jitcee_compiler_compile( handle_.get_function_address<f_ptr_ptr_char_ptr2>( "jitcee_compiler_compile" ) ), 
    m_jitcee_compiler_instance_create( handle_.get_function_address<f_ptr_ptr2>( "jitcee_compiler_instance_create" ) ) 
{
    throw_on_error( m_jitcee_compiler_create(&m_compiler_handle) );
}

void Compiler_instance::optimization_level( Optimization_level optimization_level_ ) const
{
   throw_on_error( m_jitcee_compile_options_set_optimization_level( m_compiler_instance_handle, static_cast<int>( optimization_level_ ) ) );
}

void Compiler_instance::inlining( Inlining_method inlining_method_ ) const
{
   throw_on_error( m_jitcee_compile_options_set_inlining( m_compiler_instance_handle, static_cast<int>(inlining_method_) ) );
}

Compiler_instance::Compiler_instance( const Dll_lib& handle_, void* compiler_instance_handle_ ) : 
    m_jitcee_compiler_instance_free( handle_.get_function_address<f_ptr>( "jitcee_compiler_instance_free" ) ),
    m_jitcee_compile_options_set_optimization_level( handle_.get_function_address<f_ptr_int>( "jitcee_compile_options_set_optimization_level" ) ),
    m_jitcee_compile_options_set_inlining( handle_.get_function_address<f_ptr_int>( "jitcee_compile_options_set_inlining" ) ),
    m_compiler_instance_handle( compiler_instance_handle_ )
{
}

Compiler_instance::~Compiler_instance() 
{
    assert_no_error( m_jitcee_compiler_instance_free( m_compiler_instance_handle ) );
}
