include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/rest
)

add_subdirectory(rest)

set(TARGET rest)

set(SOURCES
    main.cpp
)

set(LIBRARIES
    rest_server
)

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} PUBLIC rest_server)
