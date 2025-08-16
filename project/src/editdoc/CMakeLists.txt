project(editdoc)

set(UI_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/aiconfigdialog.ui
)

set(RCC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/image.qrc
)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/kpagecontainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ktextdoc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ktab.h
    ${CMAKE_CURRENT_SOURCE_DIR}/textstyle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/aiconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/aimanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kaieditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/aiconfigdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kheaderedit.h
)

set(CPP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/kpagecontainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ktextdoc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ktab.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/aimanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kaieditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/aiconfigdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kheaderedit.cpp
)

set(TS_FILES ${CMAKE_SOURCE_DIR}/mui/zh_CN/editor_translate_zh.ts)
set(QM_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/mui/zh_CN/)
qt5_create_translation(QM_FILES ${HEADER_FILES} ${CPP_FILES} ${TS_FILES})

add_library(editdoc STATIC ${HEADER_FILES} ${CPP_FILES} ${RCC_FILES} ${UI_FILES})

target_link_libraries(editdoc Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)

add_custom_target(translations_${PROJECT_NAME} DEPENDS ${QM_FILES})
add_dependencies(${PROJECT_NAME} translations_${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${QM_OUTPUT_DIR}
    COMMENT "Copying .qm files to ${QM_OUTPUT_DIR}"
)

# 导出头文件目录
target_include_directories(editdoc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})