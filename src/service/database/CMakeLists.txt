set(TARGET database)

set(SOURCE
    models/users/users.cpp
    models/users/user_row.cpp
    models/users/users_auth.cpp
    models/users/user_auth_row.cpp
    models/users/user_find_condition.cpp
)

set(LIBRARIES
    ${CONAN_LIBS_LIBMYSQLCLIENT}
    ${CONAN_LIBS_ZLIB}
    ${CONAN_LIBS_ZSTD}
    ${CONAN_LIBS_OPENSSL}
    ${CONAN_LIBS_POCO}
)

if(UNIX)
    list(APPEND LIBRARIES
        pthread
    )
endif(UNIX)

add_library(${TARGET} STATIC ${SOURCE})
target_link_libraries(${TARGET} PUBLIC ${LIBRARIES})
