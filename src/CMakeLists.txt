cmake_minimum_required(VERSION 3.13) # 2.2 - case insensitive syntax
                                     # 3.13 included policy CMP0077

project(${CN_APP_NAME}-src VERSION ${PROJECT_VERSION} LANGUAGES CXX)

message(STATUS "CN: Start configure server '${CN_APP_NAME}' with version '${CN_APP_VERSION}' for project '${PROJECT_NAME}'")

option(CN_USE_WSTRING "Use 16-bit string char" OFF)

configure_file(${CMAKE_CURRENT_LIST_DIR}/core/CnConfig.h.in ${CMAKE_CURRENT_LIST_DIR}/core/CnConfig.h)

set(CN_HEADERS
    core/CnConfig.h
    core/std/CnStd_string.h
    core/std/CnStd_stdio.h
    core/std/CnStd_iostream.h
    core/std/CnStd_sstream.h
    core/CnSharedPointer.h
    core/CnCore.h
    core/CnCriticalSection.h
    core/CnTimestamp.h
    core/CnDateTime_p.h
    core/CnDateTime.h
    core/CnString_p.h
    core/CnString.h
    core/CnFileInfo_p.h
    core/CnFileInfo.h
    core/CnFile_p.h
    core/CnFile.h
    core/CnDir_p.h
    core/CnDir.h
    core/CnLog.h
    core/CnEvent.h
    core/CnEventLoop.h
    core/CnThread.h
    core/CnUtils.h
    core/CnVariant.h
    core/CnModbus.h
    core/CnModbusClient.h
    core/CnObject.h
    core/CnConfReader.h
    core/CnItemCsvReader.h
    core/CnBaseItem.h
    core/CnItemLexer.h
    core/CnGlobal.h
    log/CnLogger.h
    log/CnLoggerSystem.h
    log/CnLoggerConsole.h
    log/CnLoggerFile.h
    log/CnLoggerManager.h
    cfg/CnCfgBase.h
    cfg/CnCfgPort.h
    cfg/CnCfgDeviceItem.h
    cfg/CnCfgDeviceItemParser.h
    cfg/CnCfgDevice.h
    cfg/CnCfgProject.h
    cfg/CnCfgBuilder.h
    device/items/CnDeviceBaseItem.h
    device/items/CnDeviceCfgItem.h
    device/items/CnDeviceCmdItem.h
    device/items/CnDeviceSetItem.h
    device/items/CnDeviceStatItem.h
    device/items/CnDeviceModbusItem.h
    device/items/CnDeviceItemList.h
    device/messages/CnDeviceMessageBase.h
    device/messages/CnDeviceMessage.h
    device/messages/CnDeviceMessageList.h
    device/CnDeviceConfig.h
    device/CnDeviceStat.h
    device/CnDevice.h
    device/CnDeviceRunnable.h
    port/items/CnPortBaseItem.h
    port/items/CnPortCfgItem.h
    port/items/CnPortCmdItem.h
    port/items/CnPortSetItem.h
    port/items/CnPortStatItem.h
    port/CnPortConfig.h
    port/CnPortStat.h
    port/CnPort.h
    port/CnPortRunnable.h
    port/CnPortThread.h
    app/items/CnAppBaseItem.h
    app/items/CnAppCfgItem.h
    app/items/CnAppCmdItem.h
    app/CnAppConfig.h
    app/CnApp.h
    ua/core/CnUaCore.h
    ua/core/CnUaServer.h
    ua/device/items/CnUaDeviceItemList.h
    ua/device/messages/CnUaDeviceMessage.h
    ua/device/messages/CnUaDeviceMessageList.h
    ua/device/CnUaDevice.h
    ua/port/CnUaPort.h
    ua/app/CnUaApp.h
    ua/app/CnUaThread.h
)

