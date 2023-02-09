add_library(wdtls_common STATIC
        wad.h
        wad.c
        list.h
        list.c)

target_link_libraries(wdtls_common PRIVATE)