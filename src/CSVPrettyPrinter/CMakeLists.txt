add_library(csv_printer_lib CSVPrettyPrinter.c)

add_executable(csvPrinter main.c)
target_link_libraries(csvPrinter csv_printer_lib)

add_executable(testRunner testRunner.c)
target_link_libraries(testRunner csv_printer_lib)