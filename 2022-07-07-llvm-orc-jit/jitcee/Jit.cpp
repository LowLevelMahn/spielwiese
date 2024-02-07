#include "Jit.hpp"
#include "Compiler.hpp"

#include <llvm/ExecutionEngine/Orc/CompileUtils.h>

#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/DataLayout.h>

using namespace jitcee;

Jit::Jit() : m_execution_session{std::make_unique<llvm::orc::ExecutionSession>(std::move(cantFail(llvm::orc::SelfExecutorProcessControl::Create())))},  
    m_jit_dylib{m_execution_session->createBareJITDylib("main")}
{
    llvm::orc::JITTargetMachineBuilder target_machine_builder{m_execution_session->getExecutorProcessControl().getTargetTriple()};

    m_data_layout = std::make_unique<llvm::DataLayout>(cantFail(target_machine_builder.getDefaultDataLayoutForTarget()));
    m_mangler = std::make_unique<llvm::orc::MangleAndInterner>(*m_execution_session, *m_data_layout);   
    m_object_layer = std::make_unique<llvm::orc::RTDyldObjectLinkingLayer>(*m_execution_session, []() { return std::make_unique<llvm::SectionMemoryManager>(); }),
    m_compile_layer = std::make_unique<llvm::orc::IRCompileLayer>(*m_execution_session, *m_object_layer, std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(target_machine_builder))),

    // https://www.llvm.org/docs/ORCv2.html#how-to-add-process-and-library-symbols-to-jitdylibs
    // m_jit_dylib.addGenerator(
    //     cantFail(DynamicLibrarySearchGenerator::GetForCurrentProcess(
    //         DL.getGlobalPrefix())));

    m_object_layer->setOverrideObjectFlagsWithResponsibilityFlags(true); // needed for windows    
}

Jit::~Jit() 
{
    if (auto Err = m_execution_session->endSession()) 
    {
        m_execution_session->reportError(std::move(Err));
    }
}

llvm::orc::MangleAndInterner* Jit::get_mangler() const 
{
    return m_mangler.get();
}

void Jit::apply_external_symbols( External_symbols* symbol_map_  ) const
{
    throw_on_error(m_jit_dylib.define( llvm::orc::absoluteSymbols(symbol_map_->data())));
}

Heaper<llvm::orc::ResourceTrackerSP>* Jit::add_thread_safe_module( Compile_result* result_ ) const
{

    llvm::orc::ThreadSafeModule module {std::move(result_->module), std::move(result_->context)};
    auto ressource_tracker = m_jit_dylib.createResourceTracker();
    if (auto E = m_compile_layer->add(ressource_tracker, std::move(module))) 
    {
        throw std::runtime_error{""};
    }
    return new Heaper<llvm::orc::ResourceTrackerSP>(std::move(ressource_tracker));
}

std::string Jit::dump() const 
{
    std::string str{};
    llvm::raw_string_ostream sstr(str);
    m_execution_session->dump(sstr);
    return str;
}

const llvm::orc::ExecutorSymbolDef& Jit::lookup( const char* name_ ) const
{
    llvm::Expected<llvm::orc::ExecutorSymbolDef> x = m_execution_session->lookup({&m_jit_dylib}, (*m_mangler)(name_));
    throw_on_error(x.takeError());
    return x.get();
}
           
External_symbols::External_symbols( llvm::orc::MangleAndInterner* mangler_ ) : m_mangler(mangler_)
{  
}

void External_symbols::add_external( const char* name_, void* ptr_ )
{
    m_external_symbols.try_emplace( (*m_mangler)(name_), llvm::orc::ExecutorSymbolDef(llvm::orc::ExecutorAddr::fromPtr(ptr_), {}) );
}
        
const llvm::orc::SymbolMap& External_symbols::data() 
{
    return m_external_symbols;
}