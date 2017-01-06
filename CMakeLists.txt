cmake_minimum_required(VERSION 2.8.11)


project( App )


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

find_package(Qt5 COMPONENTS Quick Core Network)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( Train train.cpp )

target_link_libraries( Train ${OpenCV_LIBS} )


# Use the Widgets module from Qt 5.
target_link_libraries(Train Qt5::Widgets)

qt5_add_resources(RESOURCES qml.qrc)

add_executable( App main.cpp ImageLoader.cpp ImageConverter.cpp ImageProvider.cpp ${RESOURCES})

qt5_use_modules(App Quick Core Network)

target_link_libraries( App ${OpenCV_LIBS} )


# Use the Widgets module from Qt 5.
target_link_libraries(App Qt5::Widgets)