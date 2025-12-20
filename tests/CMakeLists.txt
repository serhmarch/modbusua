message(STATUS "CN: Tests included")

include(${CMAKE_CURRENT_SOURCE_DIR}/gtest_dependency.cmake)

set(CN_SRC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnConfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnConfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/std/CnStd_string.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/std/CnStd_stdio.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/std/CnStd_iostream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/std/CnStd_sstream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnSharedPointer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnCore.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnCriticalSection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnTimestamp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnDateTime_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnDateTime.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnString_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnString.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnFileInfo_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnFileInfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnFile_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnFile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnDir_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnDir.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnLog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnEventLoop.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnThread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnUtils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnVariant.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnModbus.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnModbusClient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnConfReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnItemCsvReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnBaseItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnItemLexer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnGlobal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/CnLogger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/CnLoggerSystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/CnLoggerConsole.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/CnLoggerFile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/CnLoggerManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgPort.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgDeviceItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgDeviceItemParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgDevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgProject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceBaseItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceCfgItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceCmdItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceSetItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceStatItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceModbusItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceItemList.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/messages/CnDeviceMessageBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/messages/CnDeviceMessage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/messages/CnDeviceMessageList.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/CnDeviceConfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/CnDeviceStat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/CnDevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/CnDeviceRunnable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/items/CnPortBaseItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/items/CnPortCfgItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/items/CnPortCmdItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/items/CnPortSetItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/items/CnPortStatItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPortConfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPortStat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPort.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPortRunnable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPortThread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/items/CnAppBaseItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/items/CnAppCfgItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/items/CnAppCmdItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/CnAppConfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/CnApp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/core/CnUaCore.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/core/CnUaServer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/device/items/CnUaDeviceItemList.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/device/messages/CnUaDeviceMessage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/device/messages/CnUaDeviceMessageList.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/device/CnUaDevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/port/CnUaPort.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/app/CnUaApp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/app/CnUaThread.h
)

set(CN_SRC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/std/CnStd_string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnTimestamp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnDateTime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnFileInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnDir.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnLog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnVariant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnEventLoop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnModbus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnModbusClient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnConfReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnItemCsvReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnItemLexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/CnBaseItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/CnLogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/CnLoggerConsole.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/CnLoggerFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/CnLoggerManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgDeviceItemParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgProject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cfg/CnCfgBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceBaseItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceModbusItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/items/CnDeviceItemList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/messages/CnDeviceMessageBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/messages/CnDeviceMessage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/messages/CnDeviceMessageList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/CnDeviceConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/CnDeviceStat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/CnDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device/CnDeviceRunnable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/items/CnPortBaseItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPortConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPortStat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPort.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPortRunnable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/port/CnPortThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/items/CnAppBaseItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/CnAppConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/CnApp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/core/CnUaCore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/core/CnUaServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/device/items/CnUaDeviceItemList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/device/messages/CnUaDeviceMessage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/device/messages/CnUaDeviceMessageList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/device/CnUaDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/port/CnUaPort.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/app/CnUaApp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ua/app/CnUaThread.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp
    )

