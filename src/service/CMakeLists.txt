add_subdirectory(database)
add_subdirectory(network)

set(TARGET service)

set(SOURCE
    service.cpp
    utils/utils.cpp
)

set(LIBRARIES
    database
    network
    ${SOCI_LIBS}
)

add_library(${TARGET} SHARED ${SOURCE})
target_link_libraries(${TARGET} PRIVATE ${LIBRARIES})
