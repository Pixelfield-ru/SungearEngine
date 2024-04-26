vcpkg_check_linkage(ONLY_DYNAMIC_LIBRARY)

set(VCPKG_BUILD_TYPE release)

vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO Pixelfield-ru/SungearEngine
        REF "${VERSION}"
        SHA512 1eedbd4c0ed87a4c60a011411bd57195ffc8edf5d5ef6d597881f2b3bb17cf7ad537336786a8bf6877a49dde50d7645ebc577df31fcdf32eed1f7b340c43ef15
        HEAD_REF main
)

vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
        OPTIONS
        -DCMAKE_BUILD_TYPE=release
)

vcpkg_cmake_install()

#vcpkg_cmake_config_fixup(PACKAGE_NAME "sungearengine")
vcpkg_cmake_config_fixup(CONFIG_PATH share/sungearengine)
vcpkg_fixup_pkgconfig()
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
# file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

#[[file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)]]
