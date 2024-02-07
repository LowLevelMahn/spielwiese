#include "Compiler.hpp"
#include "common.hpp"

#include <system_error>

#include <clang/Basic/DiagnosticOptions.h>
#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Lex/PreprocessorOptions.h>

#include <llvm/IR/Module.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>

using namespace jitcee;

Compiler::Compiler()
{
    auto diagnostic_options =
        llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions>(new clang::DiagnosticOptions());

    m_diagnostic_consumer = std::make_unique<clang::TextDiagnosticPrinter>(llvm::errs(), diagnostic_options.get());
    m_diagnistic_engine = std::make_unique<clang::DiagnosticsEngine>( nullptr, std::move(diagnostic_options), m_diagnostic_consumer.get(), false );
}

std::unique_ptr<Compiler_instance> Compiler::create_compiler_instance() const
{
    auto compiler_instance = std::make_unique<Compiler_instance>();
    compiler_instance->compile_options_set_optimization_level(3); //default?
    compiler_instance->compile_options_set_inlining(clang::CodeGenOptions::NormalInlining);//default?
    return compiler_instance;
}

Compile_result Compiler::compile( Compiler_instance* compiler_instance_, const char* code) const 
{
    const auto err = [](llvm::errc ec) { return std::error_code(static_cast<int>(ec), std::generic_category()); };

    const char code_fname[] = "jit.c"; //TODO

    auto* instance = compiler_instance_->instance();
    bool ok = clang::CompilerInvocation::CreateFromArgs(instance->getInvocation(), {code_fname}, *m_diagnistic_engine);
    assert(ok);

    instance->createDiagnostics(m_diagnostic_consumer.get(), false);
    instance->getPreprocessorOpts().addRemappedFile(code_fname, llvm::MemoryBuffer::getMemBuffer(code).release());

    // Generate LLVM IR.
    clang::EmitLLVMOnlyAction code_gen_action;
    if (!instance->ExecuteAction(code_gen_action)) {
        throw_on_error( llvm::make_error<llvm::StringError>(
            "Failed to generate LLVM IR from C code!",
            err(llvm::errc::invalid_argument)));
    }

    // Take generated LLVM IR module and the LLVMContext.
    auto module = code_gen_action.takeModule();
    auto context = std::unique_ptr<llvm::LLVMContext>(code_gen_action.takeLLVMContext());

    // TODO: Can this become nullptr when the action succeeds?
    assert(module);
    assert(context);

    return { std::move(context), std::move(module) };
}

void Compiler::diagnostic_option_set_show_color(int show_color_) const
{
    m_diagnistic_engine->getDiagnosticOptions().ShowColors = 1; //map bool to 1 0?
}

Compiler_instance::Compiler_instance(): m_instance(std::make_unique<clang::CompilerInstance>())
{
}

void Compiler_instance::compile_options_set_optimization_level( int optimization_level_ ) const
{
   m_instance->getCodeGenOpts().OptimizationLevel = optimization_level_;
}

void Compiler_instance::compile_options_set_inlining( int inlining_method_ ) const
{
   m_instance->getCodeGenOpts().setInlining( static_cast<clang::CodeGenOptions::InliningMethod>(inlining_method_) );
}

clang::CompilerInstance* Compiler_instance::instance() const
{
    return m_instance.get();
}
