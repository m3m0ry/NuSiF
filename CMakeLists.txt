cmake_minimum_required(VERSION 3.3)

project("nusif")

SET(MAJOR_VERSION 1)
SET(MINOR_VERSION 0)
SET(PATCH_VERSION 0)

# Include doxygen
#find_package(Doxygen)
#if(DOXYGEN_FOUND)
#   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
#   add_custom_target(doc
#   ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#   COMMENT "Generating API documentation with Doxygen" VERBATIM
#   )
#endif(DOXYGEN_FOUND)

# Manual add sources
#set(SRC_FILES src/mainapp.cpp src/Student.cpp)
# Wildcard addtion of sources

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
#if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
#   message(FATAL_ERROR
#      "${PROJECT_NAME} requires an out of source Build."
#      "Please create a separate binary directory and run CMake there")
#endif()



# Use OpenMP if availible
find_package(OpenMP)
if(OPENMP_FOUND)
   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
   set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

file(GLOB SRC_FILES "src/*.cc")

# Set include folder for headers
include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-format")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -pedantic -Wextra -Wno-format")

#---------
#-Project-
#---------
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Need c++11
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
# Compile flags
target_compile_options(${PROJECT_NAME} PUBLIC "-O3")


install(TARGETS ${PROJECT_NAME} DESTINATION bin)


#Debian package
IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
   INCLUDE(InstallRequiredSystemLibraries)
    
   #SET(CPACK_SET_DESTDIR "on")
   
   SET(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
   SET(CPACK_GENERATOR "DEB")
    
   SET(CPACK_PACKAGE_DESCRIPTION "short description")
   SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "long description bla bla bla... bah")
   SET(CPACK_PACKAGE_VENDOR "Hr0m")
   SET(CPACK_PACKAGE_CONTACT "Jan Hoenig <hrominium@gmail.com>")
   SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
   SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
   SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
   SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
   SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
    
   SET(CPACK_DEBIAN_PACKAGE_DEPENDS "")
    
   SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
   SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
   SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
   SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS "paraview")
    
   SET(CPACK_COMPONENTS_ALL Libraries ApplicationData)
   INCLUDE(CPack)
    
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
