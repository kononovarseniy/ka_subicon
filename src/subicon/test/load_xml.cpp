#include "load_xml.hpp"

#include <stdexcept>

#include <fmt/format.h>

#include <pugixml.hpp>

namespace ka::subicon::test
{

[[nodiscard]] pugi::xml_document load_xml(const std::string_view xml)
{
    pugi::xml_document document;
    const auto result = document.load_buffer(xml.data(), xml.size());
    if (result.status != pugi::status_ok)
    {
        throw std::runtime_error(fmt::format("Failed to parse XML: {}", result.description()));
    }
    return document;
}

} // namespace ka::subicon::test