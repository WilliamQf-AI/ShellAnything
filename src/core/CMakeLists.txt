set(SHELLANYTHING_CORE_HEADER_FILES ""
  ${CMAKE_SOURCE_DIR}/src/core/IAction.h
  ${CMAKE_SOURCE_DIR}/src/core/ActionClipboard.h
  ${CMAKE_SOURCE_DIR}/src/core/ActionExecute.h
  ${CMAKE_SOURCE_DIR}/src/core/ActionFile.h
  ${CMAKE_SOURCE_DIR}/src/core/ActionManager.h
  ${CMAKE_SOURCE_DIR}/src/core/ActionMessage.h
  ${CMAKE_SOURCE_DIR}/src/core/ActionOpen.h
  ${CMAKE_SOURCE_DIR}/src/core/ActionPrompt.h
  ${CMAKE_SOURCE_DIR}/src/core/ActionProperty.h
  ${CMAKE_SOURCE_DIR}/src/core/ActionStop.h
  ${CMAKE_SOURCE_DIR}/src/core/App.h
  ${CMAKE_SOURCE_DIR}/src/core/BaseAction.h
  ${CMAKE_SOURCE_DIR}/src/core/ConfigFile.h
  ${CMAKE_SOURCE_DIR}/src/core/ConfigManager.h
  ${CMAKE_SOURCE_DIR}/src/core/SelectionContext.h
  ${CMAKE_SOURCE_DIR}/src/core/ConsoleLoggerService.h
  ${CMAKE_SOURCE_DIR}/src/core/DefaultSettings.h
  ${CMAKE_SOURCE_DIR}/src/core/Environment.h
  ${CMAKE_SOURCE_DIR}/src/core/Icon.h
  ${CMAKE_SOURCE_DIR}/src/core/IObject.h
  ${CMAKE_SOURCE_DIR}/src/core/IIconResolutionService.h
  ${CMAKE_SOURCE_DIR}/src/core/IKeyboardService.h
  ${CMAKE_SOURCE_DIR}/src/core/ILiveProperty.h
  ${CMAKE_SOURCE_DIR}/src/core/IClipboardService.h
  ${CMAKE_SOURCE_DIR}/src/core/ILoggerService.h
  ${CMAKE_SOURCE_DIR}/src/core/IProcessLauncherService.h
  ${CMAKE_SOURCE_DIR}/src/core/IRandomService.h
  ${CMAKE_SOURCE_DIR}/src/core/IRegistryService.h
  ${CMAKE_SOURCE_DIR}/src/core/KeyboardHelper.h
  ${CMAKE_SOURCE_DIR}/src/core/LoggerHelper.h
  ${CMAKE_SOURCE_DIR}/src/core/Menu.h
  ${CMAKE_SOURCE_DIR}/src/core/PcgRandomService.h
  ${CMAKE_SOURCE_DIR}/src/core/RandomHelper.h
  ${CMAKE_SOURCE_DIR}/src/core/Validator.h
)

add_library(sa.core SHARED
  ${SHELLANYTHING_EXPORT_HEADER}
  ${SHELLANYTHING_VERSION_HEADER}
  ${SHELLANYTHING_CONFIG_HEADER}
  ${CONFIGURATION_DEFAULT_FILES}
  ${SHELLANYTHING_CORE_HEADER_FILES}
  IAction.cpp
  ActionClipboard.cpp
  ActionExecute.cpp
  ActionFile.cpp
  ActionManager.cpp
  ActionMessage.cpp
  ActionOpen.cpp
  ActionPrompt.cpp
  ActionProperty.cpp
  ActionStop.cpp
  App.cpp
  BaseAction.cpp
  ConfigFile.cpp
  ConfigManager.cpp
  SelectionContext.cpp
  ConsoleLoggerService.cpp
  DefaultSettings.cpp
  FileMagicManager.h
  FileMagicManager.cpp
  IActionFactory.h
  IActionFactory.cpp
  IAttributeValidator.h
  IAttributeValidator.cpp
  Icon.cpp
  IObject.cpp
  IIconResolutionService.cpp
  IKeyboardService.cpp
  ILiveProperty.cpp
  IClipboardService.cpp
  ILoggerService.cpp
  IProcessLauncherService.cpp
  IRandomService.cpp
  IRegistryService.cpp
  KeyboardHelper.cpp
  LoggerHelper.cpp
  InputBox.h
  InputBox.cpp
  IntList.h
  IUpdateCallback.h
  IUpdateCallback.cpp
  Menu.cpp
  ObjectFactory.h
  ObjectFactory.cpp
  PcgRandomService.cpp
  Plugin.h
  Plugin.cpp
  Unicode.h
  Unicode.cpp
  Validator.cpp
  DriveClass.h
  DriveClass.cpp
  Enums.h
  Environment.cpp
  ErrorManager.h
  ErrorManager.cpp
  PropertyManager.h
  PropertyManager.cpp
  PropertyStore.h
  PropertyStore.cpp
  RandomHelper.cpp
  Registry.h
  Registry.cpp
  StringList.h
  Wildcard.cpp
  Wildcard.h
)

