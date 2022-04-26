#pragma once

#ifndef __CPPM_CMAKE_SCRIPT_HPP__
#define __CPPM_CMAKE_SCRIPT_HPP__

#include <serdepp/serde.hpp>
#include <serdepp/adaptor/fmt.hpp>
#include <range/v3/all.hpp>
#include <unordered_set>

namespace cppm::cmake {
    struct exception : std::exception {
        explicit exception() {}
        virtual ~exception() noexcept override = default;
        virtual const char* what() const noexcept override {return "";}
    };

    struct cmake_error :exception {
        explicit cmake_error(std::string what) : what_(fmt::format("cmake_error: {}\n", what)) {}
        virtual ~cmake_error() noexcept override = default;
        virtual const char* what() const noexcept override {return what_.c_str();}
    protected:
        std::string what_;
    };

    class FunctionBuilder;

    struct Function {
        DERIVE_SERDE(Function,
                     (&Self::name, "name")
                     (&Self::args, "args")
                     (&Self::flags, "flags")
                     (&Self::named_0_args, "named_0_args")
                     (&Self::named_n_args, "named_n_args"))
        Function(std::string_view name_,
                 std::vector<std::string_view> args_ = {},
                 std::unordered_set<std::string_view> flags_ = {},
                 std::unordered_set<std::string_view> named_0_args_ = {},
                 std::unordered_set<std::string_view> named_n_args_ = {})

        : name(name_), args(std::move(args_)), flags(std::move(flags_)),
          named_0_args(std::move(named_0_args_)), named_n_args(std::move(named_n_args_))
        {}

        FunctionBuilder operator()() const;
        FunctionBuilder operator()(std::string_view args) const;
        
        std::string_view name;
        std::vector<std::string_view> args;
        std::unordered_set<std::string_view> flags;
        std::unordered_set<std::string_view> named_0_args;
        std::unordered_set<std::string_view> named_n_args;
    };

    class FunctionBuilder{
    public:
        FunctionBuilder(const Function& define) : define_(define) {}
        FunctionBuilder& arg(const std::string &arg) {
            if(!define_.args.empty() && define_.args.size() == args_.size()) throw cmake_error("too many args");
            args_.push_back(arg);
            return *this;
        }
        FunctionBuilder& flag(const std::string& flag, bool pass=true) {
            using namespace fmt::literals;
            if(!pass) return *this;
            if(define_.flags.find(flag) == define_.flags.end()) 
                throw cmake_error("undefined named flag: {} {}"_format(define_.name, flag));
            args_.push_back(flag);
            return *this;
        }

        FunctionBuilder& flag(const std::string& flag, std::string value, bool pass=true) {
            using namespace fmt::literals;
            if(!pass) return *this;
            if(define_.named_0_args.find(flag) == define_.named_0_args.end())
                if(define_.named_n_args.find(flag) == define_.named_n_args.end())
                    throw cmake_error("undefined named flag: {} {}"_format(define_.name, flag));
            args_.push_back("{} {}"_format(flag, value));
            return *this;
        }


        FunctionBuilder& flag(const std::string& flag, std::optional<std::string> value) {
            using namespace fmt::literals;
            if(!value) return *this;
            if(define_.named_0_args.find(flag) == define_.named_0_args.end())
                if(define_.named_n_args.find(flag) == define_.named_n_args.end())
                    throw cmake_error("undefined named flag: {} {}"_format(define_.name, flag));
            args_.push_back("{} {}"_format(flag, *value));
            return *this;
        }


        FunctionBuilder& flag(const std::string& flag, std::vector<std::string> value, bool pass=true) {
            using namespace fmt::literals;
            if(!pass) return *this;
            if(define_.named_0_args.find(flag) == define_.named_0_args.end() && value.size() != 1) {
                if(define_.named_n_args.find(flag) == define_.named_n_args.end()) {
                    throw cmake_error("undefined named flag: {} {}"_format(define_.name, flag));
                }
            }
            else if(define_.named_n_args.find(flag) == define_.named_n_args.end()) {
                throw cmake_error("undefined named flag: {} {}"_format(define_.name, flag));
            }
            args_.push_back("{} {}"_format(flag, fmt::join(value," ")));
            return *this;
        }

        std::string build() const{
            using namespace fmt::literals;
            std::string depth_space="";
            for(int i =0; i < depth; ++i) depth_space += "    ";
            return "{}{}({})\n"_format(depth_space, define_.name, fmt::join(args_, " "));
        }
        std::string operator()() { return build(); }
        inline FunctionBuilder& set_depth(int depth_) { depth=depth_; return *this; }
        operator std::string() { return build(); }
    private:
        const Function& define_;
        std::vector<std::string> args_;
        int depth=0;
    };

    FunctionBuilder Function::operator()() const { return FunctionBuilder(*this); }

    FunctionBuilder Function::operator() (std::string_view args) const {
        return FunctionBuilder(*this).arg(std::string{args});
    }

