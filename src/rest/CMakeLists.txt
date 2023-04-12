set(TARGET rest_server)

set(SOURCES
    rest_server.cpp

    http/request_handler.cpp
    http/controllers/user_get.cpp
    http/controllers/user_login.cpp
    http/controllers/user_register.cpp
    http/controllers/user_search.cpp
    http/controllers/make_friends.cpp

    models/users/users.cpp
    models/users/users_auth.cpp
    models/users/user_find_condition.cpp
    models/users/user_row.cpp
    models/users/user_auth_row.cpp
    models/users/friends.cpp

    resources/jsons.cpp
    resources/methods.cpp
    resources/messages.cpp
    resources/connection_parameters.cpp

    settings/settings_reader.cpp

    utils/utils.cpp
)

set(LIBRARIES
    ${CONAN_LIBS}
)

add_library(${TARGET} ${SOURCES})
target_link_libraries(rest_server PRIVATE ${LIBRARIES})