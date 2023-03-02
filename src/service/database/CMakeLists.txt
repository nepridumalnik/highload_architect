set(TARGET database)

set(SOURCE
    models/users.cpp
)

add_library(${TARGET} ${SOURCE})
