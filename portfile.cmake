vcpkg_check_linkage(ONLY_DYNAMIC_LIBRARY)

set(VCPKG_BUILD_TYPE release)

vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO Pixelfield-ru/SungearEngine
        REF "${VERSION}"
        SHA512 fb58e869236554156e32f8a08b449b252dbac414c43df13198d0c5e0c2d2b9dc95c220dc10688aee202064aa59be2f63e89f02b1387af0f1e4f1af8789a4255b
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
