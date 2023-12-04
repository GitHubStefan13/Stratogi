QT += qml quick quickcontrols2 concurrent

TARGET = stratogi
TEMPLATE = app

CONFIG += c++11 static
CONFIG += no_keywords # No Qt Keywords like Signals, emit ...
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Set up main/install/setup directories for specfic Systems
ANDROID_DIR = $${PWD}/install/android

equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 13){
  # Qt messed up the Android build, so you need to maintain two Versions
  # of the Manifest to make it work for Qt Version >= 5.14
  android {
    ANDROID_MANIFEST_XML_INPUT = $${ANDROID_DIR}/AndroidManifest_new.xml.in
    ANDROID_GRADLE_BUILD_IN = $${ANDROID_DIR}/gradle.build.in
    ANDROID_RESOURCE_LIB_XML_IN = $${ANDROID_DIR}/libs.xml.in
  }

} else {
  DESTDIR=bin #Puts all the Binary files into this Path
  OBJECTS_DIR=generated_files
  MOC_DIR=generated_files/moc

  android {
    ANDROID_MANIFEST_XML_INPUT = $${ANDROID_DIR}/AndroidManifest_old.xml.in
    ANDROID_GRADLE_BUILD_IN = $${ANDROID_DIR}/gradle.build.in
  }
}

PARENT_DIR           = $$PWD
PROJECT_LIBS         = $$PARENT_DIR/libs
PROJECT_FONTS        = $$PARENT_DIR/fonts
THIRD_PARTY_LIBS     = $$PARENT_DIR/thirdparty/libs
THIRD_PARTY_INCLUDES = $$PARENT_DIR/thirdparty/includes

INCLUDEPATH += includes
INCLUDEPATH += includes/Abstract
INCLUDEPATH += includes/Chess
INCLUDEPATH += includes/Shogi

INCLUDEPATH += $$THIRD_PARTY_INCLUDES
				
