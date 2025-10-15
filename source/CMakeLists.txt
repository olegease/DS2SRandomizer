add_library( ${PROJECT_NAME}__lib )
target_sources( ${PROJECT_NAME}__lib PRIVATE
    modules/param_editor.hpp
    modules/utils.hpp
)
target_sources( ${PROJECT_NAME}__lib PRIVATE
    itemrando.cpp
    # ./param_editor.cpp
    randomizer.cpp
)

if( ${PROJECT_NAME}_ENABLE_WARNINGS )
    if ( MSVC )
        target_compile_options( ${PROJECT_NAME}__lib PRIVATE /W4 )
    else( )
        target_compile_options( ${PROJECT_NAME}__lib PRIVATE -Wall -Wextra -Wpedantic )
    endif( )
endif( )

if ( ${PROJECT_NAME}_BUILD_EXECUTABLE )
    add_executable( ${PROJECT_NAME} )
    target_sources( ${PROJECT_NAME} PRIVATE
        terminal.cxx
    )
    target_link_libraries( ${PROJECT_NAME} PRIVATE ${PROJECT_NAME}__lib )
    if ( ${PROJECT_NAME}_BUILD_INTO_RUNTIME )
        set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY} )
    endif( )
endif( )
