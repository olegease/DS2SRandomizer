cmake_minimum_required( VERSION 3.15..3.28 )
project( DS2SRandomizer VERSION 0.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
# TODO move to presets
set( ${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY
    # "C:/Program Files (x86)/Steam/steamapps/common/Dark Souls II Scholar of the First Sin/Game/randomizer"
    "/home/oleg/snap/steam/common/.steam/steam/steamapps/common/Dark Souls II Scholar of the First Sin/Game/randomizer"
    CACHE PATH
    "Runtime output directory"
)

option( ${PROJECT_NAME}_ENABLE_WARNINGS "Enable warnings" OFF )
option( ${PROJECT_NAME}_BUILD_INTO_RUNTIME "Build into runtime output directory" ON )

add_subdirectory( source )
add_library( cboyo::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )
target_include_directories( ${PROJECT_NAME} PUBLIC include )

if ( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/temp/CMakeLists.txt" )
    add_subdirectory( temp )
endif( )