if (WIN32)
    set(CN_SRC_HEADERS ${CN_SRC_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnWindows.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnDateTime_p_win.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnFileInfo_p_win.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnFile_p_win.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnDir_p_win.h
    )

    set(CN_SRC_SOURCES ${CN_SRC_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnTimestamp_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnDateTime_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnFileInfo_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnFile_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnDir_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnUtils_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/win/CnThread_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/win/CnLoggerSystem_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/win/CnApp_win.cpp
    )
else()
    set(CN_SRC_HEADERS ${CN_SRC_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnUnix.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnDateTime_p_unix.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnFileInfo_p_unix.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnFile_p_unix.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnDir_p_unix.h
        )

    set(CN_SRC_SOURCES ${CN_SRC_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnTimestamp_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnDateTime_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnFileInfo_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnFile_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnDir_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnUtils_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/unix/CnThread_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/log/unix/CnLoggerSystem_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/app/unix/CnApp_unix.cpp
        )
endif()

set(CN_TESTS_HEADERS 
#    MockModbusPort.h
#    MockModbusDevice.h
    "include/std/CnStd_filesystem.h"
    )

set(CN_TESTS_SOURCES
    global/global_test.cpp
    core/CnString_test.cpp 
    core/CnTimestamp_test.cpp
    core/CnDateTime_test.cpp
    core/CnVariant_test.cpp
    core/CnFileInfo_test.cpp
    core/CnFile_test.cpp
    core/CnDir_test.cpp
    core/CnUtils_test.cpp
    core/CnBaseItem_test.cpp
    core/CnConfReader_test.cpp
    core/CnItemCsvReader_test.cpp
    core/CnItemLexer_test.cpp
    core/CnModbus_test.cpp
    core/CnModbusClient_test.cpp
    log/CnLogger_test.cpp
    log/CnLoggerManager_test.cpp
    log/CnLoggerFile_test.cpp
    app/CnAppConfig_test.cpp
    app/CnApp_test.cpp
    cfg/CnCfgDevice_test.cpp
    cfg/CnCfgBuilder_test.cpp
    cfg/CnCfgPort_test.cpp
    cfg/CnCfgDeviceItem_test.cpp
    cfg/CnCfgDeviceItemParser_test.cpp
    cfg/CnCfgProject_test.cpp
    device/CnDeviceConfig_test.cpp
    device/CnDeviceStat_test.cpp
    device/CnDeviceItemList_test.cpp
    device/CnDeviceModbusItem_test.cpp
    device/CnDeviceSetItem_test.cpp
    device/CnDeviceCfgItem_test.cpp
    device/CnDeviceStatItem_test.cpp
    device/CnDeviceCmdItem_test.cpp
    device/CnDeviceMessage_test.cpp
    device/CnDeviceMessageList_test.cpp
    device/CnDevice_test.cpp
    port/CnPortConfig_test.cpp
    port/CnPortStat_test.cpp
    ua/CnUaCore_test.cpp
    main.cpp
    )

set(CN_TESTS_EXEC_NAME testmodbusua)

include_directories(.
                    ..
                    ../modbus/src
                    ../opcua/include
                    ../src
                    ../src/core
                    ../src/core/std
                    ../src/ua
                    ../src/ua/core
                    "include/std")

add_executable(${CN_TESTS_EXEC_NAME}
               ${GOOGLETEST_SOURCES}
               ${CN_SRC_HEADERS}
               ${CN_SRC_SOURCES}
               ${CN_TESTS_HEADERS}
               ${CN_TESTS_SOURCES}
               )

if (WIN32)
    target_compile_definitions(${CN_TESTS_EXEC_NAME} PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
    if (CN_USE_WSTRING)
        message(STATUS "CN TESTS: Using wide string (UTF-16) for Windows")
        target_compile_definitions(${CN_TESTS_EXEC_NAME} PRIVATE UNICODE _UNICODE)
    else()
        message(STATUS "CN TESTS: Using char string (UTF-8) for Windows")
        target_compile_definitions(${CN_TESTS_EXEC_NAME} PRIVATE -UUNICODE -U_UNICODE)
    endif()
endif()

target_link_libraries(${CN_TESTS_EXEC_NAME} PRIVATE 
                        modbus
                        open62541
)

# Register tests with CTest via GoogleTest discovery
if (BUILD_TESTING)
    include(GoogleTest)
    # Avoid running the test executable at build time; discover at test time
    gtest_discover_tests(${CN_TESTS_EXEC_NAME}
        DISCOVERY_MODE PRE_TEST
        # Optional: set working directory if needed
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
    # Also add a top-level test to run the whole suite if desired
    #add_test(NAME run-all-tests COMMAND ${CN_TESTS_EXEC_NAME})
endif()
