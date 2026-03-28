cmake_minimum_required(VERSION 3.20) # 2.2 - case insensitive syntax
                                     # 3.13 included policy CMP0077
									 # 3.20 configure_file(... FILE_PERMISSIONS ...)

project(${CN_APP_NAME}-deb VERSION ${PROJECT_VERSION})

set(CN_DEB_PACKAGE "${CN_APP_NAME}_${CN_APP_VERSION}_${CN_APP_ARCH}.deb")
set(CN_DEB_DIR "${CMAKE_BINARY_DIR}/deb")
set(CN_DEB_PACKAGE_DIR "${CN_DEB_DIR}/${CN_APP_NAME}")

message(STATUS "CN-DEB: Create '${CN_DEB_PACKAGE}' for project '${PROJECT_NAME}'")

message(STATUS "CN-DEB: Clear content of '${CN_DEB_DIR}' directory")
file(REMOVE_RECURSE "${CN_DEB_DIR}")

configure_file(${CMAKE_CURRENT_LIST_DIR}/DEBIAN.in.d/control
			   ${CN_DEB_PACKAGE_DIR}/DEBIAN/control
			   FILE_PERMISSIONS OWNER_READ OWNER_WRITE 
                     			GROUP_READ
                     			WORLD_READ)

configure_file(${CMAKE_CURRENT_LIST_DIR}/DEBIAN.in.d/conffiles
			   ${CN_DEB_PACKAGE_DIR}/DEBIAN/conffiles
			   FILE_PERMISSIONS OWNER_READ OWNER_WRITE 
                     			GROUP_READ
                     			WORLD_READ)

configure_file(${CMAKE_CURRENT_LIST_DIR}/DEBIAN.in.d/postinst
			   ${CN_DEB_PACKAGE_DIR}/DEBIAN/postinst
			   FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                     			GROUP_READ GROUP_EXECUTE
                     			WORLD_READ WORLD_EXECUTE)

configure_file(${CMAKE_CURRENT_LIST_DIR}/DEBIAN.in.d/prerm
			   ${CN_DEB_PACKAGE_DIR}/DEBIAN/prerm
			   FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                     			GROUP_READ GROUP_EXECUTE
                     			WORLD_READ WORLD_EXECUTE)

add_custom_target(${PROJECT_NAME} ALL
				  COMMAND ${CMAKE_COMMAND} --install "${CMAKE_BINARY_DIR}" --prefix "${CN_DEB_PACKAGE_DIR}"
				  COMMAND dpkg-deb --build "${CN_DEB_PACKAGE_DIR}" "${CN_DEB_DIR}/${CN_DEB_PACKAGE}"
				  COMMENT "Installing files and build .deb package"
				  USES_TERMINAL
			     )

add_dependencies(${PROJECT_NAME} ${CN_APP_NAME})
