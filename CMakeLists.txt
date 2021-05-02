cmake_minimum_required(VERSION 3.15)
project(ribbon)

set(CMAKE_CXX_STANDARD 14)

#add_executable(ribbon main.cpp)

#cmake_minimum_required(VERSION 3.10)
#project(qt_test)
#
## 指定c++标准的版本
#set(CMAKE_CXX_STANDARD 17)

# 自动调用moc，uic，rcc处理qt的扩展部分
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 设置Qt5的cmake模块所在目录，如果不设置将使用系统提供的版本
# QT_DIR和QT_VERSION是指定了qt安装目录和版本的环境变量
# 如果你使用了系统的cmake，那么会优先使用系统提供模块，如果不想发生这种行为你需要自己运行cmake或者使用CLion自带的
set(CMAKE_PREFIX_PATH $ENV{QT_DIR}/$ENV{QT_VERSION}/gcc_64/lib/cmake)

# 找到对应的qt模块，名字为qmake中QT += <name>中的name首字母大写后加上Qt5前缀
# 例如core为QtCore，你也可以去${CMAKE_PREFIX_PATH}的目录中找到正确的模块名
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

aux_source_directory(. DIRS_SRCS)

# 通常这样设置就可以，如果你的项目包含qrc文件，那么需要将它们单独添加进来
# 例如add_executable(test ${DIRS_SRCS} resources.qrc)
add_executable(ribbon ${DIRS_SRCS} FramelessHelper.cpp FramelessHelper.h RibbonGalleryItem.cpp RibbonGalleryItem.h RibbonGalleryGroup.cpp RibbonGalleryGroup.h RibbonGallery.cpp RibbonGallery.h RibbonControlButton.cpp RibbonControlButton.h RibbonMenu.cpp RibbonMenu.h RibbonElementCreateDelegate.cpp RibbonElementCreateDelegate.h RibbonElementManager.cpp RibbonElementManager.h RibbonPannelOptionButton.cpp RibbonPannelOptionButton.h RibbonPannel.cpp RibbonPannel.h RibbonToolButton.cpp RibbonToolButton.h RibbonQuickAccessBar.cpp RibbonQuickAccessBar.h RibbonCtrlContainer.cpp RibbonCtrlContainer.h RibbonDrawHelper.cpp RibbonDrawHelper.h RibbonButtonGroupWidget.cpp RibbonButtonGroupWidget.h RibbonSeparatorWidget.cpp RibbonSeparatorWidget.h RibbonCategory.cpp RibbonCategory.h RibbonCategoryLayout.cpp RibbonCategoryLayout.h RibbonContextCategory.cpp RibbonContextCategory.h RibbonBar.cpp RibbonBar.h RibbonTabBar.cpp RibbonTabBar.h RibbonApplicationButton.cpp RibbonApplicationButton.h RibbonStackedWidget.cpp RibbonStackedWidget.h RibbonMainWindow.cpp RibbonMainWindow.h WindowButtonGroup.cpp WindowButtonGroup.h)

# 把对应Qt模块的库链接进程序
target_link_libraries(ribbon Qt5::Widgets)
