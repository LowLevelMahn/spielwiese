// build: cl.exe my_dll_loader.cpp
//        clang++ my_dll_loader.cpp -o my_dll_loader

#include <cstdio>
#include <cassert>

#ifdef __linux__
#include <dlfcn.h>
#elif _WIN32
#include <windows.h>
#else
    #error not supported OS
#endif  

//using my_dll_func = int(char* u);

//typedef int (*my_dll_func)(const char* u);
using my_dll_func = int(const char* u);

int main()
{
#ifdef __linux__ 
  printf("dlopen\n");
  void* handle = dlopen("my_dll.so", RTLD_LAZY);
  assert(handle);

  printf("dlsym\n");
  my_dll_func* my_dll_func_ptr = (my_dll_func*)dlsym(handle, "my_dll_func");
  assert(my_dll_func_ptr != NULL);
  
  printf("my_dll_func\n");
  int y = my_dll_func_ptr("hello");

  printf("y: %i\n", y);
  
  int res = dlclose(handle);
  assert(res == 0);
#elif _WIN32
  printf("LoadLibrary\n");
  HMODULE hDLL = LoadLibrary("my_dll.dll");
  assert(hDLL != NULL);

  printf("GetProcAddress\n");
  my_dll_func* my_dll_func_ptr = (my_dll_func*)GetProcAddress(hDLL, "my_dll_func");
  assert(my_dll_func_ptr != NULL);
  
  printf("my_dll_func\n");
  int y = my_dll_func_ptr("hello");

  printf("y: %i\n", y);
  
  BOOL res = FreeLibrary(hDLL);
  assert(res != FALSE);
#else
    #error not supported OS
#endif  
  return 0;
}
