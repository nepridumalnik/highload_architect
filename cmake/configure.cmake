set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CONAN_INCLUDE_DIRS})
include_directories(src)

link_directories(${CMAKE_BINARY_DIR}/bin)
link_directories(${CMAKE_BINARY_DIR}/lib)
link_directories(${CONAN_LIB_DIRS})
link_directories(/usr/lib64)
link_directories(/usr/local/lib64)

if(WIN32)
    message("-- Setting up for Windows")
    add_compile_definitions(_WIN32_WINNT=0x0601)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4099")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4099")

    set(SOCI_LIBS
        ${CONAN_LIBS_SOCI}
        ${CONAN_LIBS_LIBMYSQLCLIENT}
        ${CONAN_LIBS_ZLIB}
        ${CONAN_LIBS_ZSTD}
        ${CONAN_LIBS_OPENSSL}
    )
elseif(UNIX)
    message("-- Setting up for Unix")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

    # Рецепт из conancenter поломан, поэтому существует этот костыль
    # Нужно установить libsoci-dev
    set(SOCI_MYSQL /usr/lib/x86_64-linux-gnu/libsoci_mysql.so)
    set(SOCI_CORE /usr/lib/x86_64-linux-gnu/libsoci_core.so)
    set(SOCI_LIBS ${SOCI_MYSQL} ${SOCI_CORE})

    file(COPY ${SOCI_MYSQL} DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} FOLLOW_SYMLINK_CHAIN)
    file(COPY ${SOCI_CORE} DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} FOLLOW_SYMLINK_CHAIN)
    message("-- CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
endif()
