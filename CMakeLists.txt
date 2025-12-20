cmake_minimum_required(VERSION 3.13) # 2.2 - case insensitive syntax
                                     # 3.13 included policy CMP0077

set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD_REQUIRED ON)

project(modbusua VERSION 0.4.0 LANGUAGES CXX)

# CTest integration
include(CTest)

set(CN_APP_NAME modbusua CACHE INTERNAL "Name of the application")
set(CN_APP_LNAME "${CN_APP_NAME} Gateway" CACHE INTERNAL "Long name of the application")
set(CN_APP_DESC "OPC UA - Modbus Gateway" CACHE INTERNAL "Description of the application")
set(CN_APP_ARCH "amd64" CACHE INTERNAL "Processor architecture type")
set(CN_APP_CONF "${CN_APP_NAME}.conf" CACHE INTERNAL "Name of the configuration file")
set(CN_APP_VERSION ${PROJECT_VERSION} CACHE INTERNAL "Version of the application")
set(CN_APP_VENDOR "serhmarch" CACHE INTERNAL "Vendor of the application")


option(CN_TESTS_ENABLED "Enable unit tests" OFF)

# Keep CTest's BUILD_TESTING consistent with our tests option
if (CN_TESTS_ENABLED)
    enable_testing()
    set(BUILD_TESTING ON CACHE BOOL "Enable CTest testing" FORCE)
else()
    set(BUILD_TESTING OFF CACHE BOOL "Disable CTest testing" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release")

if (NOT WIN32)
	include(cmake/linux.cmake)
    #set(CMAKE_SKIP_BUILD_RPATH  FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
    #set(CMAKE_INSTALL_RPATH "\${ORIGIN}")
endif()

#######################################
######### COMMON LIB SETTINGS #########
#######################################
set(BUILD_SHARED_LIBS OFF)


#######################################
######### MODBUS LIB SETTINGS #########
#######################################
set(MB_QT_ENABLED OFF)
set(MB_ADDRESS_CLASS_DISABLE OFF)
set(MBF_DIAGNOSTICS_DISABLE                   ON)
set(MBF_GET_COMM_EVENT_COUNTER_DISABLE        ON)
set(MBF_GET_COMM_EVENT_LOG_DISABLE            ON)
set(MBF_REPORT_SERVER_ID_DISABLE              ON)
set(MBF_READ_FILE_RECORD_DISABLE              ON)
set(MBF_WRITE_FILE_RECORD_DISABLE             ON)
set(MBF_MASK_WRITE_REGISTER_DISABLE           ON)
set(MBF_READ_WRITE_MULTIPLE_REGISTERS_DISABLE ON)
set(MBF_READ_FIFO_QUEUE_DISABLE               ON)
set(MB_SERVER_DISABLE ON)
set(MB_C_SUPPORT_DISABLE ON)

# EXCLUDE_FROM_ALL: to not generate install data
add_subdirectory(modbus/src EXCLUDE_FROM_ALL)


#######################################
######## OPEN62541 LIB SETTINGS #######
#######################################

# EXCLUDE_FROM_ALL: to not generate install data
add_subdirectory(opcua EXCLUDE_FROM_ALL)


#######################################
############# SOURCE FILES ############
#######################################
if (NOT CN_TESTS_ENABLED)
    add_subdirectory(src)
endif()

#######################################
################ TESTS ################
#######################################
if (CN_TESTS_ENABLED)
    add_subdirectory(tests)
endif()


#######################################
############### INSTALL ###############
#######################################
#if (NOT CN_TESTS_ENABLED)
#    add_subdirectory(doc)
#    add_subdirectory(install)
#endif()
