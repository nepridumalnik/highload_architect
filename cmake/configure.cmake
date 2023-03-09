set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

if(WIN32)
    conan_basic_setup()
    message("-- Setting up for Windows")
    add_compile_definitions(_WIN32_WINNT=0x0601)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4099")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4099")
endif()

include_directories(${CONAN_INCLUDE_DIRS})
include_directories(src)

link_directories(${CMAKE_BINARY_DIR}/bin)
link_directories(${CMAKE_BINARY_DIR}/lib)
link_directories(${CONAN_LIB_DIRS})
