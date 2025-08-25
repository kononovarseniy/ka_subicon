set(module_name subicon)

add_library(ka_${module_name})
add_library(ka::${module_name} ALIAS ka_${module_name})

target_sources(ka_${module_name}
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
        include/ka/subicon/subicon.hpp

    PRIVATE
        src/subicon.cpp
)

# target_enable_warnings(ka_${module_name})

# target_link_libraries(ka_${module_name}
#     PUBLIC
#         ka::common
# )

install(TARGETS ka_${module_name}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    FILE_SET HEADERS DESTINATION include
)

if(BUILD_TESTS)
    find_package(fmt REQUIRED)

    add_executable(ka_${module_name}_test
        test/test.cpp
    )
    # target_enable_warnings(ka_${module_name}_test)
    target_link_libraries(ka_${module_name}_test
        PRIVATE
            # ka::common
            ka::subicon
            fmt::fmt
            gtest::gtest
    )

    gtest_discover_tests(ka_${module_name}_test)
endif()
