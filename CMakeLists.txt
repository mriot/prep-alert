    cmake_minimum_required(VERSION 3.25)
    project(PrepAlert LANGUAGES CXX RC) # c++ and resource files

    # Create shared library target
    add_library(${PROJECT_NAME} SHARED)

    # Require C++20 standard
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

    # Set output directory for the built DLL
    set_target_properties(${PROJECT_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    # Collect all source files recursively
    file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
            src/*.cpp
            src/*.h
    )

    # Third-party source files to include explicitly
    set(THIRD_PARTY_SOURCES
            third-party/imgui/imgui.cpp
            third-party/imgui/imgui_draw.cpp
            third-party/imgui/imgui_tables.cpp
            third-party/imgui/imgui_widgets.cpp
            third-party/gw2re/Game/PropContext.cpp
            third-party/gw2re/Game/Char/Character.cpp
            third-party/gw2re/Game/Char/ChBuffMgr.cpp
            third-party/gw2re/Game/Char/ChCliContext.cpp
            third-party/gw2re/Game/Char/ChCombatant.cpp
            third-party/gw2re/Util/Validation.cpp
    )

    # Resource files
    set(RESOURCE_FILES
            src/PrepAlert.rc
    )

    # Add sources to target
    target_sources(${PROJECT_NAME} PRIVATE
            ${SRC_FILES}
            ${THIRD_PARTY_SOURCES}
            ${RESOURCE_FILES}
    )

    # Always rebuild resource on each build
    set_source_files_properties(src/PrepAlert.rc PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/maps.json")

    # Include directories
    target_include_directories(${PROJECT_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/third-party
            ${CMAKE_CURRENT_SOURCE_DIR}/third-party/GW2RE
    )

    # Compiler settings for MSVC
    if (MSVC)
        # Disable warnings about unsafe C functions
        # target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)

        # Define DEBUG macro in Debug builds
        target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

        # Keep warning level modest to avoid churn; enable multi-processor compilation
        target_compile_options(${PROJECT_NAME} PRIVATE /W3 /MP)

        # Compiler optimizations
        target_compile_options(${PROJECT_NAME} PRIVATE
                $<$<CONFIG:Release>:/O2 /GL /DNDEBUG /fp:fast>
        )

        # Linker optimizations
        target_link_options(${PROJECT_NAME} PRIVATE
                $<$<CONFIG:Release>:/LTCG /OPT:REF /OPT:ICF>
        )

        # Use the DLL runtime library
        set_property(TARGET ${PROJECT_NAME} PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
        )
    endif ()

    # --- Custom build steps ---

    if (WIN32)
        # generate Version.h before build
        add_custom_target(generate_version ALL
                COMMAND cmd /C "${CMAKE_SOURCE_DIR}/version.bat"
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        )
        add_dependencies(${PROJECT_NAME} generate_version)

        # patch map data before build
        add_custom_target(patch_map_data ALL
                COMMAND cmd /C "uv run ${CMAKE_SOURCE_DIR}/scripts/map_data_patcher/main.py"
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        )
        add_dependencies(${PROJECT_NAME} patch_map_data)
    endif ()

    # Post-build: copy DLL to GW2 addons folder using CMake -E (portable within Windows/MSYS)
    if (WIN32)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:${PROJECT_NAME}>"
                "C:/Guild Wars 2/addons/${PROJECT_NAME}.dll.update"
                COMMAND ${CMAKE_COMMAND} -E echo "Copied ${PROJECT_NAME} to Guild Wars 2 addons folder."
                VERBATIM
        )
    endif ()