cmake_minimum_required(VERSION 3.16)

project(PokerTeacher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(PROJECT_SOURCES
        main.cpp
        view.cpp
        view.h
        view.ui
        infobox.h
        infobox.cpp
        infobox.ui



)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(PokerCore STATIC
    card.h card.cpp
    suit.h
    game.h game.cpp
    handtype.h
    cardhand.h cardhand.cpp
    cardlibrary.h cardlibrary.cpp
    cardvalue.h
    phase.h
    player.h
    lesson.h lesson.cpp

    action.h

    player.cpp

)

target_include_directories(PokerCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(PokerCore PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PokerCore PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(PokerCore PRIVATE Qt6::Core)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PokerTeacher
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/card_images.qrc


        # <-- THIS MUST BE INSIDE EXE
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PokerTeacher APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PokerTeacher SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PokerTeacher
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(PokerTeacher PRIVATE PokerCore)
target_link_libraries(PokerTeacher PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PokerTeacher PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PokerTeacher)
endif()
set_target_properties(PokerTeacher PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PokerTeacher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PokerTeacher)
endif()
add_subdirectory(PokerTests)
