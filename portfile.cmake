vcpkg_check_linkage(ONLY_DYNAMIC_LIBRARY)

vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO Pixelfield-ru/SungearEngine
        REF "${VERSION}"
        SHA512 a55836e29410d28eccac3dc9126fc1d096304a36ecae969968843243e466d86a25c94ed5b582b998d0c7e33bdfc150f181084d95ed9800da67719b4fbc7f226e
        HEAD_REF main
)

vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
)

message("dfdf: " ${CURRENT_HOST_INSTALLED_DIR})

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "sungearengine")

# file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

#[[file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)]]
