set(UI_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
)

set(RCC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/image.qrc
)

set(HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
)
set(CPP_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
)

set(TS_FILES ${CMAKE_SOURCE_DIR}/mui/zh_CN/main_translate_zh.ts)
set(QM_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/mui/zh_CN/)
qt5_create_translation(QM_FILES ${HEADER_FILES} ${CPP_FILES} ${TS_FILES})

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})

add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${CPP_FILES} ${UI_FILES} ${RCC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../topmenu
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(${PROJECT_NAME} 
	Qt5::Widgets
	Qt5::Core
	Qt5::Gui
	editdoc
	topmenulib
)

add_custom_target(translations_${PROJECT_NAME} DEPENDS ${QM_FILES})
add_dependencies(${PROJECT_NAME} translations_${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${QM_OUTPUT_DIR}
    COMMENT "Copying .qm files to ${QM_OUTPUT_DIR}"
)