#pragma once

// jitcee ist der namespace der dll (JITCEE)
// parameter reihenfolge: mit was arbeiten wir -> was sind die parameter -> was ist das ergebnis
#ifdef __linux__ 
    #define SHARED_API
#elif _WIN32
    #define SHARED_API __declspec(dllexport)
#else
    #error not supported OS
#endif

extern "C" {
   
    SHARED_API int jitcee_initialize() noexcept;
    SHARED_API int jitcee_shutdown() noexcept;

    // Compiler
    SHARED_API int jitcee_compiler_create( void** compiler_handle ) noexcept;
    SHARED_API int jitcee_compiler_free ( void* compiler_handle) noexcept;
    SHARED_API int jitcee_compiler_diagnostic_option_show_color_set( void* compiler_handle, int show_color_ ) noexcept;
    //TODO more diagnostic options???
    SHARED_API int jitcee_compiler_compile( void* compiler_handle, void* compiler_instance_handle, const char* code, void** compile_result_handle ) noexcept;
    SHARED_API int jitcee_compile_result_free( void* compile_result_handle ) noexcept;

    // Compiler instance
    SHARED_API int jitcee_compiler_instance_create( void* compiler_handle, void** compiler_instance_handle ) noexcept;
    SHARED_API int jitcee_compiler_instance_free( void* compiler_instance_handle ) noexcept;
    SHARED_API int jitcee_compile_options_set_optimization_level( void* compiler_instance_handle, int optimization_level_ ) noexcept;
    SHARED_API int jitcee_compile_options_set_inlining( void* compiler_instance_handle, int inlining_method_ ) noexcept;
    //TODO more compile options???

    // External symbols
    SHARED_API int jitcee_external_symbols_create( void* jit_handle, void** external_symbols_handle ) noexcept;
    SHARED_API int jitcee_external_symbols_add( void* external_symbols_handle, const char* symbol_name, void* function_ptr ) noexcept;
    SHARED_API int jitcee_external_symbols_apply( void* jit_handle, void* external_symbols_handle ) noexcept;
    SHARED_API int jitcee_external_symbols_free( void* external_symbols_handle ) noexcept;

    // Jit
    SHARED_API int jitcee_jit_create( void** jit_handle ) noexcept;
    SHARED_API int jitcee_jit_free( void* jit_handle ) noexcept;
    SHARED_API int jitcee_jit_add_thread_safe_module( void* jit_handle, void* compile_result_handle, void** resource_tracker_handle ) noexcept;
    SHARED_API int jitcee_jit_lookup( void* jit_handle, const char* symbol_name, void** function_ptr ) noexcept;
    SHARED_API int jitcee_resource_tracker_free( void* resource_tracker_handle_ ) noexcept;
    //TODO are there any jit options?

}
