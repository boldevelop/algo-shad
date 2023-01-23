project(algo-shad)

cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD             17)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/tools/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

find_package(Catch REQUIRED)

include(tools/cmake/TestSolution.cmake)
include(tools/cmake/BuildFlags.cmake)
include_directories(tools/util)

function(add_task contest name)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/contest${contest}/${name})
    add_subdirectory(contest${contest}/${name})
  endif()
endfunction()

add_task(2 b)

add_task(3 a)
add_task(3 b)
add_task(3 c)
add_task(3 d)
add_task(3 e)

add_task(4 c)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# ${CMAKE_BINARY_DIR} - /home/boldakov/algo-shad/build
# ${CMAKE_CURRENT_SOURCE_DIR} - /home/boldakov/algo-shad
# ${CMAKE_SOURCE_DIR} - /home/boldakov/algo-shad

### Запуск скриптовых файлов

# include
# заменяет строку своего вызова кодом заданного файла, действуя аналогично препроцессорной команде include языков C/C++.

### Компиляция исполняемых файлов

# add_executable
# компилирует исполняемый файл с заданным именем из списка исходников.
# add_executable(MyExecutable ObjectHandler.c TimeManager.c MessageGenerator.c)

### Компиляция библиотек

# add_library
# компилирует библиотеку с указанным видом и именем из исходников. (например, lib<LibraryName>.a или <LibraryName>.lib).
# add_library(MyLibrary STATIC ObjectHandler.c TimeManager.c MessageConsumer.c)
# * STATIC Статические библиотеки.
# Представляют из себя архивы объектных файлов, связываемых с исполняемыми файлами и другими библиотеками во время компиляции.
# * SHARED Динамические библиотеки.
# Представляют из себя двоичные библиотеки, загружаемые операционной системой во время выполнения программы.
# * MODULE Модульные библиотеки
# Представляют из себя двоичные библиотеки, загружаемые посредством техник выполнения самим исполняемым файлом.
# * OBJECT Объектные библиотеки
# Представляют из себя набор объектных файлов, связываемых с исполняемыми файлами и другими библиотеками во время компиляции.

### Добавление исходников к цели

# target_sources
# Бывают случаи, требующие многократного добавления исходных файлов к цели.
# Команда способная добавлять исходники к цели множество раз.
# Повторяющиеся вызовы команды target_sources добавляют исходные файлы к цели в том порядке, в каком они были вызваны
# target_sources(MyExecutable MessageConsumer.c)
# target_sources(MyExecutable ResultHandler.c)

### Генерируемые файлы

# Местоположение выходных файлов, сгенерированных командами add_executable и add_library, определяется только на стадии генерации
# * Переменные RUNTIME_OUTPUT_DIRECTORY и RUNTIME_OUTPUT_NAME определяют местоположение целей выполнения.
# * Переменные LIBRARY_OUTPUT_DIRECTORY и LIBRARY_OUTPUT_NAME определяют местоположение библиотек.
# * Переменные ARCHIVE_OUTPUT_DIRECTORY и ARCHIVE_OUTPUT_NAME определяют местоположение архивов.

### Компоновка с библиотеками

# target_link_libraries
# компонует библиотеку или исполняемый файл с другими предоставляемыми библиотеками.
# Первым аргументом данная команда принимает название цели, сгенерированной с помощью команд add_executable или add_library,
# а последующие аргументы представляют собой названия целей библиотек или полные пути к библиотекам.
# target_link_libraries(MyExecutable JsonParser SocketFactory BrowserInvoker)

### Добавление подпроектов

# add_subdirectory
# Побуждает CMake к незамедлительной обработке указанного файла подпроекта.
#
# Добавить каталог "subLibrary" в сборку основного проекта,
# а генерируемые файлы расположить в каталоге "subLibrary/build":
# add_subdirectory(subLibrary subLibrary/build)
#

### Поиск пакетов

# find_package
# находит и загружает настройки внешнего проекта
# В большинстве случаев она применяется для последующей линковки внешних библиотек, таких как Boost и GSL.
# CMake ищет скриптовый файл вида Find<PackageName>.cmake в директории CMAKE_MODULE_PATH, а затем запускает его
# и импортирует все необходимые настройки (в данном случае CMake запустила стандартный файл FindGSL.cmake)
#
# Загрузить настройки пакета библиотеки "GSL":
# find_package(GSL 2.5 REQUIRED) or QUIET
#
# Скомпоновать исполняемый файл с библиотекой "GSL":
# target_link_libraries(MyExecutable GSL::gsl)
#
# Уведомить компилятор о каталоге заголовков "GSL":
# target_include_directories(MyExecutable ${GSL_INCLUDE_DIRS})
#

# Проверить результат поиска пакета
# Это можно сделать путём проверки переменной <PackageName>_FOUND,
# автоматически определяемой после завершения команды find_package.
# Например, в случае успешного импортирования настроек GSL в Ваш проект, переменная GSL_FOUND обратится в истину.

### Способы включения заголовков

# Информировать компилятора о располжении включаемых заголовков можно посредством двух команд: include_directories и target_include_directories.
#
# include_directories
# Влияет на область каталога.
# Это означает, что все директории заголовков, указанные данной командой, будут применяться для всех целей текущего CMakeLists.txt,
# а также для обрабатываемых подпроектов (смотреть add_subdirectory).
#
# target_include_directories
# Влияет лишь на указанную первым аргументом цель,
# а на другие цели никакого воздействия не оказывается.
##
# add_executable(RequestGenerator RequestGenerator.c)
# add_executable(ResponseGenerator ResponseGenerator.c)
#
# Применяется лишь для цели "RequestGenerator":
# target_include_directories(RequestGenerator headers/specific)
#
# Применяется для целей "RequestGenerator" и "ResponseGenerator":
# include_directories(headers)
#
# include_directories и link_libraries является нежелательным !

### Установка проекта

# install
# генерирует установочные правила для Вашего проекта. Данная команда способна работать с целями, файлами, папками и многим другим.
#
# # Установить цели "TimePrinter" и "DataScanner" в директорию "bin":
# install(TARGETS TimePrinter DataScanner DESTINATION bin)
#
# # Установить файлы "DataCache.txt" и "MessageLog.txt" в директорию "~/":
# install(FILES DataCache.txt MessageLog.txt DESTINATION ~/)
#
# # Установить каталоги "MessageCollection" и "CoreFiles" в директорию "~/":
# install(DIRECTORY MessageCollection CoreFiles DESTINATION ~/)
#
