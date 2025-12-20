# modbusua MSI Build Configuration
# CMakeLists.txt for Windows installer using WiX Toolset

cmake_minimum_required(VERSION 3.13) # 2.2 - case insensitive syntax
                                     # 3.13 included policy CMP0077

# Only process if on Windows
# Find WiX Toolset
find_program(CN_WIX_CANDLE_EXECUTABLE
    NAMES candle.exe
    PATHS
        "$ENV{WIX}bin"
        "$ENV{ProgramFiles}\\WiX Toolset v3.11\\bin"
        "$ENV{ProgramFiles\(x86\)}\\WiX Toolset v3.11\\bin"
        "$ENV{ProgramFiles}\\WiX Toolset v3.14\\bin"
        "$ENV{ProgramFiles\(x86\)}\\WiX Toolset v3.14\\bin"
    DOC "WiX Toolset candle.exe compiler"
)

find_program(CN_WIX_LIGHT_EXECUTABLE
    NAMES light.exe
    PATHS
        "$ENV{WIX}bin"
        "$ENV{ProgramFiles}\\WiX Toolset v3.11\\bin"
        "$ENV{ProgramFiles\(x86\)}\\WiX Toolset v3.11\\bin"
        "$ENV{ProgramFiles}\\WiX Toolset v3.14\\bin"
        "$ENV{ProgramFiles\(x86\)}\\WiX Toolset v3.14\\bin"
    DOC "WiX Toolset light.exe linker"
)

if(CN_WIX_CANDLE_EXECUTABLE AND CN_WIX_LIGHT_EXECUTABLE)
    message(STATUS "CN-MSI: Found WiX Toolset: ${CN_WIX_CANDLE_EXECUTABLE}")

    project(${CN_APP_NAME}-msi VERSION ${PROJECT_VERSION})

    set(CN_MSI_PACKAGE "${CN_APP_NAME}_${CN_APP_VERSION}_${CN_APP_ARCH}.msi")
    set(CN_MSI_DIR "${CMAKE_BINARY_DIR}/msi")
    set(CN_MSI_PACKAGE_DIR "${CN_MSI_DIR}/${CN_APP_NAME}")

    message(STATUS "CN-MSI: Create '${CN_MSI_PACKAGE}' for project '${PROJECT_NAME}'")

    configure_file(${CMAKE_CURRENT_LIST_DIR}/${CN_APP_NAME}.wxs.in
                   ${CN_MSI_PACKAGE_DIR}/${CN_APP_NAME}.wxs)

    configure_file(${CMAKE_CURRENT_LIST_DIR}/${CN_APP_NAME}service.bat.in
                   ${CN_MSI_PACKAGE_DIR}/${CN_APP_NAME}service.bat)

    # Copy License.rtf file (CMAKE COPY COMMAND)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/License.rtf
         DESTINATION ${CN_MSI_PACKAGE_DIR})

    # For Visual Studio generators, CMAKE_BUILD_TYPE is empty at configure time
    # The actual configuration is determined at build time via --config parameter
    #if(CMAKE_BUILD_TYPE)
    #    message(STATUS "CN-MSI: Build type: ${CMAKE_BUILD_TYPE}")
    #    set(CN_MSI_CONFIG "${CMAKE_BUILD_TYPE}")
    #else()
    #    message(STATUS "CN-MSI: Multi-config generator detected (Visual Studio)")
    #    message(STATUS "CN-MSI: Build configuration will be determined at build time (Release/Debug)")
    #    set(CN_MSI_CONFIG "$<CONFIG>")
    #endif()
    set(CN_MSI_CONFIG "$<CONFIG>")
    
    add_custom_target(${PROJECT_NAME} ALL
            WORKING_DIRECTORY "${CN_MSI_PACKAGE_DIR}"
            COMMAND ${CMAKE_COMMAND} --install "${CMAKE_BINARY_DIR}" --prefix "${CN_MSI_PACKAGE_DIR}" --config "${CN_MSI_CONFIG}"
            COMMAND "${CN_WIX_CANDLE_EXECUTABLE}" -arch x64 "${CN_APP_NAME}.wxs"
            COMMAND "${CN_WIX_LIGHT_EXECUTABLE}" -ext WixUIExtension "*.wixobj" -o "${CN_MSI_PACKAGE}"
            COMMENT "Installing files and build .msi package"
            )
    add_dependencies(${PROJECT_NAME} "${CN_APP_NAME}" "${CN_APP_NAME}-doc")

    #message(STATUS "MSI installer target 'msi' configured")
    #message(STATUS "Run 'cmake --build . --target msi' to build the installer")
    
else()
    message(WARNING "CN-MSI: WiX Toolset not found. MSI installer will not be available.")
    message(STATUS "CN-MSI: To install WiX Toolset, visit: https://wixtoolset.org/releases/")
endif()

