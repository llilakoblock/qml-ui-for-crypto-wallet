find_package(Qt6
    REQUIRED COMPONENTS
    Core
    HttpServer
    WebChannel
)

set(SOURCES
    src/FacetecServer.cpp
    src/WebSocketTransport.h src/WebSocketTransport.cpp
    src/WebChannelBackend.h src/WebChannelBackend.cpp
)

set(PUBLIC_HEADERS
    include/biometric/FacetecServer.h
)

add_library(Biometric STATIC ${SOURCES} ${PUBLIC_HEADERS})

qt_add_resources(Biometric "facetec_resources"
    PREFIX
        "/facetec"
    BASE
        "resources/facetec-client"
    FILES
        resources/facetec-client/pidf
        resources/facetec-client/index.html
        resources/facetec-client/css/style.css
        resources/facetec-client/sampleAppController.js
        resources/facetec-client/Config.js
        resources/facetec-client/utilities/DeveloperStatusMessages.js
        resources/facetec-client/utilities/AdditionalScreens.js
        resources/facetec-client/utilities/SampleAppUtilities.js
        resources/facetec-client/utilities/ThemeHelpers.js
        resources/facetec-client/utilities/SoundFileUtilities.js
        resources/facetec-client/utilities/SampleAppUIFunctions.js
        resources/facetec-client/processors/PhotoIDScanProcessor.js
        resources/facetec-client/processors/LivenessCheckProcessor.js
        resources/facetec-client/processors/AuthenticateProcessor.js
        resources/facetec-client/processors/PhotoIDMatchProcessor.js
        resources/facetec-client/processors/EnrollmentProcessor.js
        resources/facetec-client/sample-app-resources/images/FaceTec_cancel.png
        resources/facetec-client/sample-app-resources/images/favicon.ico
        resources/facetec-client/sample-app-resources/images/vocal_off.png
        resources/facetec-client/sample-app-resources/images/vocal_full.png
        resources/facetec-client/sample-app-resources/images/FaceTec_Info.png
        resources/facetec-client/sample-app-resources/images/upgrade.png
        resources/facetec-client/sample-app-resources/images/vocal_minimal.png
        resources/facetec-client/sample-app-resources/images/FaceTec_Logo_White.png
        resources/facetec-client/sample-app-resources/images/themes/FaceTec_ideal_5.png
        resources/facetec-client/sample-app-resources/images/themes/FaceTec_ideal_4.png
        resources/facetec-client/sample-app-resources/images/themes/FaceTec_ideal_1.png
        resources/facetec-client/sample-app-resources/images/themes/FaceTec_ideal_3.png
        resources/facetec-client/sample-app-resources/images/themes/FaceTec_ideal_2.png
        resources/facetec-client/sample-app-resources/images/themes/pseudo-fullscreen/activity_indicator_faded_black.png
        resources/facetec-client/sample-app-resources/images/themes/pseudo-fullscreen/document_offwhite.png
        resources/facetec-client/sample-app-resources/images/themes/pseudo-fullscreen/single_chevron_left_black.png
        resources/facetec-client/sample-app-resources/images/themes/pseudo-fullscreen/review_offwhite.png
        resources/facetec-client/sample-app-resources/images/themes/pseudo-fullscreen/camera_shutter_black.png
        resources/facetec-client/sample-app-resources/images/themes/pseudo-fullscreen/single_chevron_left_offwhite.png
        resources/facetec-client/sample-app-resources/images/themes/pseudo-fullscreen/document_offblack.png
        resources/facetec-client/sample-app-resources/images/themes/pseudo-fullscreen/review_offblack.png
        resources/facetec-client/sample-app-resources/images/FaceTec_Logo.png
        resources/facetec-client/sample-app-resources/FaceTec_OCR_Customization.json
        resources/facetec-client/sample-app-resources/Vocal_Guidance_Audio_Files/vocal_guidance_off.mp3
        resources/facetec-client/sample-app-resources/Vocal_Guidance_Audio_Files/please_frame_your_face_sound_file.mp3
        resources/facetec-client/sample-app-resources/Vocal_Guidance_Audio_Files/facescan_successful_sound_file.mp3
        resources/facetec-client/sample-app-resources/Vocal_Guidance_Audio_Files/please_move_closer_sound_file.mp3
        resources/facetec-client/sample-app-resources/Vocal_Guidance_Audio_Files/please_retry_sound_file.mp3
        resources/facetec-client/sample-app-resources/Vocal_Guidance_Audio_Files/please_press_button_sound_file.mp3
        resources/facetec-client/sample-app-resources/Vocal_Guidance_Audio_Files/uploading_sound_file.mp3
        resources/facetec-client/sample-app-resources/Vocal_Guidance_Audio_Files/vocal_guidance_on.mp3
        resources/facetec-client/core-sdk/FaceTec_images/FaceTec_cancel.png
        resources/facetec-client/core-sdk/FaceTec_images/FaceTec_id_scan_watermark.png
        resources/facetec-client/core-sdk/FaceTec_images/FaceTec_review.png
        resources/facetec-client/core-sdk/FaceTec_images/FaceTec_ideal.png
        resources/facetec-client/core-sdk/FaceTec_images/FaceTec_document.png
        resources/facetec-client/core-sdk/FaceTec_images/FaceTec_your_app_logo.png
        resources/facetec-client/core-sdk/FaceTec_images/FaceTec_enter_fullscreen.png
        resources/facetec-client/core-sdk/FaceTec_images/FaceTec_camera.png
        resources/facetec-client/core-sdk/FaceTec_images/FaceTec_branding_logo_id_check.png
        resources/facetec-client/core-sdk/FaceTecSDK.js/FaceTecSession.d.ts
        resources/facetec-client/core-sdk/FaceTecSDK.js/FaceTecLogging.d.ts
        resources/facetec-client/core-sdk/FaceTecSDK.js/FaceTecPublicApi.d.ts
        resources/facetec-client/core-sdk/FaceTecSDK.js/FaceTecCustomization.d.ts
        resources/facetec-client/core-sdk/FaceTecSDK.js/ThirdPartyLicenses.txt
        resources/facetec-client/core-sdk/FaceTecSDK.js/FaceTecSDK.d.ts
)

