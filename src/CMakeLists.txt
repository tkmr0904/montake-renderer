set(FileName a.out)

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

  find_package( OpenCV REQUIRED ) 

  include_directories( ${OpenCV_INCLUDE_DIRS})
  add_executable(${FileName} makepictures.cpp)
  target_link_libraries( ${FileName} ${OpenCV_LIBS} raytracing)
