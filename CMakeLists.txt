cmake_minimum_required(VERSION 3.16)
project(all_men_must_die LANGUAGES C CXX)

# Visual Studio: link the C++ runtime in, so no redistributable is needed to play.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(all-men-must-die src/main.cpp src/engine/audio.cpp)
target_include_directories(all-men-must-die PRIVATE src src/engine third_party)

find_package(OpenGL REQUIRED)
target_link_libraries(all-men-must-die PRIVATE OpenGL::GL)

if(WIN32)
  # Windows has no system freeglut: build it once, statically, from source.
  include(FetchContent)
  set(CMAKE_POLICY_VERSION_MINIMUM 3.5) # freeglut 3.4 predates CMake 4
  set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
  set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  set(FREEGLUT_BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
  FetchContent_Declare(freeglut
    URL https://github.com/FreeGLUTProject/freeglut/releases/download/v3.4.0/freeglut-3.4.0.tar.gz)
  FetchContent_MakeAvailable(freeglut)
  target_include_directories(all-men-must-die PRIVATE ${freeglut_SOURCE_DIR}/include)
  target_link_libraries(all-men-must-die PRIVATE freeglut_static)
  target_compile_definitions(all-men-must-die PRIVATE FREEGLUT_STATIC)
  # No console window behind the game.
  if(MSVC)
    target_link_options(all-men-must-die PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
  else()
    target_link_options(all-men-must-die PRIVATE -Wl,--subsystem,windows)
  endif()
else()
  # Ubuntu: sudo apt install freeglut3-dev
  find_package(GLUT REQUIRED)
  find_package(Threads REQUIRED)
  target_link_libraries(all-men-must-die PRIVATE GLUT::GLUT Threads::Threads ${CMAKE_DL_LIBS} m)
endif()

# The game looks for "assets" next to the program.
add_custom_command(TARGET all-men-must-die POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:all-men-must-die>/assets)

install(TARGETS all-men-must-die RUNTIME DESTINATION .)
install(DIRECTORY assets DESTINATION .)
