#pragma once

#ifndef __SERDEPP_ATTRIBUTE_MAKE_OPTIONAL_HPP__
#define __SERDEPP_ATTRIBUTE_MAKE_OPTIONAL_HPP__

#include "serdepp/serializer.hpp"

namespace serde::attribute {
    namespace detail {
        struct make_optional {
            template <typename T, typename serde_ctx, typename Next, typename... Attributes>
            constexpr inline void from(serde_ctx& ctx, T& data, std::string_view key,
                                       Next&& next_attr, Attributes&& ...remains) const{
                using Helper = serde_adaptor_helper<typename serde_ctx::Adaptor>;
                if constexpr (meta::is_emptyable_v<T>) {
                    if (Helper::is_null(ctx.adaptor, key)) {
                        if (data.empty()) data = std::move(T{});
                    } else {
                        next_attr.template from<T, serde_ctx>(ctx, data, key, remains...);
                    }
                } else {
                    throw serde::attribute_error(fmt::format("this {} is not emptyable type",
                                                             nameof::nameof_short_type<T>()));
                }
            }

            template <typename T, typename serde_ctx, typename Next, typename... Attributes>
            constexpr inline void into(serde_ctx& ctx, T& data, std::string_view key,
                                       Next&& next_attr, Attributes&& ...remains) const{
                if (!data.empty()) {
                    next_attr.template into<T, serde_ctx>(ctx, data, key, remains...);
                }
            }
        };
    }
    
    inline constexpr static detail::make_optional make_optional = detail::make_optional{};

}


#endif