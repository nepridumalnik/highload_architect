set(TARGET database)

set(SOURCE
    models/users/users.cpp
)

set(LIBRARIES
    boost_thread
    ${SOCI_LIBS}
)

if(UNIX)
    list(APPEND LIBRARIES
        pthread
    )
endif(UNIX)

add_library(${TARGET} STATIC ${SOURCE})
target_link_libraries(${TARGET} PRIVATE ${LIBRARIES})
