vcpkg_check_linkage(ONLY_DYNAMIC_LIBRARY)

vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO Pixelfield-ru/SungearEngine
        REF "${VERSION}"
        SHA512 e70a9ff761e3f75662f4d6a2f9d2656cdd36950acc229ba05fccede109fb3c5097fdc5c0bdcc523307e34a11a0d89d25497e5e424b8dda9855112c38e9cc865a
        HEAD_REF main
)

vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "sungearengine")

# file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

#[[file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)]]
