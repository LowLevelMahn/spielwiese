#include "../jitcee_lib/Jitcee.hpp"

#include <iostream>
#include <cstdio>
#include <chrono>
#include <format>

extern "C" bool is_prime(long long n_) {
    if (n_ == 0 || n_ == 1) { return false; }
    for (long long i = 2; i <= n_ / 2; ++i)
        if (n_ % i == 0) { return false; }
    return true; 
}

int main(int argc, char *argv[] ) 
{
    #if 1
         std::string code = R"***(
extern "C" bool is_prime(long long n_) {
    if (n_ == 0 || n_ == 1) { return false; }
    for (long long i = 2; i <= n_ / 2; ++i)
        if (n_ % i == 0) { return false; }
    return true; 
}; )***";
#else
        const char code[] =
            "class C{};"
            "template<typename X> X my_func(X x){ return x; }"
            "constexpr int brk = 23;"
            "extern \"C\" void libc_puts(const char*);"
            "extern \"C\" int libc_printf ( const char * format, ... );"
            "struct S { int a; int b; };"
            "static void init_a(struct S* s) { s->a = 1111; }"
            "static void init_b(struct S* s) { s->b = 2222; }"
            "extern \"C\" void init(struct S* s) {"
            "init_a(s); init_b(s);"
            "libc_puts(\"libc_puts()\");"
            "libc_printf(\"printf: %u\\n\", __cplusplus); }";
#endif

    try 
    {
        auto begin = std::chrono::steady_clock::now();

#if 0
        jitcee::Jitcee jitcee_root{R"(D:\Projects\Spielwiese\build\bin\Debug\jitcee.dll)"};
#else
        jitcee::Jitcee jitcee_root{R"(D:\projects\haas\llvm_dev\_test_build_debug\jitcee\Debug\jitcee.dll)"};
#endif

        jitcee::Compiler compiler = jitcee_root.create_compiler();
        compiler.diagnostic_color(true);
        jitcee::Compiler_instance instance = compiler.create_instance();
        instance.inlining(jitcee::Compiler_instance::Inlining_method::NormalInlining);
        instance.optimization_level(jitcee::Compiler_instance::Optimization_level::O3);
        jitcee::Compile_result compile_result = compiler.compile( instance, code );
      
        jitcee::Jit jit = jitcee_root.create_jit();
        jit.set_external_symbols( { { "libc_puts", &puts } } );
        jit.add_compile_result(compile_result);
        using func_type = bool(long long);
        func_type* func = jit.lookup<func_type>("is_prime");

        auto start = std::chrono::steady_clock::now();
        long long num = 2147483647;
        //long long num = 20000000179;
        std::cout << std::format("{} is{} a prime number\n", num, func(num) ? "" : " not");
        std::cout << std::format("JIT took {}/{} ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() );
        start = std::chrono::steady_clock::now();
        std::cout << std::format("{} is{} a prime number\n", num, is_prime(num) ? "" : " not");
        std::cout << std::format("Native took {} ms ", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() );

        return 0;    
    }
    catch(const std::exception& ex_ ) 
    {   
        std::cout << ex_.what() << "\n";
    }
    
    return -1;
}

