cmake_minimum_required(VERSION 3.10)
project(bank)
set (CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} main.cpp )

add_subdirectory(third_party/xxHash/cmake_unofficial third_party/xxHash/build EXCLUDE_FROM_ALL)

target_sources(${PROJECT_NAME} PRIVATE
    src/admin_filter.cpp
    src/bank_api.cpp
    src/bank.cpp
    src/change_flag.cpp
    src/log.cpp
    src/substr_view.cpp
    src/transaction.cpp
    src/user_filter.cpp
    src/user.cpp
    src/xxhash_str.cpp
)

if(USER_SAVE_LOC)
set(USER_SAVE ${USER_SAVE_LOC})
else()
set(USER_SAVE "\"users.json\"")
endif()

if(DROGON_CONFIG_LOC)
set(DROGON_CONFIG ${DROGON_CONFIG_LOC})
else()
set(DROGON_CONFIG "\"config.json\"")
endif()

configure_file(ccash_config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/ccash_config.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC third_party)
target_include_directories(${PROJECT_NAME} PUBLIC third_party/drogon/lib/inc)
target_include_directories(${PROJECT_NAME} PUBLIC third_party/base64/include)

add_subdirectory(third_party/drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries(${PROJECT_NAME} PRIVATE xxHash::xxhash)

# AVX2_CFLAGS=-mavx2 SSSE3_CFLAGS=-mssse3 SSE41_CFLAGS=-msse4.1 SSE42_CFLAGS=-msse4.2 AVX_CFLAGS=-mavx make lib/libbase64.o
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/base64/lib/libbase64.o)