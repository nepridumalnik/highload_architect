set(TARGET network)

set(SOURCE
    connection.cpp
    router.cpp

    controllers/users.cpp
    controllers/subcontrollers/user_register.cpp
)

add_library(${TARGET} ${SOURCE})