# Group external files as filter for Visual Studio
source_group("Default Configuration Files"          FILES ${CONFIGURATION_DEFAULT_FILES})

# Copy default configuration files database to target dir
add_custom_command( TARGET sa.core POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/resources/configurations $<TARGET_FILE_DIR:sa.core>/../resources/configurations
                    COMMENT "Copying default configurations files database.")

# Copy registration and unregistration scripts to target dir
add_custom_command( TARGET sa.core POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_SOURCE_DIR}/resources/installer/bin/register.bat $<TARGET_FILE_DIR:sa.core>/register.bat
                    COMMENT "Copying register.bat script.")
add_custom_command( TARGET sa.core POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_SOURCE_DIR}/resources/installer/bin/unregister.bat $<TARGET_FILE_DIR:sa.core>/unregister.bat
                    COMMENT "Copying unregister.bat script.")

# Copy files and other dependencies to target dir
add_custom_command( TARGET sa.core POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${LIBMAGIC_MGC_DIR}/magic.mgc $<TARGET_FILE_DIR:sa.core>/magic.mgc
                    COMMENT "Copying magic.mgc.")
add_custom_command( TARGET sa.core POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${LIBMAGIC_MGC_DIR}/file.exe $<TARGET_FILE_DIR:sa.core>/file.exe
                    COMMENT "Copying file.exe.")
add_custom_command( TARGET sa.core POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${ZLIB_INCLUDE_DIRS}/../bin/zlib.dll $<TARGET_FILE_DIR:sa.core>/zlib.dll
                    COMMENT "Copying zlib.dll.")
add_custom_command( TARGET sa.core POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:glog::glog> $<TARGET_FILE_DIR:sa.core>/$<TARGET_FILE_NAME:glog::glog>
                    COMMENT "Copying glog.")

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(sa.core         PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for the library.
target_include_directories(sa.core
  PUBLIC
    $<INSTALL_INTERFACE:${SHELLANYTHING_INSTALL_INCLUDE_DIR}>  # for clients using the installed library.
  PRIVATE
    rapidassist
    libexprtk
    libmagic
    ${PCGCPP_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src/libexprtk
    ${CMAKE_SOURCE_DIR}/src/shared
)

# Define linking dependencies.
add_dependencies(sa.core libexprtk sa.shared)
target_link_libraries(sa.core
  PRIVATE
    rapidassist
    libexprtk
    libmagic
    sa.shared
)

# Also add Tinyxml2 include and libraries.
# The include/libraries are added at the end to allow supporting both static or shared libraries (the target names are different).
# See issue #67 (https://github.com/end2endzone/ShellAnything/issues/67) for details.
if (TARGET tinyxml2)
  target_include_directories(sa.core PRIVATE tinyxml2)
  target_link_libraries(sa.core PRIVATE tinyxml2)
else()
  target_include_directories(sa.core PRIVATE tinyxml2_static)
  target_link_libraries(sa.core PRIVATE tinyxml2_static)
endif()

# Define files that will be part of the installation package.
install(TARGETS sa.core
        EXPORT shellanything-targets
        ARCHIVE DESTINATION ${SHELLANYTHING_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${SHELLANYTHING_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${SHELLANYTHING_INSTALL_BIN_DIR}
)
