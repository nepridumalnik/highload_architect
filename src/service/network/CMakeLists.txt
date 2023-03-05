set(TARGET network)

set(SOURCE
    connection.cpp
    router.cpp
)

add_library(${TARGET} ${SOURCE})
