project(syslibs)

if(EXISTS ${CMAKE_SOURCE_DIR}/syslibs/libgcc_s_seh-1.dll)
    file(COPY ${CMAKE_SOURCE_DIR}/syslibs/libgcc_s_seh-1.dll DESTINATION ${CMAKE_BINARY_DIR}/Sources/SGEditor)
    file(COPY ${CMAKE_SOURCE_DIR}/syslibs/libgcc_s_seh-1.dll DESTINATION ${CMAKE_BINARY_DIR}/Tests/SGPhysicsTest)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/syslibs/libstdc++-6.dll)
    file(COPY ${CMAKE_SOURCE_DIR}/syslibs/libstdc++-6.dll DESTINATION ${CMAKE_BINARY_DIR}/Sources/SGEditor)
    file(COPY ${CMAKE_SOURCE_DIR}/syslibs/libstdc++-6.dll DESTINATION ${CMAKE_BINARY_DIR}/Tests/SGPhysicsTest)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/syslibs/libwinpthread-1.dll)
    file(COPY ${CMAKE_SOURCE_DIR}/syslibs/libwinpthread-1.dll DESTINATION ${CMAKE_BINARY_DIR}/Sources/SGEditor)
    file(COPY ${CMAKE_SOURCE_DIR}/syslibs/libwinpthread-1.dll DESTINATION ${CMAKE_BINARY_DIR}/Tests/SGPhysicsTest)
endif()