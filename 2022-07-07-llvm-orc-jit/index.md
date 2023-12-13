+++
title = "Jit C in memory using LLVM ORC api"

[taxonomies]
tags = ["llvm", "clang", "c++"]
+++

Based on the in-memory compiler shared in the last post ([C to LLVM IR in
memory using libclang](@/2022-06-18-libclang-c-to-llvm-ir/index.md)), this post
demonstrates a small *just in time (JIT)* compiler which allows to compile C
code to host native code in-memory.

The JIT compiler is based on the LLVM [ORCv2 API][llvm-orc2] (the newest LLVM
JIT API at the time of writing) and the crucial parts are taken from the [JIT
tutorial][llvm-jit-tut].

The sources are available under [llvm-orc-jit][post-src].

### main.cc
```cpp
{{ include(path="content/2022-07-07-llvm-orc-jit/main.cc") }}
```

### jit.h
```cpp
{{ include(path="content/2022-07-07-llvm-orc-jit/jit.h") }}
```

### compiler.h
```cpp
{{ include(path="content/2022-07-07-llvm-orc-jit/ccompiler.h") }}
```

### Makefile
```make
{{ include(path="content/2022-07-07-llvm-orc-jit/Makefile") }}
```
[post-src]: https://git.memzero.de/blog/tree/content/2022-07-07-llvm-orc-jit?h=main
[src-clang]: https://github.com/llvm/llvm-project/tree/main/clang
[blog-clang-in-memory]: https://blog.audio-tk.com/2018/09/18/compiling-c-code-in-memory-with-clang/
[llvm-jit-tut]: https://www.llvm.org/docs/tutorial/BuildingAJIT1.html
[llvm-orc2]: https://www.llvm.org/docs/ORCv2.html
