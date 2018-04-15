add_library(unqlite unqlite.c)
target_include_directories(unqlite PUBLIC .)
target_compile_definitions(unqlite PRIVATE UNQLITE_ENABLE_THREADS)
