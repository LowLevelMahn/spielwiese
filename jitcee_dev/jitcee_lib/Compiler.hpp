#pragma once

#include "common.hpp"

class Dll_lib;

namespace jitcee {
    
    class Compile_result
    {
        public:
         ~Compile_result();

        private:
            friend class Compiler;
            friend class Jit;

            Compile_result(const Dll_lib& handle_, void* compile_result_handle_);

            Compile_result(Compile_result const&) = delete;
            Compile_result& operator=(Compile_result const&) = delete;

            f_ptr* m_jitcee_compile_result_free{};
            void* m_compile_result_handle{};
    };

    class Compiler_instance
    {
        public:     
            ~Compiler_instance();

            enum class Optimization_level
            {
                O0 = 0,
                O1 = 1,
                O2 = 2,
                O3 = 3
            };

            enum class Inlining_method
            {
                NormalInlining = 0,  // Use the standard function inlining pass.
                OnlyHintInlining = 1, // Inline only (implicitly) hinted functions.
                OnlyAlwaysInlining = 2,  // Only run the always inlining pass.
            };

            void optimization_level( Optimization_level optimization_level_ ) const;
            void inlining( Inlining_method inlining_method_ ) const;

        private:
            friend class Compiler;

            Compiler_instance( const Dll_lib& handle_, void* compiler_instance_handle_ );
            Compiler_instance(Compiler_instance const&) = delete;
            Compiler_instance& operator=(Compiler_instance const&) = delete;

            f_ptr* m_jitcee_compiler_instance_free{};
            f_ptr_int* m_jitcee_compile_options_set_optimization_level{};
            f_ptr_int* m_jitcee_compile_options_set_inlining{};
            void* m_compiler_instance_handle{};
    };

    class Compiler {
        public:
            ~Compiler();

            void diagnostic_color(bool show_color_) const;
            Compile_result compile( const Compiler_instance& compiler_instance_, const std::string& code_) const;
            Compiler_instance create_instance() const;

        private:
            friend class Jitcee;

            explicit Compiler(const Dll_lib& handle_);           
            Compiler(Compiler const&) = delete;
            Compiler& operator=(Compiler const&) = delete;

            const Dll_lib& m_dll_handle;
            f_ptr2* m_jitcee_compiler_create{};
            f_ptr* m_jitcee_compiler_free{};
            f_ptr_int* m_jitcee_compiler_diagnostic_option_show_color_set{};
            f_ptr_ptr_char_ptr2* m_jitcee_compiler_compile{};
            f_ptr_ptr2* m_jitcee_compiler_instance_create{};
            void* m_compiler_handle{};
    };
}  // namespace jitcee
