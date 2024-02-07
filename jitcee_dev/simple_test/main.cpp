#include "Dll_lib.hpp"

#include <iostream>

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

struct Funcs
{
    f_void* jitcee_initialize{};
    f_void* jitcee_shutdown{};
    f_ptr2* jitcee_compiler_create{};
    f_ptr* jitcee_compiler_free{};
    f_ptr_int* jitcee_compiler_diagnostic_option_show_color_set{};
    f_ptr_ptr_char_ptr2* jitcee_compiler_compile{};
    f_ptr* jitcee_compile_result_free{};
    f_ptr_ptr2* jitcee_compiler_instance_create{};
    f_ptr* jitcee_compiler_instance_free{};
    f_ptr_int* jitcee_compile_options_set_optimization_level{};
    f_ptr_int* jitcee_compile_options_set_inlining{};
    f_ptr_ptr2* jitcee_external_symbols_create{};
    f_ptr_char_ptr* jitcee_external_symbols_add{};
    f_ptr_ptr* jitcee_external_symbols_apply{};
    f_ptr* jitcee_external_symbols_free{};
    f_ptr2* jitcee_jit_create{};
    f_ptr* jitcee_jit_free{};
    f_ptr_ptr_ptr2* jitcee_jit_add_thread_safe_module{};
    f_ptr_char_ptr2* jitcee_jit_lookup{};
    f_ptr* jitcee_resource_tracker_free{};
};

int main(int argc, char *argv[] ) 
{
    try 
    {
        Dll_lib lib_handle{R"(D:\projects\haas\llvm_dev\_test_build_release\jitcee\Release\jitcee.dll)"};

        Funcs funcs;
        funcs.jitcee_initialize = lib_handle.get_function_address<f_void>("jitcee_initialize" );
        funcs.jitcee_shutdown = lib_handle.get_function_address<f_void>("jitcee_shutdown" );
        funcs.jitcee_compiler_create = lib_handle.get_function_address<f_ptr2>("jitcee_compiler_create" );
        funcs.jitcee_compiler_free = lib_handle.get_function_address<f_ptr>("jitcee_compiler_free" ); 
        funcs.jitcee_compiler_diagnostic_option_show_color_set = lib_handle.get_function_address<f_ptr_int>("jitcee_compiler_diagnostic_option_show_color_set" );
        funcs.jitcee_compiler_compile = lib_handle.get_function_address<f_ptr_ptr_char_ptr2>("jitcee_compiler_compile" );
        funcs.jitcee_compile_result_free = lib_handle.get_function_address<f_ptr>("jitcee_compile_result_free" );
        funcs.jitcee_compiler_instance_create = lib_handle.get_function_address<f_ptr_ptr2>("jitcee_compiler_instance_create" );
        funcs.jitcee_compiler_instance_free = lib_handle.get_function_address<f_ptr>("jitcee_compiler_instance_free" ); 
        funcs.jitcee_compile_options_set_optimization_level = lib_handle.get_function_address<f_ptr_int>("jitcee_compile_options_set_optimization_level" );
        funcs.jitcee_compile_options_set_inlining = lib_handle.get_function_address<f_ptr_int>("jitcee_compile_options_set_inlining" );
        funcs.jitcee_external_symbols_create = lib_handle.get_function_address<f_ptr_ptr2>("jitcee_external_symbols_create" );
        funcs.jitcee_external_symbols_add = lib_handle.get_function_address<f_ptr_char_ptr>("jitcee_external_symbols_add" );
        funcs.jitcee_external_symbols_apply = lib_handle.get_function_address<f_ptr_ptr>("jitcee_external_symbols_apply" );
        funcs.jitcee_external_symbols_free = lib_handle.get_function_address<f_ptr>("jitcee_external_symbols_free" );
        funcs.jitcee_jit_create = lib_handle.get_function_address<f_ptr2>("jitcee_jit_create" );
        funcs.jitcee_jit_free = lib_handle.get_function_address<f_ptr>("jitcee_jit_free" );
        funcs.jitcee_jit_add_thread_safe_module = lib_handle.get_function_address<f_ptr_ptr_ptr2>("jitcee_jit_add_thread_safe_module" );
        funcs.jitcee_jit_lookup = lib_handle.get_function_address<f_ptr_char_ptr2>("jitcee_jit_lookup" );
        funcs.jitcee_resource_tracker_free = lib_handle.get_function_address<f_ptr>("jitcee_resource_tracker_free" );

        const char code[] =
            "extern void libc_puts(const char*);"
            "struct S { int a; int b; };"
            "static void init_a(struct S* s) { s->a = 1111; }"
            "static void init_b(struct S* s) { s->b = 2222; }"
            "void init(struct S* s) {"
            "init_a(s); init_b(s);"
            "libc_puts(\"libc_puts()\"); }";

        auto throw_on_error = []( int status_code_ ){ if ( status_code_ != 0 ) throw std::runtime_error(""); };

        throw_on_error( funcs.jitcee_initialize( ) );

        void* compiler_handle;
        throw_on_error( funcs.jitcee_compiler_create( &compiler_handle ) );
        throw_on_error( funcs.jitcee_compiler_diagnostic_option_show_color_set( compiler_handle, 1 ) );
        void* compiler_intance_handle;
        throw_on_error( funcs.jitcee_compiler_instance_create( compiler_handle, &compiler_intance_handle ) );
        throw_on_error( funcs.jitcee_compile_options_set_inlining( compiler_intance_handle, 0 ) );
        throw_on_error( funcs.jitcee_compile_options_set_optimization_level( compiler_intance_handle, 3 ) );
        void* compile_result_handle;
        throw_on_error( funcs.jitcee_compiler_compile( compiler_handle, compiler_intance_handle, code, &compile_result_handle ) );
        void* jit_handle;
        throw_on_error( funcs.jitcee_jit_create( &jit_handle ) );
         
        void* external_symbols_handle;
        throw_on_error( funcs.jitcee_external_symbols_create( jit_handle, &external_symbols_handle ));
        throw_on_error( funcs.jitcee_external_symbols_add( external_symbols_handle, "libc_puts", &puts ));
        throw_on_error( funcs.jitcee_external_symbols_apply( jit_handle, external_symbols_handle )); //TODO possible to use a wrong jit_handle!
        void* resource_track_handle;
        throw_on_error( funcs.jitcee_jit_add_thread_safe_module( jit_handle, compile_result_handle, &resource_track_handle ) );
        void* function_handle;
        throw_on_error( funcs.jitcee_jit_lookup( jit_handle, "init", &function_handle ) );

        struct S {
            int a, b;
        } state = {0, 0};

        using init_func_type = void(struct S * s);
        auto* init_func = static_cast<init_func_type*>(function_handle);

        std::printf("JIT address 0x%p\n", init_func);

        std::printf("S { a=%d b=%d }\n", state.a, state.b);
        init_func(&state);
        std::printf("S { a=%d b=%d }\n", state.a, state.b);

        throw_on_error( funcs.jitcee_external_symbols_free( external_symbols_handle ));
        throw_on_error( funcs.jitcee_resource_tracker_free( resource_track_handle ) );
        throw_on_error( funcs.jitcee_compile_result_free( compile_result_handle ) );
        throw_on_error( funcs.jitcee_compiler_instance_free( compiler_intance_handle ) );
        throw_on_error( funcs.jitcee_compiler_free( compiler_handle ) );

        throw_on_error( funcs.jitcee_shutdown( ) );

        return 0;    
    }
    catch(const std::exception& ex_ ) 
    {   
        std::cout << ex_.what() << "\n";
    }
    
    return -1;
}
