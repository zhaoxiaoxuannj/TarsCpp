cmake_minimum_required(VERSION 3.10)

project(tars-cpp)

#-------------------------------------------------------------------

if("${TARS_CPP_COMMON}" STREQUAL "")

    set(TARS_CPP_COMMON "1")

    include("${PROJECT_SOURCE_DIR}/cmake/GenVersion.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/Common.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/BuildTarsCpp.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/Thirdparty.cmake")

    include("${PROJECT_SOURCE_DIR}/cmake/ParseAsm.cmake")

endif()

include(FetchContent)
if (NOT DEFINED URL_BASE)
    set(URL_BASE "github.com")
    # set(URL_BASE "github.com.cnpmjs.org")
endif()
set(TARS_PROTOCOL tarsprotocol)
set(TARS_PROTOCOL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/servant/protocol)
FetchContent_Declare(${TARS_PROTOCOL}
    GIT_REPOSITORY https://${URL_BASE}/FISCO-BCOS/TarsProtocol.git
    GIT_TAG a5b27e917ad7633bcae241282d3396b47a700159
    SOURCE_DIR ${TARS_PROTOCOL_PATH}
)
FetchContent_MakeAvailable(${TARS_PROTOCOL})

# set(major "")
# set(minor "")
# set(patch "")
# get_version(major minor patch)

# set(TARS_VERSION ${major}.${minor}.${patch})

add_definitions(-DTARS_VERSION="${TARS_VERSION}")

message("TARS_VERSION:              ${TARS_VERSION}")

set(CLEAR_INCLUDE "clear-install.cmake")
FILE(WRITE ${CLEAR_INCLUDE} "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_INSTALL_PREFIX}/include)\n")
install(SCRIPT ${CLEAR_INCLUDE})

#-------------------------------------------------------------------

include_directories(${PROJECT_SOURCE_DIR}/util/include)

include_directories(${PROJECT_SOURCE_DIR}/servant)
include_directories(${PROJECT_SOURCE_DIR}/servant/protocol)

IF(WIN32)
include_directories(${PROJECT_SOURCE_DIR}/util/src/epoll_windows)
ENDIF()

add_subdirectory(util)
add_subdirectory(tools)
add_subdirectory(servant)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_SOURCE_DIR}/cmake/tarscpp-config.cmake.in"
  "${CMAKE_BINARY_DIR}/tarscpp-config.cmake"
  INSTALL_DESTINATION "share/tarscpp"
)

install(
  FILES "${CMAKE_BINARY_DIR}/tarscpp-config.cmake"
  DESTINATION "share/tarscpp"
)

install(
    EXPORT tarsutil-targets
    DESTINATION share/tarscpp
    NAMESPACE tarscpp::
)

install(
    EXPORT tarsparse-targets
    DESTINATION share/tarscpp
    NAMESPACE tarscpp::
)

install(
    EXPORT tarsservant-targets
    DESTINATION share/tarscpp
    NAMESPACE tarscpp::
)

if (TESTS)
    add_subdirectory(examples)
    add_subdirectory(unit-test)
endif()
