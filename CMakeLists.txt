cmake_minimum_required(VERSION 3.15)

project(BallzzyArchetype VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the plugin
juce_add_plugin(BallzzyArchetype
        COMPANY_NAME "BALL"
        COMPANY_WEBSITE "http://juce.com"
        COMPANY_EMAIL ""
        PLUGIN_MANUFACTURER_CODE Ball
        PLUGIN_CODE NAmp
        FORMATS VST3 Standalone
        PRODUCT_NAME "Ballzzy Amp"
        BUNDLE_ID "com.BALL.BallzzyArchetype"

        VST3_CATEGORIES Fx
        # WebViews use web browser features: find necessary packages in the system to link against
        NEEDS_WEB_BROWSER TRUE
        # This will force JUCE to look for the WebView2 NuGet package on Windows
        NEEDS_WEBVIEW2 TRUE
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        JUCE_WEB_BROWSER
        PLUGIN_NAME "Ballzzy Amp"
        PLUGIN_DESCRIPTION "Neural Amp Modeler Plugin"
        VST3_AUTO_MANIFEST FALSE
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
        JUCE_PRODUCT_NAME="${PRODUCT_NAME}"
        JUCE_COMPANY_NAME="${COMPANY_NAME}"
        JUCE_PRODUCT_VERSION="${PROJECT_VERSION}")

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        "C:/Users/Marius/.nuget/packages/microsoft.web.webview2/1.0.3485.44/build/native/include/"
)
# Mark JUCE headers as system headers to suppress warnings in them
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${JUCE_MODULES_DIR})

# Folder where web UI data reside
set(WEBVIEW_FILES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ui/public")

# Copy JUCE frontend library to plugin UI files
file(COPY "${JUCE_MODULES_DIR}/juce_gui_extra/native/javascript/" DESTINATION "${WEBVIEW_FILES_SOURCE_DIR}/js/juce/")

# Zip WebView files
set(WEBVIEW_FILES_ZIP_NAME "webview_files.zip")
set(TARGET_WEBVIEW_FILES_ZIP_PATH "${CMAKE_BINARY_DIR}/${WEBVIEW_FILES_ZIP_NAME}")
cmake_path(ABSOLUTE_PATH WEBVIEW_FILES_SOURCE_DIR NORMALIZE OUTPUT_VARIABLE PUBLIC_PATH )
cmake_path(GET PUBLIC_PATH PARENT_PATH WORKING_DIRECTORY)
# TODO : devrait check si pas exists // Voir comment rendre les plugins indépéndants !
add_custom_target(ZipWebViewFiles
        COMMAND
        ${CMAKE_COMMAND} -E tar cvf
        "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
        --format=zip
        "${PUBLIC_PATH}"
        BYPRODUCTS
        "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
        WORKING_DIRECTORY
        "${WORKING_DIRECTORY}"
        COMMENT "Zipping WebView files..."
        VERBATIM
)

message("FAUST FAUST FAUSE COMPILE : faust -i -light -a .\\faustMinimal.h .\\DelayPedalengine.dsp  -o DelayPedalFaust.h --namespace DelayPedal -cn DelayPedal ")
message("faust -i .\\CompressorPedalengine.dsp -a .\\faustMinimal.h -lang cpp -i -fpga-mem-th 4 -ct 1 -cn CompressorPedalEngine -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -o CompressorPedalFaust.h")
# Pass the prefix of the zipped files (e.g., "public/") to C++
cmake_path(GET PUBLIC_PATH FILENAME ZIPPED_FILES_PREFIX)
target_compile_definitions(${PROJECT_NAME} PRIVATE ZIPPED_FILES_PREFIX="${ZIPPED_FILES_PREFIX}/")

# Package web UI sources as binary data
juce_add_binary_data(WebViewFiles
        HEADER_NAME WebViewFiles.h
        NAMESPACE webview_files
        SOURCES ${TARGET_WEBVIEW_FILES_ZIP_PATH}
)
add_dependencies(WebViewFiles ZipWebViewFiles)
# Sets the include directories of the plugin project.

# Mark JUCE headers as system headers to suppress warnings in them
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${JUCE_MODULES_DIR})

