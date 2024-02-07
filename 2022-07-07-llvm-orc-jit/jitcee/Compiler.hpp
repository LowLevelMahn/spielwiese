#pragma once

#include <memory>

namespace llvm 
{
    class LLVMContext;
    class Module;
}

namespace clang
{
    class DiagnosticConsumer;
    class DiagnosticsEngine;
    class CompilerInstance;
}

namespace jitcee {
    
    struct Compile_result
    {
        std::unique_ptr<llvm::LLVMContext> context;
        std::unique_ptr<llvm::Module> module;
    };

    class Compiler_instance;

    class Compiler {
        public:
            Compiler();

            Compile_result compile(Compiler_instance* compiler_instance_, const char* code) const;

            void diagnostic_option_set_show_color(int show_color_) const;

            std::unique_ptr<Compiler_instance> create_compiler_instance() const;

        private:
            std::unique_ptr<clang::DiagnosticConsumer> m_diagnostic_consumer;
            std::unique_ptr<clang::DiagnosticsEngine> m_diagnistic_engine;
    };

    class Compiler_instance
    {
        public:
            Compiler_instance();
            ~Compiler_instance() = default;
            void compile_options_set_optimization_level(  int optimization_level_ ) const;
            void compile_options_set_inlining( int inlining_method_ ) const;
            clang::CompilerInstance* instance() const;

        private:
            std::unique_ptr<clang::CompilerInstance> m_instance;
    };
}  // namespace jitcee
