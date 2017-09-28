cmake_minimum_required(VERSION 2.8)

project(tinyxml)

include_directories(
    "${PROJECT_SOURCE_DIR}"
)

add_library(tinyxml
    tinystr.cpp
    tinyxml.cpp
    tinyxmlerror.cpp
    tinyxmlparser.cpp
)

install(
    TARGETS tinyxml
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib/tinyxml
    LIBRARY DESTINATION lib
)

set(TINYXML_INCLUDE_FILES
    tinystr.h
    tinyxml.h
)

install(FILES ${TINYXML_INCLUDE_FILES} DESTINATION include)
