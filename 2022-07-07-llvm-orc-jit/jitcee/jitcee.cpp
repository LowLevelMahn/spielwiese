#include "jitcee.h"

#include "Compiler.hpp"
#include "Jit.hpp"

#include <llvm/Support/TargetSelect.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/IR/Module.h>

// jitcee ist der namespace der dll (JITCEE)
// parameter reihenfolge: mit was arbeiten wir -> was sind die parameter -> was ist das ergebnis

#define error_on_throw( X ) \
try \
{\
    X \
    return 0;\
}\
catch(...) \
{\
    return -1;\
}

int jitcee_compiler_create( void** compiler_handle_ ) noexcept
{
    error_on_throw( *compiler_handle_ = new jitcee::Compiler(); );
}

int jitcee_compiler_free ( void* compiler_handle_ ) noexcept 
{
    delete static_cast<jitcee::Compiler*>(compiler_handle_);
    return 0;
}

int jitcee_compiler_instance_create( void* compiler_handle_, void** compiler_instance_handle_ ) noexcept
{
    error_on_throw( *compiler_instance_handle_ = static_cast<jitcee::Compiler*>(compiler_handle_)->create_compiler_instance().release(); );
    return 0;
}

int jitcee_compiler_instance_free( void* compiler_instance_handle_ ) noexcept
{
    delete static_cast<jitcee::Compiler_instance*>(compiler_instance_handle_);
    return 0;
}

int jitcee_compile_result_free( void* compile_result_handle_ ) noexcept
{
    delete static_cast<jitcee::Compile_result*>(compile_result_handle_);
    return 0;
}

int jitcee_compiler_compile( void* compiler_handle_, void* compiler_instance_handle_, const char* code_, void** compile_result_handle_ ) noexcept
{
    try 
    {
         auto [context, module] =
            static_cast<jitcee::Compiler*>(compiler_handle_)
                ->compile(static_cast<jitcee::Compiler_instance*>(
                              compiler_instance_handle_),
                          code_); 
        *compile_result_handle_ = new jitcee::Compile_result{ std::move(context),  std::move(module) };
         return 0;
    }
    catch(...) 
    {
        return -1;
    }
}

int jitcee_compiler_diagnostic_option_show_color_set( void* compiler_handle_, int show_color_ ) noexcept
{
    error_on_throw( static_cast<jitcee::Compiler*>(compiler_handle_)->diagnostic_option_set_show_color(show_color_); ); 
}

int jitcee_compile_options_set_optimization_level( void* compiler_instance_handle_, int optimization_level_ ) noexcept
{
    error_on_throw( static_cast<jitcee::Compiler_instance*>(compiler_instance_handle_)->compile_options_set_optimization_level( optimization_level_); );
}

int jitcee_compile_options_set_inlining( void* compiler_instance_handle_, int inlining_method_ ) noexcept
{
    error_on_throw(static_cast<jitcee::Compiler_instance*>(compiler_instance_handle_)->compile_options_set_inlining( inlining_method_); );
}

int jitcee_jit_create( void** jit_handle_ ) noexcept
{
    error_on_throw( *jit_handle_ = new jitcee::Jit(); );
}

int jitcee_jit_free( void* jit_handle_ ) noexcept
{
    delete static_cast<jitcee::Jit*>(jit_handle_);
    return 0;
}

int jitcee_external_symbols_create( void* jit_handle_, void** external_symbols_handle_ ) noexcept
{
   error_on_throw( *external_symbols_handle_ = new jitcee::External_symbols(static_cast<jitcee::Jit*>(jit_handle_)->get_mangler()); );
}

int jitcee_external_symbols_add( void* external_symbols_handle_, const char* symbol_name_, void* function_ptr_ ) noexcept
{
    error_on_throw( static_cast<jitcee::External_symbols*>(external_symbols_handle_)->add_external( symbol_name_, function_ptr_); );
}

int jitcee_external_symbols_apply( void* jit_handle_, void* external_symbols_handle_ ) noexcept
{
   error_on_throw( static_cast<jitcee::Jit*>(jit_handle_)->apply_external_symbols(static_cast<jitcee::External_symbols*>(external_symbols_handle_) ); );
}

int jitcee_external_symbols_free( void* external_symbols_handle_ ) noexcept
{
    delete static_cast<jitcee::External_symbols*>(external_symbols_handle_);
    return 0;
}

int jitcee_initialize() noexcept
{
    error_on_throw(  
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    );

    return 0;
}

int jitcee_shutdown() noexcept
{
    return 0;
}

int jitcee_jit_add_thread_safe_module( void* jit_handle_, void* compile_result_handle_, void** resource_tracker_handle_ ) noexcept
{
   error_on_throw( *resource_tracker_handle_ = static_cast<jitcee::Jit*>(jit_handle_)->add_thread_safe_module(static_cast<jitcee::Compile_result*>(compile_result_handle_)); );
}

int jitcee_jit_lookup( void* jit_handle_, const char* symbol_name_, void** function_ptr_ ) noexcept
{
   error_on_throw( *function_ptr_ = static_cast<jitcee::Jit*>(jit_handle_)->lookup(symbol_name_).getAddress().toPtr<void*>(); );
}

int jitcee_resource_tracker_free( void* resource_tracker_handle_ ) noexcept
{
    auto* rt = static_cast<Heaper<llvm::orc::ResourceTrackerSP>*>(resource_tracker_handle_);
    auto err = rt->data()->remove();
    delete rt;

    if (err) {
        return -1;
    }

    return 0;
}

        
