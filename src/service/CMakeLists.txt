add_subdirectory(database)
add_subdirectory(network)

set(TARGET service)

set(SOURCE
    service.cpp
)

add_library(${TARGET} ${SOURCE})