set(CN_SOURCES
    core/std/CnStd_string.cpp
    core/CnTimestamp.cpp
    core/CnDateTime.cpp
    core/CnString.cpp
    core/CnFileInfo.cpp
    core/CnFile.cpp
    core/CnDir.cpp
    core/CnLog.cpp
    core/CnUtils.cpp
    core/CnVariant.cpp
    core/CnEventLoop.cpp
    core/CnThread.cpp
    core/CnModbus.cpp
    core/CnModbusClient.cpp
    core/CnObject.cpp
    core/CnConfReader.cpp
    core/CnItemCsvReader.cpp
    core/CnItemLexer.cpp
    core/CnBaseItem.cpp
    log/CnLogger.cpp
    log/CnLoggerConsole.cpp
    log/CnLoggerFile.cpp
    log/CnLoggerManager.cpp
    cfg/CnCfgBase.cpp
    cfg/CnCfgDeviceItemParser.cpp
    cfg/CnCfgDevice.cpp
    cfg/CnCfgProject.cpp
    cfg/CnCfgBuilder.cpp
    device/items/CnDeviceBaseItem.cpp
    device/items/CnDeviceModbusItem.cpp
    device/items/CnDeviceItemList.cpp
    device/messages/CnDeviceMessageBase.cpp
    device/messages/CnDeviceMessage.cpp
    device/messages/CnDeviceMessageList.cpp
    device/CnDeviceConfig.cpp
    device/CnDeviceStat.cpp
    device/CnDevice.cpp
    device/CnDeviceRunnable.cpp
    port/items/CnPortBaseItem.cpp
    port/CnPortConfig.cpp
    port/CnPortStat.cpp
    port/CnPort.cpp
    port/CnPortRunnable.cpp
    port/CnPortThread.cpp
    app/items/CnAppBaseItem.cpp
    app/CnAppConfig.cpp
    app/CnApp.cpp
    ua/core/CnUaCore.cpp
    ua/core/CnUaServer.cpp
    ua/device/items/CnUaDeviceItemList.cpp
    ua/device/messages/CnUaDeviceMessage.cpp
    ua/device/messages/CnUaDeviceMessageList.cpp
    ua/device/CnUaDevice.cpp
    ua/port/CnUaPort.cpp
    ua/app/CnUaApp.cpp
    ua/app/CnUaThread.cpp
    main.cpp
)     

set(CN_RESOURCES 
)     

if (WIN32)
    set(CN_HEADERS ${CN_HEADERS}
        core/win/CnWindows.h
        core/win/CnDateTime_p_win.h
        core/win/CnFileInfo_p_win.h
        core/win/CnFile_p_win.h
        core/win/CnDir_p_win.h
    )

    set(CN_SOURCES ${CN_SOURCES}
        core/win/CnTimestamp_win.cpp
        core/win/CnDateTime_win.cpp
        core/win/CnFileInfo_win.cpp
        core/win/CnFile_win.cpp
        core/win/CnDir_win.cpp
        core/win/CnUtils_win.cpp
        core/win/CnThread_win.cpp
        log/win/CnLoggerSystem_win.cpp
        app/win/CnApp_win.cpp
    )

    set(CN_WIN_RESOURCE_FILE win_resource.rc)

    message(STATUS "CN: ${PROJECT_NAME} resource file for Windows: '${CN_WIN_RESOURCE_FILE}'")

    set(CN_WIN_ICON "IDI_ICON1\tICON\tDISCARDABLE\t\"${CN_APP_NAME}.ico\"")
    set(CN_WIN_FILE "${CN_APP_NAME}")
    set(CN_WIN_OUTFILE "${CN_WIN_FILE}.exe")
    configure_file(${CMAKE_CURRENT_LIST_DIR}/${CN_WIN_RESOURCE_FILE}.in 
                   ${CMAKE_CURRENT_LIST_DIR}/${CN_WIN_RESOURCE_FILE})

    set(CN_RESOURCES ${CN_RESOURCES}
        ${CMAKE_CURRENT_LIST_DIR}/${CN_WIN_RESOURCE_FILE}           
    )
