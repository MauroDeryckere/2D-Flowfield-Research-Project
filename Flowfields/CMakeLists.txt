set(FLOWFIELDS_PUBLIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Public")
set(FLOWFIELDS_PRIVATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Private")

# Gather all source files
file(GLOB_RECURSE FLOWFIELDS_PRIVATE_SOURCES
    "${FLOWFIELDS_PRIVATE_DIR}/*.cpp"
)

file(GLOB_RECURSE FLOWFIELDS_PRIVATE_HEADERS
    "${FLOWFIELDS_PRIVATE_DIR}/*.h"
)

file(GLOB_RECURSE FLOWFIELDS_PUBLIC_HEADERS
    "${FLOWFIELDS_PUBLIC_DIR}/*.h"
)

add_executable(Flowfields
    ${FLOWFIELDS_PRIVATE_SOURCES}
    ${FLOWFIELDS_PRIVATE_HEADERS}
    ${FLOWFIELDS_PUBLIC_HEADERS}
)

target_include_directories(Flowfields
    PUBLIC
        ${FLOWFIELDS_PUBLIC_DIR}
    PRIVATE
        ${FLOWFIELDS_PRIVATE_DIR}
)

target_link_libraries(Flowfields
    PRIVATE
        Engine
)

set_target_properties(Flowfields PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(MSVC AND WIN32)
    set_target_properties(Flowfields PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/ENTRY:wWinMainCRTStartup"
    )
endif()