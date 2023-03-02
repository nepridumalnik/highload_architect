add_subdirectory(database)

set(TARGET service)

set(SOURCE
    service.cpp
)

add_library(${TARGET} ${SOURCE})
