
#include <concepts>
#include <cstdlib>
#include <iterator>
#include <ranges>
#include <string_view>
#include <utility>

#include <fmt/format.h>

#include <pugixml.hpp>

#include <ka/subicon/svg.hpp>

namespace ka::subicon
{

inline namespace
{

using ZeroTerminatedString = const char *;

[[nodiscard]] ZeroTerminatedString skip_white_space(ZeroTerminatedString str)
{
    while (std::isspace(*str))
    {
        ++str;
    }
    return str;
}

[[nodiscard]] ZeroTerminatedString skip_comma_white_space(ZeroTerminatedString str)
{
    while (*str == ',' || std::isspace(*str))
    {
        ++str;
    }
    return str;
}

[[nodiscard]] std::pair<double, ZeroTerminatedString> parse_svg_float(ZeroTerminatedString str)
{
    char * end;
    const auto number = std::strtod(str, &end);
    if (end == str)
    {
        throw std::runtime_error(fmt::format("Invalid SVG number: '{}'", str));
    }
    if (number == HUGE_VAL)
    {
        throw std::runtime_error(fmt::format("SVG number overflow: '{}'", str));
    }
    return { number, end };
}

template <std::output_iterator<double> Out>
std::pair<const char *, Out> parse_svg_float_list(const char * const str, size_t max_out, Out out)
{
    const char * ptr = str;
    const char * end;
    do
    {
        if (max_out == 0)
        {
            break;
        }
        --max_out;

        ptr = skip_comma_white_space(ptr);

        const auto [number, end] = parse_svg_float(ptr);
        ptr = end;

        *out = number;
        ++out;
    } while (*ptr != 0);
    return { ptr, out };
}

struct ViewBox
{
    double x;
    double y;
    double width;
    double height;
};

[[nodiscard]] ViewBox parse_svg_viewbox(const char * const str)
{
    std::array<double, 4> numbers;
    auto [end, it] = parse_svg_float_list(str, numbers.size(), numbers.begin());
    if (*skip_white_space(end) != 0 || it != numbers.end())
    {
        throw std::runtime_error(fmt::format("Failed to parse svg viewBox attribute with value '{}'", str));
    }
    return {
        .x = numbers[0],
        .y = numbers[1],
        .width = numbers[2],
        .height = numbers[3],
    };
}

enum class SvgLengthUnit
{
    User,
    Em,
    Ex,
    Pixel,
    Inch,
    Centimeter,
    Millimeter,
    Point,
    Pica,
    Percent,
};

struct SvgLength
{
    double value;
    SvgLengthUnit unit;
};

[[nodiscard]] SvgLength parse_svg_length(const char * const str)
{
    using namespace std::string_view_literals;

    const auto [number, unit_str] = parse_svg_float(str);
    // TODO: Handle trainling whitespace.

    if (unit_str == ""sv)
    {
        return { number, SvgLengthUnit::User };
    }
    if (unit_str == "px"sv)
    {
        return { number, SvgLengthUnit::Pixel };
    }
    if (unit_str == "%"sv)
    {
        return { number, SvgLengthUnit::Percent };
    }

    if (unit_str == "em"sv)
    {
        return { number, SvgLengthUnit::Em };
    }
    if (unit_str == "ex"sv)
    {
        return { number, SvgLengthUnit::Ex };
    }
    if (unit_str == "in"sv)
    {
        return { number, SvgLengthUnit::Inch };
    }
    if (unit_str == "cm"sv)
    {
        return { number, SvgLengthUnit::Centimeter };
    }
    if (unit_str == "mm"sv)
    {
        return { number, SvgLengthUnit::Millimeter };
    }
    if (unit_str == "pt"sv)
    {
        return { number, SvgLengthUnit::Point };
    }
    if (unit_str == "pc"sv)
    {
        return { number, SvgLengthUnit::Pica };
    }

    throw std::runtime_error(fmt::format("Unknown SVG length unit '{}'", unit_str));
}

} // namespace

[[nodiscard]] std::optional<SvgIntrinsicSize> svg_intrinsic_size(const pugi::xml_node svg_node)
{
    const auto width_attr = svg_node.attribute("width");
    const auto height_attr = svg_node.attribute("height");
    if (!width_attr.empty() && !height_attr.empty())
    {
        const auto width = parse_svg_length(width_attr.value());
        const auto height = parse_svg_length(height_attr.value());
        const auto width_supported_units = width.unit == SvgLengthUnit::User || width.unit == SvgLengthUnit::Pixel;
        const auto height_supported_units = height.unit == SvgLengthUnit::User || height.unit == SvgLengthUnit::Pixel;
        if (width_supported_units && height_supported_units)
        {
            return SvgIntrinsicSize { width.value, height.value };
        }
        // TODO: Поддержать другие единицы.
        return std::nullopt;
    }

    const auto view_box_attr = svg_node.attribute("viewBox");
    if (!view_box_attr.empty())
    {
        const auto viewbox = parse_svg_viewbox(view_box_attr.value());
        return SvgIntrinsicSize { viewbox.width, viewbox.height };
    }
    return std::nullopt;
}

} // namespace ka::subicon
