#include "ccompiler.h"
#include "common.hpp"
#include "jit.h"

int main() {
  const char code[] = "extern void libc_puts(const char*);"
                      "struct S { int a; int b; };"
                      "static void init_a(struct S* s) { s->a = 1111; }"
                      "static void init_b(struct S* s) { s->b = 2222; }"
                      "void init(struct S* s) {"
                      "init_a(s); init_b(s);"
                      "libc_puts(\"libc_puts()\"); }";

  auto R = cc::CCompiler().compile(code);
    throw_on_error(R.takeError());
  // Abort if compilation failed.
  auto [C, M] = cantFail(std::move(R));
  // M->print(llvm::errs(), nullptr);

  // -- JIT compiler the IR module.

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();

  auto JIT = jit::Jit::Create();
  auto TSM = llvm::orc::ThreadSafeModule(std::move(M), std::move(C));

  auto RT = JIT->addModule(std::move(TSM));
    throw_on_error(RT.takeError());

  if (auto ADDR = JIT->lookup("init")) {
#if 0
    // LLVM 13.x
    std::printf("JIT ADDR 0x%lx\n", (*ADDR).getAddress());

    struct S {
      int a, b;
    } state = {0, 0};
    auto JIT_FN = (void (*)(struct S *))(*ADDR).getAddress();
#else
    // LLVM 17.x
    const llvm::orc::ExecutorAddr& addr = ADDR->getAddress();

    struct S {
      int a, b;
    } state = {0, 0};
    using init_func_type = void(struct S * s);
    auto JIT_FN = addr.toPtr<init_func_type>();
    std::printf("JIT ADDR 0x%lx\n", JIT_FN);
#endif

    std::printf("S { a=%d b=%d }\n", state.a, state.b);
    JIT_FN(&state);
    std::printf("S { a=%d b=%d }\n", state.a, state.b);
  }

  // Remove jitted code tracked by this RT.
    throw_on_error((*RT)->remove());

#if 0
  if (auto E = JIT->lookup("init").takeError()) {
        llvm::errs()
            << "Expected error, we dropped removed code tracked by RT and "
                    "hence 'init' should be "
                    "removed from the JIT!\n";
  }
#endif

  return 0;
}
