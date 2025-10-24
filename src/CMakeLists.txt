# -------------------------------------
# Пути и общие настройки
# -------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# -------------------------------------
# Пути к исходникам
# -------------------------------------
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${SRC_DIR}
    ${SRC_DIR}/core
    ${SRC_DIR}/ui
    ${SRC_DIR}/ui/mainwindow
    ${SRC_DIR}/ui/pages
    ${SRC_DIR}/utils
)

# -------------------------------------
# Исходники и заголовки
# -------------------------------------
file(GLOB_RECURSE SOURCES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/core/*.cpp"
    "${SRC_DIR}/ui/*.cpp"
    "${SRC_DIR}/ui/**/*.cpp"
    "${SRC_DIR}/utils/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "${SRC_DIR}/core/*.h"
    "${SRC_DIR}/ui/*.h"
    "${SRC_DIR}/ui/**/*.h"
    "${SRC_DIR}/utils/*.h"
)

file(GLOB_RECURSE UIS
    "${SRC_DIR}/ui/**/*.ui"
)

# -------------------------------------
# Создание исполняемого файла
# -------------------------------------
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UIS}
)

# -------------------------------------
# Подключение библиотек
# -------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# -------------------------------------
# Копирование ресурсов (иконки и т.п.)
# -------------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/icons
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/icons
)
