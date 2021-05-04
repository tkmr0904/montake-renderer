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
  find_package(tinyobjloader CONFIG REQUIRED)

  include_directories( ${OpenCV_INCLUDE_DIRS} ../include)
  add_executable(${FileName} main.cpp)
  target_link_libraries( ${FileName} PRIVATE ${OpenCV_LIBS} raytracing)
  target_link_libraries(${FileName} PRIVATE tinyobjloader::tinyobjloader)

  
    
