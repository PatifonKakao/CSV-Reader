set(SOURCES
    tests.cpp
)

add_executable(csvreader ${SOURCES})

target_link_libraries(csvreader PUBLIC CSVReader_lib)
target_link_libraries(csvreader PUBLIC gtest_main)

set_property(TARGET csvreader PROPERTY FOLDER "tests/csvreader")