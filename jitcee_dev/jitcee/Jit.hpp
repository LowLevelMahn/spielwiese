#pragma once

#include <memory>
#include "common.hpp"
#include <llvm/ExecutionEngine/Orc/Core.h>

namespace llvm 
{
    template<typename T>
    class IntrusiveRefCntPtr;

    class DataLayout;

    namespace orc
    {
        class ResourceTracker;
        using ResourceTrackerSP = IntrusiveRefCntPtr<ResourceTracker>;

        class ThreadSafeModule;
        class ExecutorSymbolDef;
        class ExecutionSession;
        class MangleAndInterner;
        class RTDyldObjectLinkingLayer;
        class IRCompileLayer;
        class JITDylib;
    }
}

namespace jitcee {

    struct Compile_result;
    class External_symbols;

    class Jit {
        
    public:
        Jit();
         ~Jit();

        void apply_external_symbols( External_symbols* symbol_map_ ) const;
        Heaper<llvm::orc::ResourceTrackerSP>* add_thread_safe_module( Compile_result* result_ ) const;
        std::string dump() const;
        const llvm::orc::ExecutorSymbolDef& lookup( const char* name_ ) const;
        llvm::orc::MangleAndInterner* get_mangler() const;

      private:
          std::unique_ptr<llvm::orc::ExecutionSession> m_execution_session;

          std::unique_ptr<llvm::DataLayout> m_data_layout;
          std::unique_ptr<llvm::orc::MangleAndInterner> m_mangler;

          std::unique_ptr<llvm::orc::RTDyldObjectLinkingLayer> m_object_layer;
          std::unique_ptr<llvm::orc::IRCompileLayer> m_compile_layer;

          llvm::orc::JITDylib& m_jit_dylib;
    };

    class External_symbols
    {
        public:
        External_symbols( llvm::orc::MangleAndInterner* mangler_ );

        void add_external( const char* name_, void* ptr_ );
        const llvm::orc::SymbolMap& data();

    private:
        llvm::orc::SymbolMap m_external_symbols;
        llvm::orc::MangleAndInterner* m_mangler{}; 
    };
} // namespace jitcee