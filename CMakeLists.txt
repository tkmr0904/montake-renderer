# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.1)
# プロジェクト名と使用する言語を設定
project(raytracing CXX)

set(CMAKE_CXX_FLAGS "-Wall -O3 -pthread")

# サブディレクトリを登録
add_subdirectory(Source)