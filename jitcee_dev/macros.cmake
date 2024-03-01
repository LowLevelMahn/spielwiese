
macro(ADD_HEADER_FILTER HEADER_FILES)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} 
        PREFIX "${HEADER_FILTER}" 
        FILES ${HEADER_FILES}
    )
    list(APPEND ALL_FILES ${HEADER_FILES})
endmacro()


macro(ADD_SOURCE_FILTER SOURCE_FILES)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} 
        PREFIX "${SOURCE_FILTER}" 
        FILES ${SOURCE_FILES}
    )
    list(APPEND ALL_FILES ${SOURCE_FILES})
endmacro()

macro (DEFINE_STATIC_LIBRARY TARGET PARENT_NAME)
    add_library(${TARGET} STATIC ${ALL_FILES})
    if( NOT ${PARENT_NAME} STREQUAL "." )
      set_target_properties(${TARGET} PROPERTIES FOLDER ${PARENT_NAME})
    endif()
    set(ROOT_NAMESPACE ${TARGET})
    install (TARGETS ${TARGET}
         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install
         LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/_install)
endmacro()

# see https://stackoverflow.com/questions/56514533/how-to-specify-the-output-directory-of-a-given-dll
# dll plotforms need RUNTIME_OUTPUT_DIRECTORY and non dll platforms LIBRARY_OUTPUT_DIRECTORY
macro (DEFINE_DYNAMIC_LIBRARY TARGET PARENT_NAME)
    add_library(${TARGET} SHARED ${ALL_FILES})
    set_target_properties(${TARGET} PROPERTIES 
        FOLDER ${PARENT_NAME}
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CONFIG}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CONFIG}"
    )
    set(ROOT_NAMESPACE ${TARGET})
endmacro()

macro (DEFINE_EXECUTABLE TARGET PARENT_NAME)
    add_executable(${TARGET} ${ALL_FILES})
    set_target_properties(${TARGET} PROPERTIES 
        FOLDER ${PARENT_NAME}
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CONFIG}"
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CONFIG}"
    )
    set(ROOT_NAMESPACE ${TARGET}) 
endmacro()