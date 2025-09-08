#pragma once

#include <string_view>

#include <pugixml.hpp>

namespace ka::subicon::test
{

[[nodiscard]] pugi::xml_document load_xml(const std::string_view xml);

} // namespace ka::subicon::test