    struct block {
        template <typename... Builder>
        block(Builder&&... builders) : depth(0) {
            (elements.push_back(std::forward<Builder>(builders)), ...);
        }
        std::string operator()() const {
            using namespace fmt::literals;
            std::string script="";
            for(auto& e : elements) {
                std::visit([&](auto et){
                    if constexpr(std::is_same_v<decltype(et), std::tuple<FunctionBuilder, block, FunctionBuilder>>) {
                        auto& [f, b, e] = et;
                        script += f.set_depth(depth+1);
                        script += b.set_depth(depth+1);
                        script += e.set_depth(depth+1);
                    }
                    else if constexpr(std::is_same_v<decltype(et), std::vector<FunctionBuilder>>) {
                        for(auto& func : et) { script += func.set_depth(depth+1); }
                    }
                    else if constexpr(std::is_same_v<decltype(et), std::string>) {
                        script += et;
                    }
                    else {
                        script += et.set_depth(depth+1);
                    }
                }, e);
            }
            return "{}"_format(script);
        }
        operator std::string() const { return this->operator()(); }

        std::vector<std::variant<std::string,
                                 block,
                                 FunctionBuilder,
                                 std::vector<FunctionBuilder>,
                                 std::tuple<FunctionBuilder, block, FunctionBuilder>>> elements;
        inline block& set_depth(int depth_) { depth=depth_; return *this; }
        int depth;
    };

    const static std::string space{"\n"};
    static const auto _include = Function("include", {"path"});
    static const auto _if = Function("if");
    static const auto _elseif = Function("elseif");
    static const auto _else = Function("else");
    static const auto _endif = Function("endif");
    static const auto set = Function("set", {"name", "value"});
    static const auto cmake_minimum_required = Function("cmake_minimum_required", {}, {}, {"VERSION"});
    static const auto project = Function("project", {"name"}, {}, {"VERSION"}, {"LANGUAGES"});
    static const auto cppm_project = Function("cppm_project", {}, {"WITH_VCPKG"});
    static const auto cppm_setting = Function("cppm_setting", {}, {"UNITY_BUILD"});
    static const auto cppm_cxx_standard = Function("cppm_cxx_standart", {"ver"});
    static const auto find_cppkg =
        Function("find_cppkg",
                {"name", "version"},
                {"HUNTER"},
                {"TYPE", "OPTIONAL"},
                {"MODULE","COMPONENTS", "LOADPATH"});

    inline std::string var(std::string_view name) {
        return fmt::format("${{{}}}",name);
    }

    
    auto if_block(std::string_view flag, cmake::block&& child)
        -> std::tuple<FunctionBuilder, block, FunctionBuilder> {
        return std::make_tuple(_if(flag), std::forward<block>(child), _endif());
    }

    auto scope(FunctionBuilder&& start, cmake::block&& child, FunctionBuilder&& end)
        -> std::tuple<FunctionBuilder, block, FunctionBuilder> {
        return std::make_tuple(std::forward<FunctionBuilder>(start),
                               std::forward<block>(child),
                               std::forward<FunctionBuilder>(end));
    }

    using CMakeScript = block;
};

//namespace serde {
//    template<> struct serde_adaptor_helper<cppm::cmake::Function>: derive_serde_adaptor_helper<cppm::cmake::Function> {
//        inline constexpr static bool is_null(cppm::cmake::Function& adaptor, std::string_view key) { return false; }
//        inline constexpr static size_t size(cppm::cmake::Function& adaptor) { return 1; }
//        inline constexpr static bool is_struct(cppm::cmake::Function& adaptor) { return true; }
//    };
//
//    template<typename T, typename U> struct serde_adaptor<cppm::cmake::Function, T, U> {
//        static void from(cppm::cmake::Function& s, std::string_view key, T& data){
//            throw serde::unimplemented_error(fmt::format("serde_adaptor<{}>::from(cppm::cmake::Function, key data)",
//                                                         nameof::nameof_short_type<cppm::cmake::Function>()));
//        }
//        //static void into(cppm::cmake::Function& s, std::string_view key, const T& data) { s.add(key, data); }
//    };
//
//    template<typename... T>
//    struct serde_adaptor<cppm::cmake::Function, std::variant<T...>>  {
//        constexpr static void from(cppm::cmake::Function& s, std::string_view key, std::variant<T...>& data) {
//            throw serde::unimplemented_error(fmt::format("serde_adaptor<{}>::from(cppm::cmake::Function, key data)",
//                                                         nameof::nameof_short_type<cppm::cmake::Function>()));
//        }
//        constexpr static void into(cppm::cmake::Function& s, std::string_view key, const std::variant<T...>& data) {
//            std::visit([&](auto& type){ serialize_to<cppm::cmake::Function>(type, s, key); }, data);
//        }
//    };
//}


template<>
struct fmt::formatter<cppm::cmake::FunctionBuilder> : fmt::formatter<std::string> {
    template <typename format_ctx>
    auto format(const cppm::cmake::FunctionBuilder& func_builder, format_ctx& ctx) {
        return formatter<std::string>::format(func_builder.build(), ctx);
    }
};

template<>
struct fmt::formatter<cppm::cmake::block> : fmt::formatter<std::string> {
    template <typename format_ctx>
    auto format(const cppm::cmake::block& func_builder, format_ctx& ctx) {
        return formatter<std::string>::format(func_builder(), ctx);
    }
};

template<>
struct fmt::formatter<cppm::cmake::Function> : fmt::formatter<std::string> {
    template <typename format_ctx>
    auto format(const cppm::cmake::Function& func_builder, format_ctx& ctx) {
        return formatter<std::string>::format(func_builder(), ctx);
    }
};

#endif
