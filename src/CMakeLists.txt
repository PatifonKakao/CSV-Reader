set(SOURCES
CSVReader.h
CSVReader.cpp
)

add_library(CSVReader_lib ${SOURCES})

target_include_directories(CSVReader_lib PUBLIC ${SRC_ROOT_DIR})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${SOURCES})

set_property(TARGET CSVReader_lib  PROPERTY FOLDER "CSVReader_lib")

add_subdirectory(tests)