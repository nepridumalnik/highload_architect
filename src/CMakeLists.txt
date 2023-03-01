set(TARGET hihgload_server)

set(SOURCES
    main.cpp
)

set(LIBRARIES
    ${CONAN_LIBS}
)

set(INCLUDE_DIRS
    ${CONAN_INCLUDE_DIRS}
)

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} PRIVATE ${LIBRARIES})
target_include_directories(${TARGET} PRIVATE ${INCLUDE_DIRS})
