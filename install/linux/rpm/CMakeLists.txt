cmake_minimum_required(VERSION 3.20) # 2.2 - case insensitive syntax
                                     # 3.13 included policy CMP0077
									 # 3.20 configure_file(... FILE_PERMISSIONS ...)

project(${CN_APP_NAME}-rpm VERSION ${PROJECT_VERSION})

set(CN_RPM_PACKAGE "${CN_APP_NAME}_${CN_APP_VERSION}_${CN_APP_ARCH}.rpm")
set(CN_RPM_SPEC_FILE "${CN_APP_NAME}.spec")
set(CN_RPM_SPEC_DIR "$ENV{HOME}/rpmbuild/SPECS")
set(CN_RPM_SPEC_PATH "${CN_RPM_SPEC_DIR}/${CN_RPM_SPEC_FILE}")

message(STATUS "CN-RPM: Configure file '${CN_RPM_SPEC_PATH}' for project '${PROJECT_NAME}'")

configure_file(${CMAKE_CURRENT_LIST_DIR}/${CN_RPM_SPEC_FILE}.in
			   ${CN_RPM_SPEC_PATH}
			   FILE_PERMISSIONS OWNER_READ OWNER_WRITE 
                     			GROUP_READ
                     			WORLD_READ)


add_custom_target(${PROJECT_NAME} ALL
				  COMMAND rpmbuild -bb "${CN_RPM_SPEC_PATH}"
				  COMMENT "Build .rpm package"
				  USES_TERMINAL
			     )

add_dependencies(${PROJECT_NAME} ${CN_APP_NAME})
