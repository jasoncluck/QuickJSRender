cmake_minimum_required(VERSION 3.16)
project(QuickJS_SDL3_App)

# Use C++17 for modern SDL3 and QuickJS-ng compatibility
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Include QuickJS-ng as a sub-project
# This builds the 'qjs' target automatically
add_subdirectory(deps/quickjs)

# 2. Find SDL3 and SDL3_ttf
# Note: On macOS, ensure you've run 'brew install sdl3 sdl3_ttf'
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

# 3. Handle the SDL3_ttf target naming quirk
# Some versions of SDL3_ttf don't export the :: target properly yet
if(TARGET SDL3_ttf::SDL3_ttf)
    set(SDL_TTF_LIB SDL3_ttf::SDL3_ttf)
else()
    set(SDL_TTF_LIB ${SDL3_TTF_LIBRARIES})
    include_directories(${SDL3_TTF_INCLUDE_DIRS})
endif()

# 4. Define our executable and its source files
# We include JSBridge.cpp here so it compiles into the final binary
add_executable(MyApp 
    src/main.cpp 
    src/JSBridge.cpp
)

# 5. Setup Include Directories
# This allows 'main.cpp' to find 'JSBridge.hpp' and 'quickjs.h'
target_include_directories(MyApp PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/deps/quickjs
)

# 6. Link Libraries
# 'qjs' comes from the add_subdirectory(deps/quickjs)
# 'm' and 'dl' are required for QuickJS on Unix/macOS
target_link_libraries(MyApp PRIVATE 
    SDL3::SDL3 
    ${SDL_TTF_LIB}
    qjs
    m 
    dl
)

# 7. Post-build convenience (Optional)
# This ensures the binary is easy to find in the root for quick testing
set_target_properties(MyApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
