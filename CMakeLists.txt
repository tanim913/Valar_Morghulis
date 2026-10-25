cmake_minimum_required(VERSION 3.16)
project(all_men_must_die VERSION 1.0.0 LANGUAGES C CXX)
if(WIN32)
  enable_language(RC) # for the icon
endif()

# Visual Studio: link the C++ runtime in, so no redistributable is needed to play.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(all-men-must-die src/main.cpp src/engine/audio.cpp)
if(WIN32)
  target_sources(all-men-must-die PRIVATE packaging/windows/app.rc)
endif()
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
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # One less library to install: the C++ runtime travels inside the program.
    target_link_options(all-men-must-die PRIVATE -static-libstdc++ -static-libgcc)
  endif()
endif()

# The game looks for "assets" next to the program.
add_custom_command(TARGET all-men-must-die POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:all-men-must-die>/assets)

install(TARGETS all-men-must-die RUNTIME DESTINATION .)
install(DIRECTORY assets DESTINATION .)

# Windows setup program (cpack -G NSIS): Start menu and desktop icons, an uninstaller.
# Ubuntu uses packaging/linux/make_deb.sh instead.
if(WIN32)
  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/packaging/windows/game.ico" GAME_ICON)
  set(CPACK_PACKAGE_NAME "All Men Must Die")
  set(CPACK_PACKAGE_VENDOR "tanim913")
  set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
  set(CPACK_PACKAGE_FILE_NAME "all-men-must-die-windows-setup")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "All Men Must Die")
  set(CPACK_PACKAGE_EXECUTABLES "all-men-must-die" "All Men Must Die")
  set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
  set(CPACK_NSIS_DISPLAY_NAME "All Men Must Die")
  set(CPACK_NSIS_MUI_ICON "${GAME_ICON}")
  set(CPACK_NSIS_MUI_UNIICON "${GAME_ICON}")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "all-men-must-die.exe")
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
  set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$DESKTOP\\All Men Must Die.lnk' '$INSTDIR\\all-men-must-die.exe'")
  set(CPACK_NSIS_DELETE_ICONS_EXTRA "Delete '$DESKTOP\\All Men Must Die.lnk'")
  include(CPack)
endif()

# Checks for the score list: ctest --test-dir build
enable_testing()
add_executable(test_scores tests/test_scores.cpp)
target_include_directories(test_scores PRIVATE src)
add_test(NAME scores COMMAND test_scores)
