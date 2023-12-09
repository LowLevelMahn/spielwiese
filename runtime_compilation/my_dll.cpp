//
// build: 
//  mit runtime statisch eingebunden, unoptimiert -> 107kb
//  cl.exe /D_USRDLL /D_WINDLL my_dll.cpp /MT /link /DLL /OUT:my_dll.dll
//  mit runtime dynamisch eingebunden, optimiert -> 9kb
//  cl.exe /O2 /D_USRDLL /D_WINDLL my_dll.cpp /MD /link /DLL /OUT:my_dll.dll
//  oder mit clang++
//  clang++ -shared my_dll.cpp -o my_dll.dll
//  using lld as linker
//  clang++ -fuse-ld=lld -shared my_dll.cpp -o my_dll.dll
//
// oder mit LLVM bitcode (bc):           clang++ -emit-llvm -c my_dll.cpp
//                                       clang++ -fuse-ld=lld -shared my_dll.bc -o my_dll.dll
//
// oder mit LLVM IR code (ll, llir): clang++ -emit-llvm -c -S my_dll.cpp
//                                   clang++ -fuse-ld=lld -shared my_dll.ll -o my_dll.dll
//
// clang++ -fuse-ld=lld -shared my_dll.cpp -o my_dll.dll
//

#ifdef __linux__ 
    #define SHARED_API
#elif _WIN32
    #define SHARED_API __declspec(dllexport)
#else
    #error not supported OS
#endif

extern "C" SHARED_API int my_dll_func(const char* u)
{
  return 23;
}

/*

a = 2
b = 3

a*b*c+10

extern "C" __declspec(dllexport) int evaluate()
{
#if DEBUG_ACTIVE
  int a = 2;
  on_scope_value(ast_node_ptr+0, &a);
  on_breakpoint(...);
  int b = 3;
  on_scope_value(ast_node_ptr+0, &b);
  on_breakpoint(...);
  int a_b = a * b;
  on_scope_value(ast_node_ptr+1, &a_b);
  on_breakpoint(...);
  int c = call_get(ast_node_ptr, ???) // from user
  int a_b_c = a_b * c;
  on_scope_value(ast_node_ptr+2, &a_b_c);
  on_breakpoint(...);
  int a_b_c_10 = a_b_c + 10;
  on_scope_value(ast_node_ptr+2, &a_b_c_10);
  on_breakpoint(...);
#else
  return 2*3*get_c()+10;
#endif  
}

*/