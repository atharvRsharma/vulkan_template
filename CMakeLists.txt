cmake_minimum_required(VERSION 3.10)
project(VulkanTutorial)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force MSVC to use C++20 with designated initializers
if(MSVC)
    add_compile_options(/std:c++20)
    add_compile_options(/Zc:preprocessor)
endif()

# Find all the packages that vcpkg installed
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Ktx CONFIG REQUIRED)

# Create your executable
add_executable(vulkan_app main.cpp)

# Enable designated initializers for Vulkan-Hpp
target_compile_definitions(vulkan_app PUBLIC VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1)

# Link all the libraries
target_link_libraries(vulkan_app PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
    nlohmann_json::nlohmann_json
    tinyobjloader::tinyobjloader
    KTX::ktx
)

# Include Stb headers
target_include_directories(vulkan_app PRIVATE ${Stb_INCLUDE_DIR})