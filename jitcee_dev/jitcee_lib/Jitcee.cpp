#include "Jitcee.hpp"

using namespace jitcee;

Jitcee::Jitcee(const std::filesystem::path& path_to_dll_ ) :
    m_dll_lib{ path_to_dll_.string().c_str() }, 
    m_jitcee_initialize( m_dll_lib.get_function_address<f_void>("jitcee_initialize" ) ), 
    m_jitcee_shutdown( m_dll_lib.get_function_address<f_void>("jitcee_shutdown" ) )
{
    throw_on_error( m_jitcee_initialize() );
}

Jitcee::~Jitcee() {
    assert_no_error( m_jitcee_shutdown( ) );
}

Compiler Jitcee::create_compiler() {
    return Compiler{ m_dll_lib };
}

Jit Jitcee::create_jit() {
    return Jit{ m_dll_lib };
}
