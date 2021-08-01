#pragma once

#ifndef __CPPM_CORE_CPPKG_HPP__
#define __CPPM_CORE_CPPKG_HPP__

#include <serdepp/utility.hpp>
#include <cppm/core/dependency.hpp>

namespace cppm::core {
    enum class cppkg_type {
        bin,
        lib,
        test,
        example,
        cmake,
    };
    enum class cppkg_type_detail {
        STATIC,
        BINARY,
        SHARED,
        HEADER_ONLY,
    };

    struct Cppkg {
        DERIVE_SERDE(Cppkg,
                     (&Self::name, "name")
                     (&Self::namespace_, "namespace")
                     (&Self::install,    "install", default_{true})
                     (&Self::source,     "source",  make_optional)
                     (&Self::exclude,    "flag",    default_{false});
        )

        std::string name;
        cppkg_type type; 
        cppkg_type_detail cppkg_type_d;
        std::optional<std::string> namespace_;
        bool install;
        std::vector<std::string> source;
        std::string exclude_var;
        bool exclude;

        virtual ~Cppkg() = default;
    };



    struct CppkgLib : Cppkg {
        template<class Context>
        constexpr static void serde(Context& context, CppkgLib& value) {
            using namespace serde::attribute;
            using Self = CppkgLib;
            Cppkg& upcast = static_cast<Cppkg&>(value);
            Cppkg::serde(context, upcast);
            serde::serde_struct(context, value)
                (&Self::type, "cppkg_type", default_{cppkg_type::lib})
                (&Self::cppkg_type_d, "type", default_{cppkg_type_detail::STATIC}, under_to_dash, to_lower)
                ;
        }
    };
    struct CppkgBin : Cppkg {
        template<class Context>
        constexpr static void serde(Context& context, CppkgBin& value) {
            using namespace serde::attribute;
            using Self = CppkgBin;
            Cppkg& upcast = static_cast<Cppkg&>(value);
            Cppkg::serde(context, upcast);
            serde::serde_struct(context, value)
                (&Self::type, "cppkg_type", default_{cppkg_type::bin})
                (&Self::cppkg_type_d, "type", default_{cppkg_type_detail::BINARY}, under_to_dash, to_lower)
                ;
        }
    };
    struct CppkgExample : Cppkg {
        template<class Context>
        constexpr static void serde(Context& context, CppkgExample& value) {
            using namespace serde::attribute;
            using Self = CppkgExample;
            Cppkg& upcast = static_cast<Cppkg&>(value);
            Cppkg::serde(context, upcast);
            serde::serde_struct(context, value)
                (&Self::type, "cppkg_type", default_{cppkg_type::example})
                (&Self::cppkg_type_d, "type", default_{cppkg_type_detail::BINARY}, under_to_dash, to_lower)
                ;
        }
    };

    struct CppkgTest : Cppkg {
        template<class Context>
        constexpr static void serde(Context& context, CppkgTest& value) {
            using namespace serde::attribute;
            using Self = CppkgTest;
            Cppkg& upcast = static_cast<Cppkg&>(value);
            Cppkg::serde(context, upcast);
            serde::serde_struct(context, value)
                (&Self::type, "cppkg_type", default_{cppkg_type::test})
                (&Self::cppkg_type_d, "type", default_{cppkg_type_detail::BINARY}, under_to_dash, to_lower)
                ;
        }
    };
}

//namespace serde {
//    template<typename T, typename serde_ctx>
//    struct serde_serializer<T, serde_ctx,
//                            std::enable_if_t<std::is_base_of_v<cppm::core::Cppkg, T> &&
//                                             meta::is_serdeable_v<serde_ctx, T>>> {
//        inline static auto from(serde_ctx& ctx, T& data, std::string_view key) {
//            using namespace cppm::core;
//            fmt::print("here");
//            auto upper = static_cast<Cppkg&>(data);
//            deserialize_to(ctx.adaptor, key, upper);
//            deserialize_to(ctx.adaptor, key, data);
//            //serde_adaptor<typename serde_ctx::Adaptor, Cppkg>::from(ctx.adaptor, key, static_cast<Cppkg&>(data));
//            //serde_adaptor<typename serde_ctx::Adaptor, T>::from(ctx.adaptor, key, data);
//        }
//
//        // deserialize step
//        inline static auto into(serde_ctx& ctx, const T& data, std::string_view key) {
//            using namespace cppm::core;
//            fmt::print("here");
//            auto upper = static_cast<Cppkg&>(data);
//            serialize_to(ctx.adaptor, key, upper);
//            serialize_to(ctx.adaptor, key, data);
//            //serde_adaptor<typename serde_ctx::Adaptor, Cppkg>::into(ctx.adaptor, key, static_cast<Cppkg&>(data));
//            //serde_adaptor<typename serde_ctx::Adaptor, T>::into(ctx.adaptor, key, data);
//        }
//    };
//}

#endif
