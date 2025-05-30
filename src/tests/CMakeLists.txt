find_package(GTest REQUIRED)
find_package(rapidassist REQUIRED)
find_package(glog REQUIRED)
find_package(libmagic REQUIRED)

set(CONFIGURATION_TEST_FILES ""
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/samples.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionExecute.testWaitTimeout.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionExecute.testWaitInfinite.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionProperty.testCaptureOutput.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionProperty.testCopyFile.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionProperty.testFail.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionProperty.testLiveProperties.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionProperty.testRandomProperties.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionProperty.testRandomPropertiesAdvanced.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionProperty.testRegistryKey.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestActionProperty.testSearchPath.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testAssignCommandId.1.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testAssignCommandId.2.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testAssignCommandIdsInvalid.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testClear.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testDetectNewFile.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testFileModifications.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testFindMenuByName.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testFindMenuByNameCaseInsensitive.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testFindMenuByNameExpanding.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfigManager.testParentWithoutChildren.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestConfiguration.testLoadProperties.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestIObject.testToLongString.expected.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestIObject.testToLongString.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testGetParent.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseActionExecute.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseActionFile.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseActionMessage.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseActionPrompt.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseActionProperty.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseActionStop.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseSeparator.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseValidator.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseDefaults.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseIcon.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParseMenuMaxLength.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestObjectFactory.testParsePlugins.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestPlugins.testPluginActionGetData.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestPlugins.testPluginInitializeAndTerminate.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestPlugins.testProcess.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestPlugins.testServices.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestPlugins.testStrings.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestPlugins.testTime.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/tests.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files/TestTools.testArgumentsDebugger.xml
)

set(HEADER_AND_SOURCE_TEST_FILES ""
  TestActionExecute.cpp
  TestActionExecute.h
  TestActionFile.cpp
  TestActionFile.h
  TestActionProperty.cpp
  TestActionProperty.h
  TestActionStop.cpp
  TestActionStop.h
  TestBitmapCache.cpp
  TestBitmapCache.h
  TestConfigManager.cpp
  TestConfigManager.h
  TestConfiguration.cpp
  TestConfiguration.h
  TestDemoSamples.cpp
  TestDemoSamples.h
  TestEnvironment.cpp
  TestEnvironment.h
  TestGlogUtils.cpp
  TestGlogUtils.h
  TestIcon.cpp
  TestIcon.h
  TestInputBox.cpp
  TestInputBox.h
  TestIObject.cpp
  TestIObject.h
  TestKeyboardService.cpp
  TestKeyboardService.h
  TestLibExprtk.cpp
  TestLibExprtk.h
  TestLoggerHelper.cpp
  TestLoggerHelper.h
  TestMenu.cpp
  TestMenu.h
  TestObjectFactory.cpp
  TestObjectFactory.h
  TestPropertyManager.cpp
  TestPropertyManager.h
  TestRandomHelper.cpp
  TestRandomHelper.h
  TestRandomService.cpp
  TestRandomService.h
  TestSaUtils.cpp
  TestSaUtils.h
  TestSelectionContext.cpp
  TestSelectionContext.h
  TestShellExtension.cpp
  TestShellExtension.h
  TestTools.cpp
  TestTools.h
  TestUnicode.cpp
  TestUnicode.h
  TestValidator.cpp
  TestValidator.h
  TestWildcard.cpp
  TestWildcard.h
  TestWin32Clipboard.cpp
  TestWin32Clipboard.h
  TestWin32Registry.cpp
  TestWin32Registry.cpp
  TestWin32Registry.h
  TestWin32Registry.h
  TestWin32Utils.cpp
  TestWin32Utils.h
  TestWorkspace.cpp
  TestWorkspace.h
)

if(SHELLANYTHING_BUILD_PLUGINS)
  set(PLUGINS_TEST_FILES ""
    TestPlugins.cpp
    TestPlugins.h
  )
else()
  set(PLUGINS_TEST_FILES "")
endif()

add_executable(sa.tests
  ${SHELLANYTHING_EXPORT_HEADER}
  ${SHELLANYTHING_VERSION_HEADER}
  ${SHELLANYTHING_CONFIG_HEADER}
  ${CONFIGURATION_TEST_FILES}
  ${PLUGINS_TEST_FILES}
  ${HEADER_AND_SOURCE_TEST_FILES}
  ArgumentsHandler.cpp
  ArgumentsHandler.h
  LockFile.cpp
  LockFile.h
  main.cpp
  QuickLoader.cpp
  QuickLoader.h
  Workspace.cpp
  Workspace.h
)

# Group external files as filter for Visual Studio
source_group("Test Data Files"          FILES ${CONFIGURATION_TEST_FILES})
source_group("Test Source Files"        FILES ${HEADER_AND_SOURCE_TEST_FILES} ${PLUGINS_TEST_FILES})

# Unit test projects requires to link with pthread if also linking with gtest
if(NOT WIN32)
  set(PTHREAD_LIBRARIES -pthread)
endif()

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(sa.tests PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for the executable.
target_include_directories(sa.tests
  PRIVATE
    ${GTEST_INCLUDE_DIR}
    rapidassist
    libmagic
    ${CMAKE_SOURCE_DIR}/src/libexprtk
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/logger/glog
    ${CMAKE_BINARY_DIR}/src/logger/glog
    ${CMAKE_SOURCE_DIR}/src/windows
    ${CMAKE_BINARY_DIR}/src/windows
)

# Define linking dependencies.
add_dependencies(sa.tests sa.shared sa.core sa.shellextension libmagic)
target_link_libraries(sa.tests
  PRIVATE
    sa.shared
    sa.core
    sa.api
    sa.shellextension
    sa.logger.glog
    sa.windows
    ${PTHREAD_LIBRARIES}
    ${GTEST_LIBRARIES}
    rapidassist
    libexprtk
    libmagic
)

# Copy test configuration files database to target dir
add_custom_command( TARGET sa.tests POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/test_files $<TARGET_FILE_DIR:sa.tests>/test_files
                    COMMENT "Copying test configuration files database.")

# Copy test configuration files to build dir for local execution (from within the IDE)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Copy default configuration files to build dir for local execution (from within the IDE)
file(COPY ${CMAKE_SOURCE_DIR}/resources/configurations DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS sa.tests
        EXPORT shellanything-targets
        ARCHIVE DESTINATION ${SHELLANYTHING_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${SHELLANYTHING_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${SHELLANYTHING_INSTALL_BIN_DIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_files
        DESTINATION ${SHELLANYTHING_INSTALL_BIN_DIR}
)
