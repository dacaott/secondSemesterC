add_executable(countries_app 
    main.c 
    countries.c 
    heap.c 
    countries.h 
    heap.h
)


add_executable(tests_app 
    testRunner.c 
    countries.c 
    heap.c 
    countries.h 
    heap.h
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/results)