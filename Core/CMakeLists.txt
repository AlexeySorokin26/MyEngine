cmake_minimum_required(VERSION 3.12)

set(CORE_PROJECT_NAME Core)

set(CORE_PUBLIC_INCLUDES
	includes/Application.hpp
	includes/Log.hpp
	includes/Event.hpp
)
set(CORE_PRIVATE_INCLUDES
	includes/Window.hpp
)
set(CORE_PRIVATE_SOURCES
	src/Application.cpp
	src/Window.cpp
)

add_library(${CORE_PROJECT_NAME} STATIC
	${CORE_PUBLIC_INCLUDES}
	${CORE_PRIVATE_INCLUDES}
	${CORE_PRIVATE_SOURCES}
)

target_include_directories(${CORE_PROJECT_NAME} PUBLIC
	includes
)

target_include_directories(${CORE_PROJECT_NAME} PRIVATE
	src
)

target_compile_features(${CORE_PROJECT_NAME} PUBLIC
	cxx_std_17
)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE
	glfw
)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE
	spdlog
)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE
	glad
)

set(IMGUI_INCLUDES 
	../external/imgui/imgui.h 
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_draw.cpp
	../external/imgui/imgui_demo.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
)

add_library(IMGUI STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(IMGUI
	PUBLIC
	../external
)
target_include_directories(IMGUI
	PUBLIC
	../external/imgui
)
target_link_libraries(IMGUI PRIVATE
	glad 
	glfw
)

target_link_libraries(${CORE_PROJECT_NAME} PRIVATE
	IMGUI
)

