add_library( ${PROJECT_NAME} )
target_sources( ${PROJECT_NAME} PRIVATE
    modules/param_editor.hpp
    modules/utils.hpp
)

target_sources( ${PROJECT_NAME} PRIVATE
    itemrando.cpp
    # ./param_editor.cpp
    randomizer.cpp
)

if( ${PROJECT}_ENABLE_WARNINGS AND MSVC )
    target_compile_options( ${PROJECT_NAME} PRIVATE /W4 )
endif( )

add_executable( ds2srand )
target_sources( ds2srand PRIVATE
    terminal.cpp
)
target_link_libraries( ds2srand PRIVATE ${PROJECT_NAME} )
if ( ${PROJECT_NAME}_BUILD_INTO_RUNTIME )
    set_target_properties( ds2srand PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY} )
endif( )
