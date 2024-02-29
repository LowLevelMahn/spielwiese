#include "Jit.hpp"
#include "Compiler.hpp"

#include <llvm/ExecutionEngine/Orc/CompileUtils.h>

#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/Orc/IRTransformLayer.h>
#include <llvm/ExecutionEngine/Orc/ObjectTransformLayer.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/ExecutionEngine/Orc/DebugUtils.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>


using namespace jitcee;

static llvm::Expected<llvm::orc::ThreadSafeModule> optimizeModule(llvm::orc::ThreadSafeModule module_, const llvm::orc::MaterializationResponsibility &responsibility_) 
{
    module_.withModuleDo([](llvm::Module &m_) {
        // Create a function pass manager.
        auto function_pass_manager = std::make_unique<llvm::legacy::FunctionPassManager>(&m_);
      
        // Add some optimizations.
        function_pass_manager->add(llvm::createInstructionCombiningPass());
        function_pass_manager->add(llvm::createReassociatePass());
        function_pass_manager->add(llvm::createGVNPass());
        function_pass_manager->add(llvm::createCFGSimplificationPass());
        function_pass_manager->doInitialization();      

        // Run the optimizations over all functions in the module being added to the JIT.
        for (auto &function : m_) 
        {      
            function_pass_manager->run(function);
        }        
    });

    return std::move(module_);
}

Jit::Jit() : m_execution_session{std::make_unique<llvm::orc::ExecutionSession>(std::move(cantFail(llvm::orc::SelfExecutorProcessControl::Create())))},  
    m_jit_dylib{m_execution_session->createBareJITDylib("main")}
{
    llvm::orc::JITTargetMachineBuilder target_machine_builder{m_execution_session->getExecutorProcessControl().getTargetTriple()};

    m_data_layout = std::make_unique<llvm::DataLayout>(cantFail(target_machine_builder.getDefaultDataLayoutForTarget()));
    m_mangler = std::make_unique<llvm::orc::MangleAndInterner>(*m_execution_session, *m_data_layout);   
    m_object_layer = std::make_unique<llvm::orc::RTDyldObjectLinkingLayer>(*m_execution_session, []() { return std::make_unique<llvm::SectionMemoryManager>(); });
    m_dump_layer = std::make_unique<llvm::orc::ObjectTransformLayer>( *m_execution_session, *m_object_layer, llvm::orc::DumpObjects("D:/tmp", "out" ));
    m_compile_layer = std::make_unique<llvm::orc::IRCompileLayer>(*m_execution_session, *m_dump_layer, std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(target_machine_builder)));
    // https://www.llvm.org/docs/ORCv2.html#how-to-add-process-and-library-symbols-to-jitdylibs
    // m_jit_dylib.addGenerator(
    //     cantFail(DynamicLibrarySearchGenerator::GetForCurrentProcess(
    //         DL.getGlobalPrefix())));
  
    m_object_layer->setOverrideObjectFlagsWithResponsibilityFlags(true); // needed for windows    

    m_optimize_layer = std::make_unique<llvm::orc::IRTransformLayer>(*m_execution_session, *m_compile_layer, optimizeModule);
}

Jit::~Jit() 
{
    if (auto error = m_execution_session->endSession()) 
    {
        m_execution_session->reportError(std::move(error));
    }
}

void Jit::apply_external_symbols( External_symbols* symbol_map_  ) const
{
    llvm::orc::SymbolMap symbols;
    for ( const auto& [name, function] : symbol_map_->data() ) 
    {
        symbols.try_emplace( (*m_mangler)(name), llvm::orc::ExecutorSymbolDef(llvm::orc::ExecutorAddr::fromPtr(function), {}) );
    }
    throw_on_error(m_jit_dylib.define( llvm::orc::absoluteSymbols(symbols)));
}

Heaper<llvm::orc::ResourceTrackerSP>* Jit::add_thread_safe_module( Compile_result* result_ ) const
{

    llvm::orc::ThreadSafeModule module {std::move(result_->module), std::move(result_->context)};
    auto ressource_tracker = m_jit_dylib.createResourceTracker();
    if (auto E = m_optimize_layer->add(ressource_tracker, std::move(module))) 
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

void* Jit::lookup( const char* name_ ) const
{
    llvm::Expected<llvm::orc::ExecutorSymbolDef> x = m_execution_session->lookup({&m_jit_dylib}, (*m_mangler)(name_));
    throw_on_error(x.takeError());
    return x.get().getAddress().toPtr<void*>();
}
           
void External_symbols::add_external( const char* name_, void* ptr_ )
{
    m_external_symbols.try_emplace( name_, ptr_);
}
        
const std::unordered_map<std::string, void*>& External_symbols::data() 
{
    return m_external_symbols;
}