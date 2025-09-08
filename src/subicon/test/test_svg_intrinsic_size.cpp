#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <stdexcept>

#include <fmt/format.h>

#include <pugixml.hpp>

#include <ka/subicon/svg.hpp>

#include "load_xml.hpp"

namespace ka::subicon::test
{

using ::testing::ElementsAreArray;

TEST(SvgIntrinsiocSizeTest, test_empty_atributes)
{
    const auto xml = load_xml(R"(
        <svg>
        </svg>
    )");
    const auto svg = xml.document_element();

    const auto intrinsic_size = svg_intrinsic_size(svg);
    EXPECT_FALSE(intrinsic_size.has_value());
}

TEST(SvgIntrinsiocSizeTest, test_width_only)
{
    const auto xml = load_xml(R"(
        <svg width="10">
        </svg>
    )");
    const auto svg = xml.document_element();

    const auto intrinsic_size = svg_intrinsic_size(svg);
    EXPECT_FALSE(intrinsic_size.has_value());
}

TEST(SvgIntrinsiocSizeTest, test_height_only)
{
    const auto xml = load_xml(R"(
        <svg width="10">
        </svg>
    )");
    const auto svg = xml.document_element();

    const auto intrinsic_size = svg_intrinsic_size(svg);
    EXPECT_FALSE(intrinsic_size.has_value());
}

TEST(SvgIntrinsiocSizeTest, test_percents)
{
    const auto xml = load_xml(R"(
        <svg width="10%" height="100%" viewBox="0 0 10 100">
        </svg>
    )");
    const auto svg = xml.document_element();

    const auto intrinsic_size = svg_intrinsic_size(svg);
    EXPECT_FALSE(intrinsic_size.has_value());
}

TEST(SvgIntrinsiocSizeTest, test_unknown_units)
{
    const auto xml = load_xml(R"(
        <svg width="10km" height="100km" viewBox="0 0 10 100">
        </svg>
    )");
    const auto svg = xml.document_element();

    EXPECT_THROW(std::ignore = svg_intrinsic_size(svg), std::runtime_error);
}

TEST(SvgIntrinsiocSizeTest, test_unsupported_units)
{
    const auto xml = load_xml(R"(
        <svg width="10em" height="100em" viewBox="0 0 10 100">
        </svg>
    )");
    const auto svg = xml.document_element();

    const auto intrinsic_size = svg_intrinsic_size(svg);
    EXPECT_FALSE(intrinsic_size.has_value());
}

TEST(SvgIntrinsiocSizeTest, test_invalid_length)
{
    const auto xml = load_xml(R"(
        <svg width="1 2" height="2">
        </svg>
    )");
    const auto svg = xml.document_element();

    EXPECT_THROW(std::ignore = svg_intrinsic_size(svg), std::runtime_error);
}

TEST(SvgIntrinsiocSizeTest, test_width_height_viewbox)
{
    const auto xml = load_xml(R"(
        <svg width="10px" height="100" viewBox="0 0 42 37">
        </svg>
    )");
    const auto svg = xml.document_element();

    const auto intrinsic_size = svg_intrinsic_size(svg);
    ASSERT_TRUE(intrinsic_size.has_value());
    EXPECT_EQ(10, intrinsic_size->width);
    EXPECT_EQ(100, intrinsic_size->height);
}

TEST(SvgIntrinsiocSizeTest, test_viewbox)
{
    const auto xml = load_xml(R"(
        <svg viewBox="0 0 42 37">
        </svg>
    )");
    const auto svg = xml.document_element();

    const auto intrinsic_size = svg_intrinsic_size(svg);
    ASSERT_TRUE(intrinsic_size.has_value());
    EXPECT_EQ(42, intrinsic_size->width);
    EXPECT_EQ(37, intrinsic_size->height);
}

TEST(SvgIntrinsiocSizeTest, test_invalid_too_short_viewbox)
{
    const auto xml = load_xml(R"(
        <svg viewBox="0 0 42">
        </svg>
    )");
    const auto svg = xml.document_element();

    EXPECT_THROW(std::ignore = svg_intrinsic_size(svg), std::runtime_error);
}

TEST(SvgIntrinsiocSizeTest, test_invalid_too_long_viewbox)
{
    const auto xml = load_xml(R"(
        <svg viewBox="1 2 3 4 5">
        </svg>
    )");
    const auto svg = xml.document_element();

    EXPECT_THROW(std::ignore = svg_intrinsic_size(svg), std::runtime_error);
}

} // namespace ka::subicon::test
