#pragma once

#include <optional>
#include <string_view>

#include <pugixml.hpp>

namespace ka::subicon
{

struct SvgIntrinsicSize
{
    double width;
    double height;
};

[[nodiscard]] std::optional<SvgIntrinsicSize> svg_intrinsic_size(const pugi::xml_node svg_node);

} // namespace ka::subicon
