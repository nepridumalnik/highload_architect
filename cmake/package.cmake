include(CPack)

if(WIN32)
    FILE(GLOB INSTALL_LIBS ${CMAKE_BINARY_DIR}/bin/*.dll)
elseif(UNIX)
    FILE(GLOB INSTALL_LIBS ${CMAKE_BINARY_DIR}/bin/*.so*)
endif()

install(FILES ${INSTALL_LIBS} DESTINATION bin)