else()
    set(CN_HEADERS ${CN_HEADERS}
        core/unix/CnUnix.h
        core/unix/CnDateTime_p_unix.h
        core/unix/CnFileInfo_p_unix.h
        core/unix/CnFile_p_unix.h
        core/unix/CnDir_p_unix.h
        )

    set(CN_SOURCES ${CN_SOURCES}
        core/unix/CnTimestamp_unix.cpp
        core/unix/CnDateTime_unix.cpp
        core/unix/CnFileInfo_unix.cpp
        core/unix/CnFile_unix.cpp
        core/unix/CnDir_unix.cpp
        core/unix/CnUtils_unix.cpp
        core/unix/CnThread_unix.cpp
        log/unix/CnLoggerSystem_unix.cpp
        app/unix/CnApp_unix.cpp
        )
endif()

add_executable(${CN_APP_NAME} ${CN_HEADERS} ${CN_SOURCES} ${CN_RESOURCES})

include_directories(.
                    ..
                    ../modbus/src
                    ../opcua/include
                    core
                    core/std
                    ua
                    ua/core
                    ${CMAKE_BINARY_DIR}/opcua/src_generated
)

#target_compile_definitions(${CN_APP_NAME} PRIVATE QT_NO_KEYWORDS)

if (WIN32)
    target_compile_definitions(${CN_APP_NAME} PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
    if (CN_USE_WSTRING)
        message(STATUS "CN: Using wide string (UTF-16) for Windows")
        target_compile_definitions(${CN_APP_NAME} PRIVATE UNICODE _UNICODE)
    else()
        message(STATUS "CN: Using char string (UTF-8) for Windows")
        target_compile_definitions(${CN_APP_NAME} PRIVATE -UUNICODE -U_UNICODE)
    endif()
    set_target_properties(${CN_APP_NAME} PROPERTIES LINK_FLAGS "/PROFILE")
endif()

set_target_properties(
    ${CN_APP_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    #WIN32_EXECUTABLE true
)

target_link_libraries(${CN_APP_NAME} PRIVATE 
                      modbus
                      open62541
)

if (WIN32)
    target_link_libraries(${CN_APP_NAME} PRIVATE 
                        shlwapi
    )
endif()

# to initiate install process run: `cmake --install <build_dir> --prefix <install_dest_dir>`

if (WIN32)
	message(STATUS "CN: Generate install-data for Windows")

	file(GLOB CN_WIN_CONF_FILES "${CMAKE_SOURCE_DIR}/install/${CN_APP_CONF}")
	file(GLOB CN_WIN_HELP_FILES "${CMAKE_BINARY_DIR}/doc/output-en/${CN_APP_NAME}.chm")

	install(TARGETS ${CN_APP_NAME}       DESTINATION ".")
	install(FILES   ${CN_WIN_CONF_FILES} DESTINATION "./conf")
	install(FILES   ${CN_WIN_HELP_FILES} DESTINATION "./doc")
	install(DIRECTORY 					 DESTINATION "./log")
else()
	# Example of starting install process:
	# `$ sudo cmake --install ~/tmp/cmake/modbusua/Linux/Debug --prefix /`
	message(STATUS "CN: Generate install-data for Linux")

	file(GLOB CN_LINUX_CONF_FILES "${CMAKE_SOURCE_DIR}/install/${CN_APP_CONF}")
	file(GLOB CN_LINUX_SERV_FILES "${CMAKE_SOURCE_DIR}/install/linux/${CN_APP_NAME}.service")

	install(TARGETS ${CN_APP_NAME}         DESTINATION "./usr/bin")
	install(FILES   ${CN_LINUX_CONF_FILES} DESTINATION "./etc/${CN_APP_NAME}")
	install(FILES   ${CN_LINUX_SERV_FILES} DESTINATION "./etc/systemd/system")
	install(DIRECTORY 					   DESTINATION "./var/lib/${CN_APP_NAME}")
	install(DIRECTORY 					   DESTINATION "./var/log/${CN_APP_NAME}")
endif()