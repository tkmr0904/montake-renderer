add_library(raytracing
  STATIC
    camera.cpp
    draw.cpp
    hit.cpp
    image.cpp
    rand.cpp
    ray.cpp
    shapes.cpp
    sky.cpp
    texture.cpp
    vec.cpp
  )

  target_include_directories(raytracing
  PUBLIC ${PROJECT_SOURCE_DIR}/Header
  )
add_executable(a.out makepictures.cpp)
# a.outをコンパイルする際にraytracingをリンクする
target_link_libraries(a.out raytracing)