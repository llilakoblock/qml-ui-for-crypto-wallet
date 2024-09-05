var ThemeHelpers = (function () {
    // Set the default theme
    var currentTheme = "Bitcoin Exchange";
    var themeResourceDirectory = "../../sample-app-resources/images/themes/";
    function setAppTheme(theme) {
        Config.currentCustomization = getCustomizationForTheme(theme);
        Config.currentLowLightCustomization = getLowLightCustomizationForTheme(theme);
        Config.currentDynamicDimmingCustomization = getDynamicDimmingCustomizationForTheme(theme);
        FaceTecSDK.setCustomization(Config.currentCustomization);
        FaceTecSDK.setLowLightCustomization(Config.currentLowLightCustomization);
        FaceTecSDK.setDynamicDimmingCustomization(Config.currentDynamicDimmingCustomization);
    }
    function getCustomizationForTheme(theme) {
        var currentCustomization = new FaceTecSDK.FaceTecCustomization();
        // Add sound customization to the new theme customization
        var soundFileUtilities = new SoundFileUtilities();
        currentCustomization = soundFileUtilities.setVocalGuidanceSoundFiles(currentCustomization);
        var retryScreenSlideshowImages = [themeResourceDirectory + "FaceTec_ideal_1.png", themeResourceDirectory + "FaceTec_ideal_2.png", themeResourceDirectory + "FaceTec_ideal_3.png", themeResourceDirectory + "FaceTec_ideal_4.png", themeResourceDirectory + "FaceTec_ideal_5.png"];

        var primaryColor = "rgb(250, 250, 250)";
        var primaryColorLight = "rgb(250, 250, 250)";
        var secondaryColor = "rgb(150, 159, 168)";
        var backgroundColor = "rgb(67, 66, 144)";
        var font = "SF Compact Text";
        var shadow = "0px 3px 10px black";
        var buttonColor = "rgb(12, 12, 35)";
        var buttonHoverColor = "rgb(17, 17, 49)";
        var buttonDisabledColor = "rgba(106, 108, 141, 0.70)"
        var buttonDisabledHoverColor = "rgba(130, 132, 161, 0.70)"
        var textPrimaryColor = "rgb(250, 250, 250)";
        var textSecondaryColor = "rgb(150, 159, 168)";

        var activityIndicatorSVG = document.createElementNS("http://www.w3.org/2000/svg", "svg");
        activityIndicatorSVG.setAttribute("viewBox", "0 0 52 52");
        activityIndicatorSVG.classList.add("pseudo-fullscreen-activity-indicator-svg");
        activityIndicatorSVG.innerHTML = "<circle class='path' cx='26' cy='26' r='22'></circle>";

        var uploadActivityIndicatorSVG = document.createElementNS("http://www.w3.org/2000/svg", "svg");
        uploadActivityIndicatorSVG.setAttribute("viewBox", "0 0 52 52");
        uploadActivityIndicatorSVG.classList.add("pseudo-fullscreen-activity-indicator-svg");
        uploadActivityIndicatorSVG.innerHTML = "<circle class='path' cx='26' cy='26' r='22'></circle>";

        var successResultAnimationSVG = document.createElementNS("http://www.w3.org/2000/svg", "svg");
        successResultAnimationSVG.setAttribute("viewBox", "0 0 52 52");
        successResultAnimationSVG.classList.add("pseudo-fullscreen-success-svg");
        successResultAnimationSVG.innerHTML = "<circle class='circlePath' cx='26' cy='26' r='22'></circle><path class='checkmarkPath' d='M14.1 27.2l7.1 7.2 16.7-16.8'></path>";

        var unsuccessResultAnimationSVG = document.createElementNS("http://www.w3.org/2000/svg", "svg");
        unsuccessResultAnimationSVG.setAttribute("viewBox", "0 0 52 52");
        unsuccessResultAnimationSVG.classList.add("pseudo-fullscreen-unsuccess-svg");
        unsuccessResultAnimationSVG.innerHTML = "<circle class='circlePath' cx='26' cy='26' r='22'></circle><line class='crossPath1' x1='18' y1='18' x2='34' y2='34'></line><line class='crossPath2' x1='34' y1='18' x2='18' y2='34'></line>";

        // Initial Loading Animation Customization
        currentCustomization.initialLoadingAnimationCustomization.customAnimation = activityIndicatorSVG;
        currentCustomization.initialLoadingAnimationCustomization.animationRelativeScale = 1.0;
        currentCustomization.initialLoadingAnimationCustomization.backgroundColor = backgroundColor;
        currentCustomization.initialLoadingAnimationCustomization.foregroundColor = textPrimaryColor;
        currentCustomization.initialLoadingAnimationCustomization.messageTextColor = textPrimaryColor;
        currentCustomization.initialLoadingAnimationCustomization.messageFont = font;
        // Frame Customization
        currentCustomization.frameCustomization.backgroundColor = backgroundColor;
        currentCustomization.frameCustomization.borderColor = "transparent";
        currentCustomization.frameCustomization.borderWidth = "0px";
        currentCustomization.frameCustomization.borderCornerRadius = "0px";
        currentCustomization.frameCustomization.shadow = "transparent";
        // Overlay Customization
        currentCustomization.overlayCustomization.backgroundColor = backgroundColor;
        currentCustomization.overlayCustomization.showBrandingImage = false;
        currentCustomization.overlayCustomization.brandingImage = "";
        // Guidance Customization
        currentCustomization.guidanceCustomization.backgroundColors = backgroundColor;
        currentCustomization.guidanceCustomization.foregroundColor = textPrimaryColor;
        currentCustomization.guidanceCustomization.headerFont = font;
        currentCustomization.guidanceCustomization.subtextFont = font;
        currentCustomization.guidanceCustomization.buttonFont = font;
        currentCustomization.guidanceCustomization.buttonTextNormalColor = textPrimaryColor;
        currentCustomization.guidanceCustomization.buttonBackgroundNormalColor = buttonColor;
        currentCustomization.guidanceCustomization.buttonTextHighlightColor = textPrimaryColor;
        currentCustomization.guidanceCustomization.buttonBackgroundHighlightColor = buttonHoverColor;
        currentCustomization.guidanceCustomization.buttonTextDisabledColor = textSecondaryColor;
        currentCustomization.guidanceCustomization.buttonBackgroundDisabledColor = buttonDisabledColor;
        currentCustomization.guidanceCustomization.buttonBorderColor = "transparent";
        currentCustomization.guidanceCustomization.buttonBorderWidth = "0px";
        currentCustomization.guidanceCustomization.buttonCornerRadius = "12px";
        currentCustomization.guidanceCustomization.readyScreenOvalFillColor = "transparent";
        currentCustomization.guidanceCustomization.readyScreenTextBackgroundColor = backgroundColor;
        currentCustomization.guidanceCustomization.readyScreenTextBackgroundCornerRadius = "5px";
        currentCustomization.guidanceCustomization.retryScreenImageBorderColor = primaryColor;
        currentCustomization.guidanceCustomization.retryScreenImageBorderWidth = "2px";
        currentCustomization.guidanceCustomization.retryScreenImageCornerRadius = "10px";
        currentCustomization.guidanceCustomization.retryScreenOvalStrokeColor = backgroundColor;
        currentCustomization.guidanceCustomization.retryScreenSlideshowImages = retryScreenSlideshowImages;
        currentCustomization.guidanceCustomization.retryScreenSlideshowInterval = "2000ms";
        currentCustomization.guidanceCustomization.enableRetryScreenSlideshowShuffle = true;
        currentCustomization.guidanceCustomization.cameraPermissionsScreenImage = themeResourceDirectory + "pseudo-fullscreen/camera_shutter_black.png";
        currentCustomization.guidanceCustomization.cameraFeedIssueScreenImage = themeResourceDirectory + "pseudo-fullscreen/camera_shutter_black.png";
        // ID Scan Customization
        currentCustomization.idScanCustomization.showSelectionScreenDocumentImage = true;
        currentCustomization.idScanCustomization.selectionScreenDocumentImage = themeResourceDirectory + "pseudo-fullscreen/document_offblack.png";
        currentCustomization.idScanCustomization.showSelectionScreenBrandingImage = false;
        currentCustomization.idScanCustomization.selectionScreenBrandingImage = "";
        currentCustomization.idScanCustomization.selectionScreenBackgroundColors = backgroundColor;
        currentCustomization.idScanCustomization.reviewScreenBackgroundColors = backgroundColor;
        currentCustomization.idScanCustomization.captureScreenForegroundColor = primaryColor;
        currentCustomization.idScanCustomization.reviewScreenForegroundColor = primaryColor;
        currentCustomization.idScanCustomization.selectionScreenForegroundColor = primaryColor;
        currentCustomization.idScanCustomization.headerFont = font;
        currentCustomization.idScanCustomization.subtextFont = font;
        currentCustomization.idScanCustomization.buttonFont = font;
        currentCustomization.idScanCustomization.buttonTextNormalColor = backgroundColor;
        currentCustomization.idScanCustomization.buttonBackgroundNormalColor = primaryColor;
        currentCustomization.idScanCustomization.buttonTextHighlightColor = backgroundColor;
        currentCustomization.idScanCustomization.buttonBackgroundHighlightColor = buttonHoverColor;
        currentCustomization.idScanCustomization.buttonTextDisabledColor = backgroundColor;
        currentCustomization.idScanCustomization.buttonBackgroundDisabledColor = primaryColor;
        currentCustomization.idScanCustomization.buttonBorderColor = "transparent";
        currentCustomization.idScanCustomization.buttonBorderWidth = "0px";
        currentCustomization.idScanCustomization.buttonCornerRadius = "20px";
        currentCustomization.idScanCustomization.captureScreenTextBackgroundColor = backgroundColor;
        currentCustomization.idScanCustomization.captureScreenTextBackgroundBorderColor = primaryColor;
        currentCustomization.idScanCustomization.captureScreenTextBackgroundBorderWidth = "2px";
        currentCustomization.idScanCustomization.captureScreenTextBackgroundCornerRadius = "5px";
        currentCustomization.idScanCustomization.reviewScreenTextBackgroundColor = backgroundColor;
        currentCustomization.idScanCustomization.reviewScreenTextBackgroundBorderColor = primaryColor;
        currentCustomization.idScanCustomization.reviewScreenTextBackgroundBorderWidth = "2px";
        currentCustomization.idScanCustomization.reviewScreenTextBackgroundBorderCornerRadius = "5px";
        currentCustomization.idScanCustomization.captureScreenBackgroundColor = backgroundColor;
        currentCustomization.idScanCustomization.captureFrameStrokeColor = primaryColor;
        currentCustomization.idScanCustomization.captureFrameStrokeWidth = "2px";
        currentCustomization.idScanCustomization.captureFrameCornerRadius = "12px";
        currentCustomization.idScanCustomization.additionalReviewScreenBackgroundColors = backgroundColor;
        currentCustomization.idScanCustomization.additionalReviewScreenForegroundColor = primaryColor;
        currentCustomization.idScanCustomization.additionalReviewScreenImage = themeResourceDirectory + "pseudo-fullscreen/review_offblack.png";
        currentCustomization.idScanCustomization.additionalReviewScreenAnimation = null;
        // OCR Confirmation Screen Customization
        currentCustomization.ocrConfirmationCustomization.backgroundColors = backgroundColor;
        currentCustomization.ocrConfirmationCustomization.mainHeaderDividerLineColor = secondaryColor;
        currentCustomization.ocrConfirmationCustomization.mainHeaderDividerLineWidth = "2px";
        currentCustomization.ocrConfirmationCustomization.mainHeaderFont = font;
        currentCustomization.ocrConfirmationCustomization.sectionHeaderFont = font;
        currentCustomization.ocrConfirmationCustomization.fieldLabelFont = font;
        currentCustomization.ocrConfirmationCustomization.fieldValueFont = font;
        currentCustomization.ocrConfirmationCustomization.inputFieldFont = font;
        currentCustomization.ocrConfirmationCustomization.inputFieldPlaceholderFont = font;
        currentCustomization.ocrConfirmationCustomization.mainHeaderTextColor = secondaryColor;
        currentCustomization.ocrConfirmationCustomization.sectionHeaderTextColor = primaryColor;
        currentCustomization.ocrConfirmationCustomization.fieldLabelTextColor = primaryColor;
        currentCustomization.ocrConfirmationCustomization.fieldValueTextColor = primaryColor;
        currentCustomization.ocrConfirmationCustomization.inputFieldTextColor = primaryColor;
        currentCustomization.ocrConfirmationCustomization.inputFieldPlaceholderTextColor = "rgba(59, 195, 113, 0.4)";
        currentCustomization.ocrConfirmationCustomization.inputFieldBackgroundColor = "transparent";
        currentCustomization.ocrConfirmationCustomization.inputFieldBorderColor = secondaryColor;
        currentCustomization.ocrConfirmationCustomization.inputFieldBorderWidth = "2px";
        currentCustomization.ocrConfirmationCustomization.inputFieldCornerRadius = "0px";
        currentCustomization.ocrConfirmationCustomization.showInputFieldBottomBorderOnly = true;
        currentCustomization.ocrConfirmationCustomization.buttonFont = font;
        currentCustomization.ocrConfirmationCustomization.buttonTextNormalColor = backgroundColor;
        currentCustomization.ocrConfirmationCustomization.buttonBackgroundNormalColor = primaryColor;
        currentCustomization.ocrConfirmationCustomization.buttonTextHighlightColor = backgroundColor;
        currentCustomization.ocrConfirmationCustomization.buttonBackgroundHighlightColor = "rgb(86, 86, 86)";
        currentCustomization.ocrConfirmationCustomization.buttonTextDisabledColor = backgroundColor;
        currentCustomization.ocrConfirmationCustomization.buttonBackgroundDisabledColor = primaryColor;
        currentCustomization.ocrConfirmationCustomization.buttonBorderColor = "transparent";
        currentCustomization.ocrConfirmationCustomization.buttonBorderWidth = "0px";
        currentCustomization.ocrConfirmationCustomization.buttonCornerRadius = "20px";
        currentCustomization.ocrConfirmationCustomization.customScrollIndicatorAnimation = null;
        currentCustomization.ocrConfirmationCustomization.scrollIndicatorBackgroundNormalColor = primaryColor;
        currentCustomization.ocrConfirmationCustomization.scrollIndicatorBackgroundHighlightColor = primaryColorLight;
        currentCustomization.ocrConfirmationCustomization.scrollIndicatorForegroundNormalColor = backgroundColor;
        currentCustomization.ocrConfirmationCustomization.scrollIndicatorForegroundHighlightColor = backgroundColor;
        currentCustomization.ocrConfirmationCustomization.scrollIndicatorBorderColor = "transparent";
        currentCustomization.ocrConfirmationCustomization.scrollIndicatorBorderWidth = "0px";
        currentCustomization.ocrConfirmationCustomization.scrollIndicatorCornerRadius = "-1";
        currentCustomization.ocrConfirmationCustomization.scrollIndicatorFont = font;
        currentCustomization.ocrConfirmationCustomization.scrollIndicatorShadow = shadow;
        currentCustomization.ocrConfirmationCustomization.enableScrollIndicator = true;
        currentCustomization.ocrConfirmationCustomization.enableScrollIndicatorTextAnimation = false;
        currentCustomization.ocrConfirmationCustomization.enableFixedConfirmButton = false;
        currentCustomization.ocrConfirmationCustomization.showScrollIndicatorImage = true;
        // Result Screen Customization
        currentCustomization.resultScreenCustomization.backgroundColors = backgroundColor;
        currentCustomization.resultScreenCustomization.foregroundColor = primaryColor;
        currentCustomization.resultScreenCustomization.messageFont = font;
        currentCustomization.resultScreenCustomization.activityIndicatorColor = primaryColor;
        currentCustomization.resultScreenCustomization.customActivityIndicatorImage = themeResourceDirectory + "pseudo-fullscreen/activity_indicator_faded_black.png";
        currentCustomization.resultScreenCustomization.customActivityIndicatorRotationInterval = "0.8s";
        currentCustomization.resultScreenCustomization.customActivityIndicatorAnimation = uploadActivityIndicatorSVG;
        currentCustomization.resultScreenCustomization.resultAnimationBackgroundColor = backgroundColor;
        currentCustomization.resultScreenCustomization.resultAnimationForegroundColor = primaryColor;
        currentCustomization.resultScreenCustomization.resultAnimationSuccessBackgroundImage = "";
        currentCustomization.resultScreenCustomization.resultAnimationUnsuccessBackgroundImage = "";
        currentCustomization.resultScreenCustomization.customResultAnimationSuccess = successResultAnimationSVG;
        currentCustomization.resultScreenCustomization.customResultAnimationUnsuccess = unsuccessResultAnimationSVG;
        currentCustomization.resultScreenCustomization.showUploadProgressBar = true;
        currentCustomization.resultScreenCustomization.uploadProgressTrackColor = primaryColor;
        currentCustomization.resultScreenCustomization.uploadProgressFillColor = secondaryColor;
        currentCustomization.resultScreenCustomization.animationRelativeScale = 1.0;
        // Feedback Customization
        currentCustomization.feedbackCustomization.backgroundColor = backgroundColor;
        currentCustomization.feedbackCustomization.textColor = textPrimaryColor;
        currentCustomization.feedbackCustomization.textFont = font;
        currentCustomization.feedbackCustomization.cornerRadius = "5px";
        currentCustomization.feedbackCustomization.shadow = "0px 3px 10px black";
        // Oval Customization
        currentCustomization.ovalCustomization.strokeColor = primaryColor;
        currentCustomization.ovalCustomization.progressColor1 = "rgba(59, 195, 113, 0.7)";
        currentCustomization.ovalCustomization.progressColor2 = "rgba(59, 195, 113, 0.7)";
        // Cancel Button Customization
        currentCustomization.cancelButtonCustomization.customImage = themeResourceDirectory + "pseudo-fullscreen/single_chevron_left_offwhite.png";
        currentCustomization.cancelButtonCustomization.location = FaceTecSDK.FaceTecCancelButtonLocation.Custom;
        currentCustomization.cancelButtonCustomization.setCustomLocation(50, 50, 20, 20);
        currentCustomization.cancelButtonCustomization.hideForCameraPermissions = false;
        // Guidance Customization -- Text Style Overrides
        // Ready Screen Header
        currentCustomization.guidanceCustomization.readyScreenHeaderFont = font;
        currentCustomization.guidanceCustomization.readyScreenHeaderTextColor = textPrimaryColor;
        // Ready Screen Subtext
        currentCustomization.guidanceCustomization.readyScreenSubtextFont = font;
        currentCustomization.guidanceCustomization.readyScreenSubtextTextColor = textPrimaryColor;
        // Retry Screen Header
        currentCustomization.guidanceCustomization.retryScreenHeaderFont = font;
        currentCustomization.guidanceCustomization.retryScreenHeaderTextColor = textPrimaryColor;
        // Retry Screen Subtext
        currentCustomization.guidanceCustomization.retryScreenSubtextFont = font;
        currentCustomization.guidanceCustomization.retryScreenSubtextTextColor = textPrimaryColor;
        // Security Watermark Customization
        currentCustomization.securityWatermarkCustomization.setSecurityWatermarkImage(FaceTecSDK.FaceTecSecurityWatermarkImage.FaceTec);

        return currentCustomization;
    }
    function getLowLightCustomizationForTheme(theme) {
        var currentLowLightCustomization = getCustomizationForTheme(theme);
        var retryScreenSlideshowImages = [themeResourceDirectory + "FaceTec_ideal_1.png", themeResourceDirectory + "FaceTec_ideal_2.png", themeResourceDirectory + "FaceTec_ideal_3.png", themeResourceDirectory + "FaceTec_ideal_4.png", themeResourceDirectory + "FaceTec_ideal_5.png"];

        return currentLowLightCustomization;
    }
    function getDynamicDimmingCustomizationForTheme(theme) {
        var currentDynamicDimmingCustomization = getCustomizationForTheme(theme);
        var retryScreenSlideshowImages = [themeResourceDirectory + "FaceTec_ideal_1.png", themeResourceDirectory + "FaceTec_ideal_2.png", themeResourceDirectory + "FaceTec_ideal_3.png", themeResourceDirectory + "FaceTec_ideal_4.png", themeResourceDirectory + "FaceTec_ideal_5.png"];

            var primaryColor = "rgba(238, 246, 248, 0)"; // off-white
            var primaryColorLight = "rgba(238, 246, 248, 0)"; // white
            var secondaryColor = "rgba(238, 246, 248, 0)"; // green
            var backgroundColor = "black";
            var activityIndicatorSVG = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            activityIndicatorSVG.setAttribute("viewBox", "0 0 52 52");
            activityIndicatorSVG.classList.add("pseudo-fullscreen-activity-indicator-svg__offwhite");
            activityIndicatorSVG.innerHTML = "<circle class='path' cx='26' cy='26' r='22'></circle>";
            var successResultAnimationSVG = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            successResultAnimationSVG.setAttribute("viewBox", "0 0 52 52");
            successResultAnimationSVG.classList.add("pseudo-fullscreen-success-svg__offwhite");
            successResultAnimationSVG.innerHTML = "<circle class='circlePath' cx='26' cy='26' r='22'></circle><path class='checkmarkPath' d='M14.1 27.2l7.1 7.2 16.7-16.8'></path>";
            var unsuccessResultAnimationSVG = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            unsuccessResultAnimationSVG.setAttribute("viewBox", "0 0 52 52");
            unsuccessResultAnimationSVG.classList.add("pseudo-fullscreen-unsuccess-svg__offwhite");
            unsuccessResultAnimationSVG.innerHTML = "<circle class='circlePath' cx='26' cy='26' r='22'></circle><line class='crossPath1' x1='18' y1='18' x2='34' y2='34'></line><line class='crossPath2' x1='34' y1='18' x2='18' y2='34'></line>";
            // Overlay Customization
            currentDynamicDimmingCustomization.overlayCustomization.brandingImage = "";
            // Guidance Customization
            currentDynamicDimmingCustomization.guidanceCustomization.foregroundColor = primaryColor;
            currentDynamicDimmingCustomization.guidanceCustomization.buttonTextNormalColor = backgroundColor;
            currentDynamicDimmingCustomization.guidanceCustomization.buttonBackgroundNormalColor = primaryColor;
            currentDynamicDimmingCustomization.guidanceCustomization.buttonTextHighlightColor = backgroundColor;
            currentDynamicDimmingCustomization.guidanceCustomization.buttonBackgroundHighlightColor = "white";
            currentDynamicDimmingCustomization.guidanceCustomization.buttonTextDisabledColor = backgroundColor;
            currentDynamicDimmingCustomization.guidanceCustomization.buttonBackgroundDisabledColor = "rgba(238, 246, 248, 0.3)";
            currentDynamicDimmingCustomization.guidanceCustomization.buttonBorderColor = "transparent";
            currentDynamicDimmingCustomization.guidanceCustomization.readyScreenOvalFillColor = "transparent";
            currentDynamicDimmingCustomization.guidanceCustomization.readyScreenTextBackgroundColor = backgroundColor;
            currentDynamicDimmingCustomization.guidanceCustomization.retryScreenImageBorderColor = primaryColor;
            currentDynamicDimmingCustomization.guidanceCustomization.retryScreenOvalStrokeColor = primaryColor;
            currentDynamicDimmingCustomization.guidanceCustomization.retryScreenSlideshowImages = retryScreenSlideshowImages;
            currentDynamicDimmingCustomization.guidanceCustomization.cameraPermissionsScreenImage = themeResourceDirectory + "pseudo-fullscreen/camera_shutter_black.png";
            currentDynamicDimmingCustomization.guidanceCustomization.cameraFeedIssueScreenImage = themeResourceDirectory + "pseudo-fullscreen/camera_shutter_black.png";
            // ID Scan Customization
            currentDynamicDimmingCustomization.idScanCustomization.selectionScreenDocumentImage = themeResourceDirectory + "pseudo-fullscreen/document_offwhite.png";
            currentDynamicDimmingCustomization.idScanCustomization.selectionScreenBrandingImage = "";
            currentDynamicDimmingCustomization.idScanCustomization.captureScreenForegroundColor = primaryColor;
            currentDynamicDimmingCustomization.idScanCustomization.reviewScreenForegroundColor = primaryColor;
            currentDynamicDimmingCustomization.idScanCustomization.selectionScreenForegroundColor = primaryColor;
            currentDynamicDimmingCustomization.idScanCustomization.buttonTextNormalColor = backgroundColor;
            currentDynamicDimmingCustomization.idScanCustomization.buttonBackgroundNormalColor = primaryColor;
            currentDynamicDimmingCustomization.idScanCustomization.buttonTextHighlightColor = backgroundColor;
            currentDynamicDimmingCustomization.idScanCustomization.buttonBackgroundHighlightColor = "white";
            currentDynamicDimmingCustomization.idScanCustomization.buttonTextDisabledColor = backgroundColor;
            currentDynamicDimmingCustomization.idScanCustomization.buttonBackgroundDisabledColor = "rgba(238, 246, 248, 0.3)";
            currentDynamicDimmingCustomization.idScanCustomization.buttonBorderColor = "transparent";
            currentDynamicDimmingCustomization.idScanCustomization.captureScreenTextBackgroundColor = backgroundColor;
            currentDynamicDimmingCustomization.idScanCustomization.captureScreenTextBackgroundBorderColor = primaryColor;
            currentDynamicDimmingCustomization.idScanCustomization.reviewScreenTextBackgroundColor = backgroundColor;
            currentDynamicDimmingCustomization.idScanCustomization.reviewScreenTextBackgroundBorderColor = primaryColor;
            currentDynamicDimmingCustomization.idScanCustomization.captureFrameStrokeColor = primaryColor;
            currentDynamicDimmingCustomization.idScanCustomization.additionalReviewScreenForegroundColor = primaryColor;
            currentDynamicDimmingCustomization.idScanCustomization.additionalReviewScreenImage = themeResourceDirectory + "pseudo-fullscreen/review_offwhite.png";
            currentDynamicDimmingCustomization.idScanCustomization.additionalReviewScreenAnimation = null;
            // OCR Confirmation Screen Customization
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.mainHeaderDividerLineColor = secondaryColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.mainHeaderTextColor = secondaryColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.sectionHeaderTextColor = primaryColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.fieldLabelTextColor = primaryColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.fieldValueTextColor = primaryColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.inputFieldTextColor = primaryColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.inputFieldPlaceholderTextColor = "rgba(59, 195, 113, 0.4)";
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.inputFieldBackgroundColor = "transparent";
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.inputFieldBorderColor = secondaryColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.buttonTextNormalColor = backgroundColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.buttonBackgroundNormalColor = primaryColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.buttonTextHighlightColor = backgroundColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.buttonBackgroundHighlightColor = "white";
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.buttonTextDisabledColor = backgroundColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.buttonBackgroundDisabledColor = "rgba(238, 246, 248, 0.3)";
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.buttonBorderColor = "transparent";
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.customScrollIndicatorAnimation = null;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.scrollIndicatorBackgroundNormalColor = primaryColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.scrollIndicatorBackgroundHighlightColor = primaryColorLight;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.scrollIndicatorForegroundNormalColor = backgroundColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.scrollIndicatorForegroundHighlightColor = backgroundColor;
            currentDynamicDimmingCustomization.ocrConfirmationCustomization.scrollIndicatorBorderColor = "transparent";
            // Result Screen Customization
            currentDynamicDimmingCustomization.resultScreenCustomization.foregroundColor = primaryColor;
            currentDynamicDimmingCustomization.resultScreenCustomization.activityIndicatorColor = primaryColor;
            currentDynamicDimmingCustomization.resultScreenCustomization.customActivityIndicatorImage = themeResourceDirectory + "pseudo-fullscreen/activity_indicator_faded_black.png";
            currentDynamicDimmingCustomization.resultScreenCustomization.customActivityIndicatorAnimation = activityIndicatorSVG;
            currentDynamicDimmingCustomization.resultScreenCustomization.resultAnimationBackgroundColor = secondaryColor;
            currentDynamicDimmingCustomization.resultScreenCustomization.resultAnimationForegroundColor = backgroundColor;
            currentDynamicDimmingCustomization.resultScreenCustomization.resultAnimationSuccessBackgroundImage = "";
            currentDynamicDimmingCustomization.resultScreenCustomization.resultAnimationUnsuccessBackgroundImage = "";
            currentDynamicDimmingCustomization.resultScreenCustomization.customResultAnimationSuccess = successResultAnimationSVG;
            currentDynamicDimmingCustomization.resultScreenCustomization.customResultAnimationUnsuccess = unsuccessResultAnimationSVG;
            currentDynamicDimmingCustomization.resultScreenCustomization.showUploadProgressBar = true;
            currentDynamicDimmingCustomization.resultScreenCustomization.uploadProgressTrackColor = "rgba(238, 246, 248, 0.2)";
            currentDynamicDimmingCustomization.resultScreenCustomization.uploadProgressFillColor = secondaryColor;
            currentDynamicDimmingCustomization.resultScreenCustomization.animationRelativeScale = 1.0;
            // Feedback Customization
            currentDynamicDimmingCustomization.feedbackCustomization.backgroundColor = secondaryColor;
            currentDynamicDimmingCustomization.feedbackCustomization.textColor = backgroundColor;
            currentDynamicDimmingCustomization.feedbackCustomization.shadow = "0px 3px 10px black";
            // Frame Customization
            currentDynamicDimmingCustomization.frameCustomization.borderColor = primaryColor;
            currentDynamicDimmingCustomization.frameCustomization.shadow = "none";
            // Oval Customization
            currentDynamicDimmingCustomization.ovalCustomization.strokeColor = primaryColor;
            currentDynamicDimmingCustomization.ovalCustomization.progressColor1 = "rgba(59, 195, 113, 0.7)";
            currentDynamicDimmingCustomization.ovalCustomization.progressColor2 = "rgba(59, 195, 113, 0.7)";
            // Cancel Button Customization
            currentDynamicDimmingCustomization.cancelButtonCustomization.customImage = themeResourceDirectory + "pseudo-fullscreen/single_chevron_left_offwhite.png";
            // Guidance Customization -- Text Style Overrides
            // Ready Screen Header
            currentDynamicDimmingCustomization.guidanceCustomization.readyScreenHeaderTextColor = primaryColor;
            // Ready Screen Subtext
            currentDynamicDimmingCustomization.guidanceCustomization.readyScreenSubtextTextColor = primaryColor;
            // Retry Screen Header
            currentDynamicDimmingCustomization.guidanceCustomization.retryScreenHeaderTextColor = primaryColor;
            // Retry Screen Subtext
            currentDynamicDimmingCustomization.guidanceCustomization.retryScreenSubtextTextColor = primaryColor;

        return currentDynamicDimmingCustomization;
    }
    function showNewTheme() {
        var themes = [""];
        if (Config.wasSDKConfiguredWithConfigWizard === true) {
            themes = ["Config Wizard Theme", "FaceTec Theme", "Pseudo-Fullscreen", "Well-Rounded", "Bitcoin Exchange", "eKYC", "Sample Bank"];
        }
        else {
            themes = ["FaceTec Theme", "Pseudo-Fullscreen", "Well-Rounded", "Bitcoin Exchange", "eKYC", "Sample Bank"];
        }
        var currentThemeIndex = themes.indexOf(currentTheme);
        currentThemeIndex = currentThemeIndex >= themes.length - 1 ? 0 : currentThemeIndex + 1;
        currentTheme = themes[currentThemeIndex];
        setAppTheme(currentTheme);
        updateThemeTransitionView();
        DeveloperStatusMessages.logAndDisplayMessage("Theme set to: " + currentTheme);
    }
    function updateThemeTransitionView() {
        var transitionViewImage = "";
        var transitionViewTextColor = Config.currentCustomization.guidanceCustomization.foregroundColor;
        var transitionViewClass = "theme-transition-overlay__";
        var deviceType = "desktop";
        if (SampleAppUtilities.isLikelyMobileDevice() === true) {
            deviceType = "mobile";
        }
        switch (currentTheme) {
            case "FaceTec Theme":
                transitionViewClass = "default";
                break;
            case "Pseudo-Fullscreen":
                transitionViewClass += "default";
                break;
            case "Well-Rounded":
                transitionViewImage = themeResourceDirectory + "well-rounded/well_rounded_" + deviceType + "_bg.svg";
                transitionViewClass += "well-rounded";
                transitionViewTextColor = Config.currentCustomization.frameCustomization.backgroundColor;
                break;
            case "Bitcoin Exchange":
                transitionViewImage = themeResourceDirectory + "bitcoin-exchange/bitcoin_exchange_" + deviceType + "_bg.svg";
                transitionViewClass += "bitcoin-exchange";
                transitionViewTextColor = Config.currentCustomization.frameCustomization.backgroundColor;
                break;
            case "eKYC":
                transitionViewImage = themeResourceDirectory + "ekyc/ekyc_" + deviceType + "_bg.svg";
                transitionViewClass += "ekyc";
                break;
            case "Sample Bank":
                transitionViewImage = themeResourceDirectory + "sample-bank/sample_bank_" + deviceType + "_bg.svg";
                transitionViewClass += "sample-bank";
                transitionViewTextColor = Config.currentCustomization.frameCustomization.backgroundColor;
                break;
            default:
                transitionViewClass = "default";
                break;
        }
        transitionViewClass += "__" + deviceType;
        document.getElementById("theme-transition-overlay-img").src = transitionViewImage;
        document.getElementById("theme-transition-overlay").className = transitionViewClass;
        document.getElementById("loading-session-token-text").style.color = transitionViewTextColor;
    }
    function getCurrentTheme() {
        currentTheme = Config.wasSDKConfiguredWithConfigWizard ? "Config Wizard Theme" : "FaceTec Theme";
        return currentTheme;
    }
    return {
        getCurrentTheme: getCurrentTheme,
        themeResourceDirectory: themeResourceDirectory,
        setAppTheme: function (theme) {
            setAppTheme(theme);
        },
        showNewTheme: showNewTheme
    };
})();
