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

target_sources(${PROJECT_NAME} PRIVATE
    src/bank_api.cpp
    src/bank_resp.cpp
    src/bank.cpp
    src/change_flag.cpp
    src/json_filter.cpp
    src/log.cpp
    src/simdjson.cpp
    src/str_intrusion.cpp
    src/transaction.cpp
    src/user_filter.cpp
    src/user.cpp
    src/xxhash_str.cpp
    src/xxhash.c

    fbe/user_model/bank_dom_final_models.cpp
    fbe/user_model/bank_dom_models.cpp
    fbe/user_model/bank_dom.cpp
    fbe/user_model/fbe_final_models.cpp
    fbe/user_model/fbe_models.cpp
    fbe/user_model/fbe.cpp
)

if(DEFINED USER_SAVE_LOC)
set(USER_SAVE ${USER_SAVE_LOC})
else()
set(USER_SAVE "\"users.dat\"")
endif()

if(DEFINED DROGON_CONFIG_LOC)
set(DROGON_CONFIG ${DROGON_CONFIG_LOC})
else()
set(DROGON_CONFIG "\"config.json\"")
endif()

if(DEFINED MAX_LOG_SIZE)
set(MAX_LOG_SIZE_VAL ${MAX_LOG_SIZE})
else()
set(MAX_LOG_SIZE_VAL 100)
endif()

if(DEFINED CONSERVATIVE_DISK_SAVE)
set(CONSERVATIVE_DISK_SAVE_VAL ${CONSERVATIVE_DISK_SAVE})
else()
set(CONSERVATIVE_DISK_SAVE_VAL true)
endif()

if(DEFINED MULTI_THREADED)
set(MULTI_THREADED_VAL ${MULTI_THREADED})
else()
set(MULTI_THREADED_VAL true)
endif()

if(DEFINED RETURN_ON_DEL_NAME)
set(RETURN_ON_DEL_VAL true)
set(RETURN_ON_DEL_NAME_VAL "\"" + ${RETURN_ON_DEL_NAME} + "\"")
else()
set(RETURN_ON_DEL_VAL false)
set(RETURN_ON_DEL_NAME_VAL "\"\"")
endif()

configure_file(ccash_config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/ccash_config.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC fbe/user_model)
target_include_directories(${PROJECT_NAME} PUBLIC third_party)
target_include_directories(${PROJECT_NAME} PUBLIC third_party/drogon/lib/inc)
target_include_directories(${PROJECT_NAME} PUBLIC third_party/base64/include)

add_subdirectory(third_party/drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} )

#AVX2_CFLAGS=-mavx2 SSSE3_CFLAGS=-mssse3 SSE41_CFLAGS=-msse4.1 SSE42_CFLAGS=-msse4.2 AVX_CFLAGS=-mavx make lib/libbase64.o
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/base64/lib/libbase64.o)
