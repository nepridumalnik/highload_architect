set(TARGET network)

set(SOURCE
    connection.cpp
    router.cpp

    controllers/users/users.cpp
    controllers/users/subcontrollers/user_get.cpp
    controllers/users/subcontrollers/user_login.cpp
    controllers/users/subcontrollers/user_register.cpp
)

add_library(${TARGET} STATIC ${SOURCE})
