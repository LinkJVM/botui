PROJECT(botui)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

FIND_PACKAGE(Qt4 REQUIRED)

SET(INCLUDE ${botui_SOURCE_DIR}/include/botui)
SET(RC ${botui_SOURCE_DIR}/rc)
SET(SRC ${botui_SOURCE_DIR}/src)
SET(UI ${botui_SOURCE_DIR}/ui)

SET(DEVICES ${botui_SOURCE_DIR}/devices)
SET(KOVAN ${DEVICES}/kovan)
SET(SIMULATOR ${DEVICES}/simulator)

SET(DBUS ${CMAKE_SOURCE_DIR}/dbus)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
	${DBUS}/include
	${botui_SOURCE_DIR} 
	${botui_BINARY_DIR}
	${KOVAN}/include
	${SIMULATOR}/include
	${INCLUDE}
)

SET(QT_USE_QTDECLARATIVE TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTDBUS TRUE)

INCLUDE(${QT_USE_FILE})

FILE(GLOB INCLUDES ${INCLUDE}/*.h ${KOVAN}/include/*.h ${SIMULATOR}/include/*.h ${DBUS}/include/*.h)
FILE(GLOB SOURCES ${SRC}/*.cpp ${KOVAN}/src/*.cpp ${SIMULATOR}/src/*.cpp ${DBUS}/src/*.cpp)
FILE(GLOB UIS ${UI}/*)

SET(botui_SRCS_CXX ${SOURCES})
SET(botui_UI ${UIS})
SET(botui_MOC_SRCS ${INCLUDES})

SET(botui_QRC_FILES
	${RC}/icons.qrc
	${RC}/construction.qrc
	${RC}/qml.qrc
	${RC}/fonts.qrc
)

QT4_WRAP_CPP(botui_SRCS_CXX ${botui_MOC_SRCS})
QT4_WRAP_UI(botui_SRCS_CXX ${botui_UI})
QT4_ADD_RESOURCES(botui_SRCS_CXX ${botui_QRC_FILES})

SET(CMAKE_CXX_FLAGS "-Wall")

IF(APPLE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
	LINK_DIRECTORIES(/Library/Frameworks/)
ELSEIF(WIN32)
	SET(CMAKE_CXX_FLAGS "-Wl,--enable-auto-import")
ENDIF()

IF(WIN32)
	ADD_EXECUTABLE(botui WIN32 ${botui_SRCS_CXX})
ELSE()
	ADD_EXECUTABLE(botui ${botui_SRCS_CXX})
ENDIF()

add_library(libbotui SHARED ${botui_SRCS_CXX})
set_target_properties(libbotui PROPERTIES OUTPUT_NAME "botui")

add_definitions(-O3)

SET(EXECUTABLE_OUTPUT_PATH ${botui_SOURCE_DIR}/deploy)
SET(LIBRARY_OUTPUT_PATH ${botui_SOURCE_DIR}/lib)
TARGET_LINK_LIBRARIES(botui ${QT_LIBRARIES} pcompiler kar z opencv_core opencv_highgui opencv_imgproc kovan)
TARGET_LINK_LIBRARIES(libbotui ${QT_LIBRARIES} pcompiler kar z opencv_core opencv_highgui opencv_imgproc kovan)

IF(WIN32)
install(FILES ${INCLUDES} DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/include/botui)
INSTALL(TARGETS libbotui
	ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib
	RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib)
ELSE(WIN32)
install(FILES ${INCLUDES} DESTINATION include/botui)
install(TARGETS libbotui DESTINATION lib)
ENDIF(WIN32)