SOURCES = $$files(src/*.cpp) \
          $$files(src/Abstract/*.cpp) \
          $$files(src/Chess/*.cpp) \
          $$files(src/Shogi/*.cpp) \
			
HEADERS = $$files(includes/*.h) \
          $$files(includes/*.hpp) \
          $$files(includes/Abstract/*.h) \
          $$files(includes/Abstract/*.hpp) \
          $$files(includes/Chess/*.h) \
          $$files(includes/Chess/*.hpp) \
          $$files(includes/Shogi/*.h) \
          $$files(includes/Shogi/*.hpp) \

win32|unix|macox{
  RESOURCES += res.qrc
}

android|ios {
  RESOURCES += res_smartphone.qrc
}

RESOURCES_ENABLED  = false
contains(RESOURCES_ENABLED, true){
    # Resources are enabled, so we will look for them in the
    # Stantard Folder, if they are not there, we will throw an ERROR
    RESOURCE_ICONS     = $$PWD/icons
    RESOURCE_ACTIONBAR = $$RESOURCE_ICONS/actionbar
    RESOURCE_MATERIAL  = $$RESOURCE_ICONS/material

    !exists($$RESOURCE_ICONS){
        error("Could not find Resource Icons, check the Path or disable the Resources")
    } else {
        message(Using Standard Resource Icons from $$RESOURCE_ICONS)
    }

    RESOURCES += $$RESOURCE_ACTIONBAR/index.theme

    RESOURCES += $$files($$RESOURCE_ICONS/*.png) \
                 $$files($$RESOURCE_ACTIONBAR/20x20/*.png) \
                 $$files($$RESOURCE_ACTIONBAR/20x20@2/*.png) \
                 $$files($$RESOURCE_ACTIONBAR/20x20@3/*.png) \
                 $$files($$RESOURCE_ACTIONBAR/20x20@4/*.png) \
                 $$files($$RESOURCE_MATERIAL/action/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/alert/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/av/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/communication/*.svg) \
                 $$files($$RESOURCE_MATERIAL/content/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/device/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/editor/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/file/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/hardware/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/image/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/maps/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/navigation/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/notification/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/places/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/social/svg/*.svg) \
                 $$files($$RESOURCE_MATERIAL/toogle/svg/*.svg)
}

unix{

}

android {
  QT += androidextras

  ANDROID_PACKAGE_SOURCE_DIR = $${ANDROID_DIR} # Qt needs this to be set

  # Android arm64-v8a is 64-Bit
  # Android x86_64 is for Emulation or Windows Phones - i686 cpu architecture
  QMAKE_TARGET = "Android"
  QMAKE_TARGET.os   = "Android"
  QMAKE_TARGET.arch = $$replace(ANDROID_TARGET_ARCH, -, _)
  QMAKE_TARGET.version_string = $$ANDROID_PLATFORM

  # Minimale Android-SDK-Version 5.0.0 SDK:21
  MIN_OS_VERSION_MAJOR = 5
  MIN_OS_VERSION_MINOR = 0
  MIN_OS_VERSION_PATCH = 0

  NDK_PLATFORM = $$(ANDROID_NDK_PLATFORM)
  NDK_PLATFORM = $$replace(NDK_PLATFORM, "android-", "")

  # Android Manifest Settings
  LAUNCHER_ICON        = ic_launcher
  ANDROID_MINSDK       = 21
  ANDROID_TARGETSDK    = 29
  GRADE_VERSION        = 3.2.0

  CONFIG(release, debug|release) {
    ANDROID_PACKAGENAME = "com.dsgaming.Stratogi"
  } else {
    ANDROID_PACKAGENAME = "com.dsgaming.Stratogi" #TODO: With Debug at the end but Qt does not like that yet...
  }
  #ANDROID_AUTHORITIES = $${ANDROID_PACKAGENAME}.sharex.fileprovider

  # If we need to adjust things for a given Architecture
  contains(ANDROID_TARGET_ARCH,arm64-v8a){
  }
  contains(ANDROID_TARGET_ARCH,x86_64){
  }
  contains(ANDROID_TARGET_ARCH,armeabi-v7a){
  }
  contains(ANDROID_TARGET_ARCH,x86){
  }

  # Fixed in new Qt-Versions https://bugreports.qt.io/browse/QTBUG-76293
  equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 14){
    QMAKE_LINK += -nostdlib++
    QMAKE_CFLAGS += -nostdinc++
  }

  DISTFILES += \
    $${ANDROID_DIR}/gradle/wrapper/gradle-wrapper.jar        \
    $${ANDROID_DIR}/gradlew                                  \
    $${ANDROID_DIR}/gradle/wrapper/gradle-wrapper.properties \
    $${ANDROID_DIR}/gradlew.bat                              \
    $${ANDROID_MANIFEST_XML_INPUT}                           \
    $${ANDROID_DIR}/build.gradle

  # Um ggf. Anpassungen im Android Manifest vorzunehmen
  Manifest.input  = $${ANDROID_MANIFEST_XML_INPUT}
  Manifest.output = $${ANDROID_DIR}/AndroidManifest.xml
  QMAKE_SUBSTITUTES += Manifest

  # Um ggf. Anpassungen im Gradle vorzunehmen
  Gradle.input  = $${ANDROID_GRADLE_BUILD_IN}
  Gradle.output = $${ANDROID_DIR}/build.gradle
  QMAKE_SUBSTITUTES += Gradle

  # Um ggf. Anpassungen im Gradle vorzunehmen
  LibsXML.input  = $${ANDROID_RESOURCE_LIB_XML_IN}
  LibsXML.output = $${ANDROID_DIR}/res/values/libs.xml
  QMAKE_SUBSTITUTES += LibsXML



  OTHER_FILES += $${ANDROID_DIR}/build.gradle.in        \
                 $${ANDROID_MANIFEST_XML_INPUT}
  message("Using Package Source: $$ANDROID_PACKAGE_SOURCE_DIR")

  message("Android NDK-Platform: $$NDK_PLATFORM")

  OS_VERSION_STRING = $$MIN_OS_VERSION_MAJOR'.'$$MIN_OS_VERSION_MINOR'.'$$MIN_OS_VERSION_PATCH
  message("Android minSdkVersion: $$OS_VERSION_STRING'($$QMAKE_TARGET.version_string)'")
}

win32 {
  RC_FILE = stratogi.rc
}

message("Using external Libraries: $${LIBS}")
