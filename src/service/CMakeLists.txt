add_subdirectory(database)
add_subdirectory(network)

set(TARGET service)

set(SOURCE
    service.cpp
)

set(LIBRARIES
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

add_library(${TARGET} SHARED ${SOURCE})
target_link_libraries(${TARGET} PRIVATE ${LIBRARIES})
