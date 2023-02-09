add_executable(wdtls_console)

target_sources(wdtls_console PRIVATE
        wdtls.c)

target_link_libraries(wdtls_console PRIVATE
        wdtls_common)

set_target_properties(wdtls_console PROPERTIES OUTPUT_NAME wdtls)