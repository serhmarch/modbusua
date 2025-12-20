# doxygen Configuration

cmake_minimum_required(VERSION 3.13) # 2.2 - case insensitive syntax
                                     # 3.13 included policy CMP0077

if (WIN32)
    message(STATUS "CN-DOC: Try to find Doxygen on Windows")
    find_program(CN_DOXYGEN_EXECUTABLE
        NAMES doxygen.exe
        PATHS
            "$ENV{ProgramFiles}\\Doxygen\\bin"
            "$ENV{ProgramFiles\(x86\)}\\Doxygen\\bin"
        DOC "Doxygen documentation generator"
        NO_CACHE
    )
else()
    message(STATUS "CN-DOC: Try to find Doxygen on Unix-like OS")
    find_program(CN_DOXYGEN_EXECUTABLE
        NAME doxygen
        DOC "Doxygen documentation generator"
        NO_CACHE
    )
endif()

if(CN_DOXYGEN_EXECUTABLE)
    message(STATUS "CN-DOC: Found Doxygen: ${CN_DOXYGEN_EXECUTABLE}")

    project(${CN_APP_NAME}-doc VERSION ${PROJECT_VERSION})

    set(CN_DOC_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}")
    set(CN_DOC_DIR "${CMAKE_BINARY_DIR}/doc")
    set(CN_DOC_FILE_NAME "Doxyfile")
    set(CN_DOC_FILE "${CN_DOC_DIR}/${CN_DOC_FILE_NAME}")
    set(CN_DOC_DEV_FILE_NAME "Doxyfile-dev")
    set(CN_DOC_DEV_FILE "${CN_DOC_DIR}/${CN_DOC_DEV_FILE_NAME}")

    message(STATUS "CN-DOC: Create '${CN_DOC_FILE}' for project '${PROJECT_NAME}'")

    configure_file(${CMAKE_CURRENT_LIST_DIR}/${CN_DOC_FILE_NAME}.in
                   ${CN_DOC_DIR}/${CN_DOC_FILE_NAME})

    #configure_file(${CMAKE_CURRENT_LIST_DIR}/${CN_DOC_DEV_FILE_NAME}.in
    #               ${CN_DOC_DIR}/${CN_DOC_DEV_FILE_NAME})


    SET (CN_DOC_TARGET "${CN_APP_NAME}-doc")    
    add_custom_target(${CN_DOC_TARGET} ALL
            WORKING_DIRECTORY "${CN_DOC_DIR}"
            COMMAND "${CN_DOXYGEN_EXECUTABLE}" "${CN_DOC_FILE}"
            COMMENT "Build documentation"
            )
    add_dependencies(${CN_DOC_TARGET} ${CN_APP_NAME})

    #SET (CN_DOC_DEV_TARGET "${CN_APP_NAME}-doc-dev")    
    #add_custom_target(${CN_DOC_DEV_TARGET} ALL
    #        WORKING_DIRECTORY "${CN_DOC_DIR}"
    #        COMMAND "${CN_DOXYGEN_EXECUTABLE}" "${CN_DOC_DEV_FILE}"
    #        COMMENT "Build documentation"
    #        )
    #add_dependencies(${CN_DOC_DEV_TARGET} ${CN_APP_NAME})

else()
    message(WARNING "CN-DOC: Doxygen not found. Documentation will not be generated.")
    message(STATUS "CN-DOC: To install Doxygen, visit: https://www.doxygen.nl/download.html")
endif()

