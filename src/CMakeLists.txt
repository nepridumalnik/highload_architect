add_subdirectory(service)

set(TARGET highload_server)

set(SOURCES
    main.cpp
)

set(LIBRARIES
    service
)

if(UNIX)
    list(APPEND LIBRARIES
        pthread
    )
endif(UNIX)

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} PUBLIC ${LIBRARIES})

install(TARGETS ${TARGET})
