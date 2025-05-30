cmake_minimum_required(VERSION 3.10)
project(DietManager C)

set(CMAKE_C_STANDARD 99)

# 공통 소스
set(COMMON_SRC UserInfo.c)
include_directories(${CMAKE_SOURCE_DIR})

# 클라이언트 빌드
add_executable(
    client
    Client/MainClient.c
    ßClient/ClientController.c
    Client/MenuIO.c
    ${COMMON_SRC}
)

# 서버 빌드
add_executable(
    server
    Server/MainServer.c
    Server/DBM.c
    ${COMMON_SRC}
)

# sqlite3 라이브러리 찾기 및 링크
find_library(SQLITE3_LIB sqlite3)

if(SQLITE3_LIB)
    target_link_libraries(server ${SQLITE3_LIB})
else()
    target_link_libraries(server sqlite3)  # 기본값
endif()