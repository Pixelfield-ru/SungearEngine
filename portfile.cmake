vcpkg_check_linkage(ONLY_DYNAMIC_LIBRARY)

vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO Pixelfield-ru/SungearEngine
        REF "${VERSION}"
        SHA512 13c7599c157e50d273120312b17a41bc8e62b075146f9bffe566bfc6cb07aded9b6e0e20098787d05806e6fdc884357174fbf272a38018f7e39540bb4e85f300
        HEAD_REF main
)

vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install([ADD_BIN_TO_PATH])

vcpkg_cmake_config_fixup(PACKAGE_NAME "sungearengine")

# file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

#[[file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)]]
