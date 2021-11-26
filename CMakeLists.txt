cmake_minimum_required(VERSION 3.20)

#Project name
set(TARGET_NAME chatDatabaseProject)
project(${TARGET_NAME})

#Set the CXX standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Make sure CMake will take care of moc for us
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# .ui directory
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)

#We use Qt 6
set(QT_MAJOR_VERSION 6)

#Qt6 requires setting the CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH $ENV{Qt${QT_MAJOR_VERSION}_HOME})

#find_package should find everything fine so long as the ENV Variable is set or, for linux systems,
#it is in the default install path.
find_package(Qt${QT_MAJOR_VERSION} COMPONENTS Widgets REQUIRED PATHS $ENV{Qt${QT_MAJOR_VERSION}_HOME})

#Add every file into this list, otherwise moc don't find the classes
add_executable(${TARGET_NAME}
        source/main.cpp
        source/MainWindow.cpp
        include/MainWindow.h)

#Copy required libraries into work directory
if (WIN32)
    set(QT_INSTALL_PATH "$ENV{Qt${QT_MAJOR_VERSION}_HOME}")
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

target_include_directories(${TARGET_NAME} PRIVATE ${Qt${QT_MAJOR_VERSION}_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

#Add any extra libs to link also.
target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_MAJOR_VERSION}::Widgets)