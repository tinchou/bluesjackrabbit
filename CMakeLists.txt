cmake_minimum_required(VERSION 2.8.4)
project(BluesJackrabbit)

set(CMAKE_CXX_STANDARD 98)

if(CMAKE_COMPILER_IS_GNUCXX)
    # Errors and warnings.
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -Wconversion -Wtype-limits -pedantic -pedantic-errors")
    # Gtkmm
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-inline -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DGDKMM_DISABLE_DEPRECATED -DGTKMM_DISABLE_DEPRECATED")
    # Valgrind and debug
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} -ggdb -DDEBUG")
    # Optimizing
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
find_package(Cppunit REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Threads)
include(CppLint)
enable_testing()

pkg_check_modules(CAIROMM REQUIRED cairomm-1.0)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)
pkg_check_modules(SIGC REQUIRED sigc++-2.0)
pkg_check_modules(LOG4CPP REQUIRED log4cpp)
pkg_check_modules(GOOCANVAS REQUIRED goocanvasmm-2.0)
pkg_check_modules(LIBXML REQUIRED libxml++-2.6)
include_directories(.)
include_directories(SYSTEM ${CAIROMM_INCLUDE_DIRS})
include_directories(SYSTEM ${GLIBMM_INCLUDE_DIRS})
include_directories(SYSTEM ${GTKMM3_INCLUDE_DIRS})
include_directories(SYSTEM ${SIGC_INCLUDE_DIRS})
include_directories(SYSTEM ${LOG4CPP_INCLUDE_DIRS})
include_directories(SYSTEM ${GOOCANVAS_INCLUDE_DIRS})
include_directories(SYSTEM ${LIBXML_INCLUDE_DIRS})
add_subdirectory(client)
add_subdirectory(common)
add_subdirectory(editor)
add_subdirectory(engine)
add_subdirectory(server)
add_subdirectory(spikes)
