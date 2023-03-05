set(TARGET network)

set(SOURCE
    connection.cpp
    router.cpp

    controllers/users.cpp
)

add_library(${TARGET} ${SOURCE})