qt_add_resources(Biometric "facetec_blob_resources"
    PREFIX
        "/facetec"
    BASE
        "resources/facetec-client"
    FILES
        resources/facetec-client/sample-app-resources/js/very-old-browser-check.js
        resources/facetec-client/sample-app-resources/js/third-party/jquery-3.3.1.min.js
        resources/facetec-client/sample-app-resources/js/third-party/qwebchannel.js
        resources/facetec-client/core-sdk/FaceTecSDK.js/resources/011c90516755d702cfb4205ca9d93e21fe6683b8.data
        resources/facetec-client/core-sdk/FaceTecSDK.js/resources/011c90516755d702cfb4205ca9d93e21fe6683b8.js
        resources/facetec-client/core-sdk/FaceTecSDK.js/resources/011c90516755d702cfb4205ca9d93e21fe6683b8_cache.wasm
        resources/facetec-client/core-sdk/FaceTecSDK.js/resources/011c90516755d702cfb4205ca9d93e21fe6683b8.wasm
        resources/facetec-client/core-sdk/FaceTecSDK.js/resources/Face75bc1215a068466281685bd80c04c12a.js
        resources/facetec-client/core-sdk/FaceTecSDK.js/FaceTecSDK.js
)

target_include_directories(Biometric
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src
        include/biometric
)

target_link_libraries(Biometric
    PRIVATE
        Qt6::Core
        Qt6::QmlIntegration
    PUBLIC
        Qt6::HttpServer
        Qt6::WebChannel
)
