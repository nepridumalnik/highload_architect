add_subdirectory(service)

set(TARGET highload_server)

set(SOURCES
    main.cpp
)

set(LIBRARIES
    service
)

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} PRIVATE ${LIBRARIES})

install(TARGETS ${TARGET})
