set(LV_BUILD_TYPE SHARED)

if (MSVC)
    set(LV_BUILD_TYPE STATIC)
endif()

file(GLOB_RECURSE LV_LIB_HEADERS
    ${PROJECT_SOURCE_DIR}/src/*.h
    )
file(GLOB_RECURSE LV_LIB_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.c
    )

add_library(lvgl ${LV_BUILD_TYPE}
    ${LV_LIB_HEADERS}
    ${LV_LIB_SOURCES}
    )

install(TARGETS lvgl
    DESTINATION  ${TARGET_SYS_PATH}/usr/lib
    )
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/
    DESTINATION ${TARGET_SYS_PATH}/usr/include
    FILES_MATCHING PATTERN "*.h"
    PATTERN "lv_drivers" EXCLUDE
    )
