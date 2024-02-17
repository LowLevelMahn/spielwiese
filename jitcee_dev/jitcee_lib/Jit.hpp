#pragma once

#include "common.hpp"
#include <vector>

class Dll_lib;

namespace jitcee {

    class Compile_result;

    struct External_symbol 
    {
        std::string name;
        void* function_ptr{};
    };

    class Jit {        
        public:
            ~Jit();

            void set_external_symbols(const std::vector<External_symbol>& symbols_);

            void add_compile_result( const Compile_result& result_ );

            template<typename T>
            T* lookup( const std::string& name_ ) const 
            {
                return static_cast<T*>( lookup( name_ ) );
            }

        private:
            friend class Jitcee;
        
            explicit Jit( const Dll_lib& handle_ );
            Jit(Jit const&) = delete;
            Jit& operator=(Jit const&) = delete;

            void* lookup(const std::string& name_) const;

            f_ptr* m_jitcee_resource_tracker_free{};
            f_ptr* m_jitcee_jit_free{};
            f_ptr_ptr_ptr2* m_jitcee_jit_add_thread_safe_module{};
            f_ptr2* m_jitcee_jit_create{};
            f_ptr* m_jitcee_external_symbols_create{};
            f_ptr_char_ptr* m_jitcee_external_symbols_add{};
            f_ptr_ptr* m_jitcee_jit_apply_external_symbols{};
            f_ptr* m_jitcee_external_symbols_free{};
            f_ptr_char_ptr2* m_jitcee_jit_lookup{};

            void* m_jit_handle{};
            std::vector<void*> m_resource_tracker_handles{};
    };

} // namespace jitcee
