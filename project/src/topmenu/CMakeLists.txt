cmake_minimum_required(VERSION 3.10)  #cmake版本最低3.10

project(topmenudll)  #添加当前项目名称

#默认生成动态库
add_library(${PROJECT_NAME} SHARED)

set(RCC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/image.qrc
)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/kclipboardtoolbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kcolorpicker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kfonttoolbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kparagraphtoolbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kstartribbon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ktopmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/clipboardsidebar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kstylebutton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stylemanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kstylestoolbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kstylesfloatingwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kinsertribbon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ktablepickerpopup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kairibbon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kpageribbon.h
)

set(CPP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/kclipboardtoolbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kcolorpicker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kfonttoolbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kparagraphtoolbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kstartribbon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ktopmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clipboardsidebar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kstylebutton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stylemanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kstylestoolbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kstylesfloatingwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kinsertribbon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ktablepickerpopup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kairibbon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kpageribbon.cpp
)

# 国际化

set(TS_FILES ${CMAKE_SOURCE_DIR}/mui/zh_CN/topmenu_translate_zh.ts)
set(QM_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/mui/zh_CN/)

qt5_create_translation(QM_FILES ${HEADER_FILES} ${CPP_FILES} ${TS_FILES})

target_sources(topmenudll PRIVATE ${CPP_FILES} ${HEADER_FILES} ${RCC_FILES})

#针对特定的目标,定义宏
target_compile_definitions(topmenudll PRIVATE
    KCLIPBOARDTOOLBAR_EXPORTS
    KCOLORPICKER_EXPORTS
    KFONTTOOLBAR_EXPORTS
    KPARAGRAPHTOOLBAR_EXPORTS
    KSTARTRIBBON_EXPORTS
    KTOPMENU_EXPORTS
    CLIPBOARDSIDEBAR_EXPORTS
    KSTYLEBUTTON_EXPORTS
    STYLEMANAGER_EXPORTS
    KSTYLESTOOLBAR_EXPORTS
    KSTYLESFLOATINGWINDOW_EXPORTS
    KINSERTRIBBON_EXPORTS
    KTABLEPICKERPOPUP_EXPORTS
    KAIRIBBON_EXPORTS
    KPAGERIBBON_EXPORTS
)

target_link_libraries(${PROJECT_NAME} 
	Qt5::Widgets
	Qt5::Core
	Qt5::Gui
    editdoc
)

# 设置库的输出目录
set_target_properties(topmenudll PROPERTIES
    OUTPUT_NAME "topmenulib"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  #.lib .exp 输出路径
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  #.dll 输出路径
)

# 国际化

add_custom_target(translations_${PROJECT_NAME} DEPENDS ${QM_FILES})
add_dependencies(${PROJECT_NAME} translations_${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${QM_OUTPUT_DIR}
    COMMENT "Copying .qm files to ${QM_OUTPUT_DIR}"
)