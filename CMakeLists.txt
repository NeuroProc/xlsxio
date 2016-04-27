CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(xlsxio)

# parameters
SET(BUILD_STATIC ON CACHE BOOL "Build static libraries")
SET(BUILD_SHARED ON CACHE BOOL "Build shared libraries")
SET(BUILD_TOOLS ON CACHE BOOL "Build tools")
SET(BUILD_EXAMPLES ON CACHE BOOL "Build examples")

# conditions
IF(NOT BUILD_STATIC AND NOT BUILD_SHARED)
  MESSAGE(FATAL_ERROR "Cannot build with both BUILD_STATIC and BUILD_SHARED disabled")
ENDIF()

# dependancies
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
FIND_PACKAGE(libzip REQUIRED)
FIND_PACKAGE(expat REQUIRED)

# build parameters
#SET(CMAKE_CXX_FLAGS "-g -Wall")
#ADD_SUBDIRECTORY(src)

#IF(NOT WIN32)
#-pthread
#ENDIF

INCLUDE_DIRECTORIES(include)

# build definitions
SET(ALLTARGETS)
SET(LINKTYPES)
IF(BUILD_STATIC)
  LIST(APPEND LINKTYPES "STATIC")
ENDIF()
IF(BUILD_SHARED)
  LIST(APPEND LINKTYPES "SHARED")
ENDIF()

FOREACH(LINKTYPE ${LINKTYPES})
  SET(LINKTYPEDEFINE)
  IF(LINKTYPE STREQUAL "STATIC")
    SET(LINKTYPEDEFINE "BUILD_XLSXIO_STATIC")
  ENDIF()
  IF(LINKTYPE STREQUAL "SHARED")
    SET(LINKTYPEDEFINE "BUILD_XLSXIO_DLL")
  ENDIF()
  
  ADD_LIBRARY(xlsxio_read_${LINKTYPE} ${LINKTYPE} lib/xlsxio_read.c)
  SET_TARGET_PROPERTIES(xlsxio_read_${LINKTYPE} PROPERTIES DEFINE_SYMBOL ${LINKTYPEDEFINE})
  SET_TARGET_PROPERTIES(xlsxio_read_${LINKTYPE} PROPERTIES OUTPUT_NAME xlsxio_read)
  TARGET_LINK_LIBRARIES(xlsxio_read_${LINKTYPE} ${LIBZIP_LIBRARIES} ${EXPAT_LIBRARIES})
  SET(ALLTARGETS ${ALLTARGETS} xlsxio_read_${LINKTYPE})

  ADD_LIBRARY(xlsxio_write_${LINKTYPE} ${LINKTYPE} lib/xlsxio_write.c)
  SET_TARGET_PROPERTIES(xlsxio_write_${LINKTYPE} PROPERTIES DEFINE_SYMBOL ${LINKTYPEDEFINE})
  SET_TARGET_PROPERTIES(xlsxio_write_${LINKTYPE} PROPERTIES OUTPUT_NAME xlsxio_write)
  TARGET_LINK_LIBRARIES(xlsxio_write_${LINKTYPE} ${LIBZIP_LIBRARIES})
  SET(ALLTARGETS ${ALLTARGETS} xlsxio_write_${LINKTYPE})

  SET(EXELINKTYPE ${LINKTYPE})
ENDFOREACH()

IF(BUILD_TOOLS)
  ADD_EXECUTABLE(xlsxio_xlsx2csv src/xlsxio_xlsx2csv.c)
  TARGET_LINK_LIBRARIES(xlsxio_xlsx2csv xlsxio_read_${EXELINKTYPE})
  SET(ALLTARGETS ${ALLTARGETS} xlsxio_xlsx2csv)

  ADD_EXECUTABLE(xlsxio_csv2xlsx src/xlsxio_csv2xlsx.c)
  TARGET_LINK_LIBRARIES(xlsxio_csv2xlsx xlsxio_write_${EXELINKTYPE})
  SET(ALLTARGETS ${ALLTARGETS} xlsxio_csv2xlsx)
ENDIF()

IF(BUILD_EXAMPLES)
  ADD_EXECUTABLE(example_xlsxio_write_getversion examples/example_xlsxio_write_getversion.c)
  TARGET_LINK_LIBRARIES(example_xlsxio_write_getversion xlsxio_write_${EXELINKTYPE})
  SET(ALLTARGETS ${ALLTARGETS} example_xlsxio_write_getversion)

  ADD_EXECUTABLE(example_xlsxio_write examples/example_xlsxio_write.c)
  TARGET_LINK_LIBRARIES(example_xlsxio_write xlsxio_write_${EXELINKTYPE})
  SET(ALLTARGETS ${ALLTARGETS} example_xlsxio_write)

  ADD_EXECUTABLE(example_xlsxio_read examples/example_xlsxio_read.c)
  TARGET_LINK_LIBRARIES(example_xlsxio_read xlsxio_read_${EXELINKTYPE})
  SET(ALLTARGETS ${ALLTARGETS} example_xlsxio_read)

  ADD_EXECUTABLE(example_xlsxio_read_advanced examples/example_xlsxio_read_advanced.c)
  TARGET_LINK_LIBRARIES(example_xlsxio_read_advanced xlsxio_read_${EXELINKTYPE})
  SET(ALLTARGETS ${ALLTARGETS} example_xlsxio_read_advanced)
ENDIF()

# install definitions
INSTALL(TARGETS ${ALLTARGETS}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
INSTALL(DIRECTORY include/
  DESTINATION include 
  FILES_MATCHING PATTERN "xlsxio*.h"
)