cmake_minimum_required(VERSION 3.15)

project(gcstr)

set(SOURCES
  convenience.c
  convenience.h
  error.c
  error.h
  filedata.c
  filedata.h
  gcstr.h
  gotypes.h
  main.c
  panic.c
  panic.h
  print.c
  print.h
  str.c
  str.h
  stringlist.c
  stringlist.h
  structs.h)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

# GC
include(FindPkgConfig)
pkg_search_module(GC REQUIRED bdw-gc)
include_directories(${GC_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GC_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include)

set(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/share/cmake/${PROJECT_NAME})

set(INSTALL_PC_DIR ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

install(FILES
  convenience.h
  error.h
  filedata.h
  gcstr.h
  gotypes.h
  panic.h
  print.h
  str.h
  stringlist.h
  structs.h
  DESTINATION "include/${PROJECT_NAME}"
  COMPONENT dev
)

install(FILES
  gcstr-config.cmake
  gcstr-config-version.cmake
  DESTINATION "${INSTALL_CMAKE_DIR}"
  COMPONENT dev
)

install(FILES
  gcstr.pc
  DESTINATION "${INSTALL_PC_DIR}"
  COMPONENT dev
)