# Folder where web UI data reside
set(WEBVIEW_FILES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ui/public")

# Copy JUCE frontend library to plugin UI files
file(COPY "${JUCE_MODULES_DIR}/juce_gui_extra/native/javascript/" DESTINATION "${WEBVIEW_FILES_SOURCE_DIR}/js/juce/")

# Zip WebView files
set(WEBVIEW_FILES_ZIP_NAME "webview_files.zip")
set(TARGET_WEBVIEW_FILES_ZIP_PATH "${CMAKE_BINARY_DIR}/${WEBVIEW_FILES_ZIP_NAME}")
cmake_path(ABSOLUTE_PATH WEBVIEW_FILES_SOURCE_DIR NORMALIZE OUTPUT_VARIABLE PUBLIC_PATH )
cmake_path(GET PUBLIC_PATH PARENT_PATH WORKING_DIRECTORY)
target_compile_definitions(${PROJECT_NAME} PRIVATE ZIPPED_FILES_PREFIX="${ZIPPED_FILES_PREFIX}/")
# -----------------------------------------------------------------------
# FIX: juce_add_binary_data MUST be declared before target_link_libraries
#      so that the BallzzyArchetypeData target exists when it is referenced.
# -----------------------------------------------------------------------
juce_add_binary_data(BallzzyArchetypeData
        SOURCES
        # Images

        #Source/ressources/namModels/Fender/Fender.nam
        Source/ressources/namModels/Fender/Marshall.nam
        Source/ressources/namModels/Fender/Fender_Deluxe_Reverb.nam
        Source/ressources/namModels/Fender/Peavey_5150_Mesa_cab_SM57.nam
)

# Link binary data and JUCE modules to the plugin
target_link_libraries(BallzzyArchetype
        PRIVATE
        BallzzyArchetypeData
        NAM
        WebViewFiles
        AudioDSPTools
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_dsp
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Add source files
target_sources(BallzzyArchetype
        PRIVATE
        source/Main.cpp
        source/PluginAudioProcessor.cpp
        source/PluginAudioProcessor.h
        # DSP files
        source/dsp/faustParameterMappers/PedalDelayMap.h
        source/dsp/faustParameterMappers/PedalFuzzMap.h
        source/dsp/faustParameterMappers/PedalReverbMap.h
        source/dsp/faustParameterMappers/PostEqMap.h
        source/dsp/faustParameterMappers/PedalOverdriveMap.h
        source/dsp/faustParameterMappers/AmpStackMap.h


        source/dsp/ParameterSetup.h
        source/dsp/ParameterSetup.cpp
        source/dsp/Processor.cpp
        source/dsp/Processor.h
        source/dsp/ParameterIDs.hpp
        source/dsp/paramsDeclaration.h

        source/dsp/TabsProcessor/AmpProcessor.h
        source/dsp/TabsProcessor/InputPedalProcessor.h
        source/dsp/TabsProcessor/OutputPedalProcessor.h
        source/dsp/TabsProcessor/PostEqProcessor.h
        source/dsp/TabsProcessor/AmpProcessor.cpp
        source/dsp/TabsProcessor/InputPedalProcessor.cpp
        source/dsp/TabsProcessor/OutputPedalProcessor.cpp
        source/dsp/TabsProcessor/PostEqProcessor.cpp
        # DSP Bones
        #source/dsp/Bones/FaustMultiheadFeedback.h
        #source/dsp/Bones/FaustHpLp.h
        #source/dsp/Bones/DuckingEngine.h
        # TODO : new faust bones soon

        source/dsp/Bones/FuzzPedalFaust.h
         source/dsp/Bones/OverdrivePedalFaust.h
        # wesh sans les bones ca marche ??!
        source/dsp/Bones/CompressorPedalFaust.h
        source/dsp/Bones/EqPedalFaust.h
        source/dsp/Bones/ReverbPedalFaust.h
        source/dsp/Bones/DelayPedalFaust.h
        source/dsp/Bones/AmpToneStackFaust.h
        # UI
        source/ui/gui.cpp
        source/ui/gui.h

        source/ui/webSliderAttachments.h

        source/service/PresetManager.h
        source/service/PresetManager.cpp



        # UI Compon
        #  ${INCLUDE_DIR}/PluginEditor.h
        #   ${INCLUDE_DIR}/PluginProcessor.h
)
# Preprocessor definitions
target_compile_definitions(BallzzyArchetype
        PUBLIC
        JUCE_TARGET_HAS_BINARY_DATA=1
        JUCE_WEB_BROWSER=1
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        # This will enable WebView2 as the WebView backend on Windows
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1

)

# Add header search paths for NAM library
target_include_directories(BallzzyArchetype
        PRIVATE
        "C:/Users/Marius/Documents/JUCE/libraries/NeuralAmpModelerCore"
)

# Platform-specific settings
if(MSVC)
    target_compile_options(BallzzyArchetype PRIVATE /bigobj)
endif()

# Custom post-build command to copy VST3 to specific location
if(WIN32)
    set(VST3_DESTINATION_PATH "D:/vstcreation/BallzzyAmp.vst3/Contents/x86_64-win")

    add_custom_command(TARGET BallzzyArchetype_VST3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${VST3_DESTINATION_PATH}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:BallzzyArchetype_VST3>"
            "${VST3_DESTINATION_PATH}/$<TARGET_FILE_NAME:BallzzyArchetype_VST3>"
            COMMENT "Creating VST3 Bundle structure in D:/vstcreation"
    )
endif()