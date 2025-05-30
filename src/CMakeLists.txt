find_package(rapidassist REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package(glog REQUIRED)
find_package(libmagic REQUIRED)
find_package(ZLIB REQUIRED)

#resource.rc
set(SHELLANYTHING_RESOURCE_RC ${CMAKE_BINARY_DIR}/src/resource.rc)
message("Generating ${SHELLANYTHING_RESOURCE_RC}...")
configure_file( ${CMAKE_SOURCE_DIR}/src/resource.rc.in ${SHELLANYTHING_RESOURCE_RC} )

#version.rc
set(SHELLANYTHING_VERSION_RC ${CMAKE_BINARY_DIR}/src/version.rc)
message("Generating ${SHELLANYTHING_VERSION_RC}...")
configure_file( ${CMAKE_SOURCE_DIR}/src/version.rc.in ${SHELLANYTHING_VERSION_RC} )

set(CONFIGURATION_DEFAULT_FILES ""
   ${CMAKE_SOURCE_DIR}/resources/configurations/default.xml
   ${CMAKE_SOURCE_DIR}/resources/configurations/shellanything.xml
)

# Subprojects
add_subdirectory(shared)
add_subdirectory(libexprtk)
add_subdirectory(core)
add_subdirectory(shellextension)
add_subdirectory(api)
add_subdirectory(logger/glog)
add_subdirectory(windows)
add_subdirectory(file_explorer_renew)
add_subdirectory(arguments.debugger.console)
add_subdirectory(arguments.debugger.window)
add_subdirectory(flat-color-icons)

if(SHELLANYTHING_BUILD_PLUGINS)
  add_subdirectory(plugins/sa_plugin_process)
  add_subdirectory(plugins/sa_plugin_services)
  add_subdirectory(plugins/sa_plugin_strings)
  add_subdirectory(plugins/sa_plugin_time)
endif()
add_subdirectory(tests/sa_plugin_test_data)

add_custom_target(src_dummy_target
                  SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
                          "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
                          "${CMAKE_CURRENT_SOURCE_DIR}/resource.rc.in"
                          "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
                          "${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in"
)

install(FILES ${LIBMAGIC_MGC_DIR}/magic.mgc
              ${LIBMAGIC_MGC_DIR}/file.exe
              ${ZLIB_INCLUDE_DIRS}/../bin/zlib.dll
              $<TARGET_FILE:glog::glog>
              DESTINATION ${SHELLANYTHING_INSTALL_BIN_DIR}
)
