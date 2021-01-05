find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(Sensorfw sensord-qt5 IMPORTED_TARGET)
    if (NOT TARGET PkgConfig::Sensorfw)
        set(Sensorfw_FOUND 0)
    endif()
else()
    message("Sensorfw pkg not found. Looking for library")
    find_library(sensorfw)
endif()
