set(TARGET database)

set(SOURCE
    models/users/users.cpp
)

set(LIBRARIES
    ${CONAN_LIBS}
)

add_library(${TARGET} STATIC ${SOURCE})
target_link_libraries(${TARGET} PUBLIC ${LIBRARIES})
