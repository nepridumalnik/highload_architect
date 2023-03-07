add_subdirectory(service)

set(TARGET hihgload_server)

set(SOURCES
    main.cpp
)

set(LIBRARIES
    service
    database
    network

    boost_system
    boost_thread

    ${CONAN_LIBS_SOCI}
    ${CONAN_LIBS_ZLIB}
    ${CONAN_LIBS_ZSTD}
    ${CONAN_LIBS_OPENSSL}
    ${CONAN_LIBS_LIBMYSQLCLIENT}
)

set(INCLUDE_DIRS
    ${CONAN_INCLUDE_DIRS}
)

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} PRIVATE ${LIBRARIES})
target_include_directories(${TARGET} PRIVATE ${INCLUDE_DIRS})
