set(TARGET database)

set(SOURCE
    models/users/users.cpp
)

add_library(${TARGET} ${SOURCE})
