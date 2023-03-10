add_subdirectory(database)
add_subdirectory(network)

set(TARGET service)

set(SOURCE
    service.cpp
)

set(LIBRARIES
    /usr/lib/x86_64-linux-gnu/libsoci_mysql.so
    /usr/lib/x86_64-linux-gnu/libsoci_core.so

    database
    network
)

add_library(${TARGET} SHARED ${SOURCE})
target_link_libraries(${TARGET} PUBLIC ${LIBRARIES})
