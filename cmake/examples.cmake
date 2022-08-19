macro(subdirlist ParamOutResultList ParamInTargetDir)
    file(GLOB children RELATIVE ${ParamInTargetDir} ${ParamInTargetDir}/*)
    set(dirlist "")
    foreach(child ${children})
    if(IS_DIRECTORY ${ParamInTargetDir}/${child})
    list(APPEND dirlist ${child})
    endif()
    endforeach()
    set(${ParamOutResultList} ${dirlist})
endmacro(subdirlist)

set(EXAMPLES_DIR ${PROJECT_SOURCE_DIR}/examples)

subdirlist(LV_EXAMPLES ${EXAMPLES_DIR})
foreach(EX_NAME ${LV_EXAMPLES})
    file(GLOB_RECURSE LV_EX_HEADERS
        ${EXAMPLES_DIR}/${EX_NAME}/*.h
        )
    file(GLOB_RECURSE LV_EX_SOURCES
        ${EXAMPLES_DIR}/${EX_NAME}/*.c
        )
    add_executable(${EX_NAME}
        ${LV_EX_HEADERS}
        ${LV_EX_SOURCES}
        )
    target_include_directories(${EX_NAME}
        PRIVATE
        ${EXAMPLES_DIR}/${EX_NAME}
        )
    target_link_libraries(${EX_NAME}
        lvgl
        )
    set_target_properties(${EX_NAME}
        PROPERTIES
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
        )
    install(TARGETS ${EX_NAME}
        DESTINATION ${TARGET_SYS_PATH}/usr/bin
        )
endforeach()
