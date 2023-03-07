set(TARGET network)

set(SOURCE
    connection.cpp
    router.cpp

    controllers/users/users.cpp
    controllers/users/subcontrollers/user_register.cpp
)

add_library(${TARGET} ${SOURCE})
