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

add_library(${CORE_PROJECT_NAME}
	STATIC
	${CORE_PUBLIC_INCLUDES}
	${CORE_PRIVATE_INCLUDES}
	${CORE_PRIVATE_SOURCES}
)

target_include_directories(${CORE_PROJECT_NAME}
	PUBLIC
	includes
)

target_include_directories(${CORE_PROJECT_NAME}
	PRIVATE
	src
)

target_compile_features(${CORE_PROJECT_NAME}
	PUBLIC
	cxx_std_17
)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME}
	PRIVATE
	glfw
)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${CORE_PROJECT_NAME}
	PRIVATE
	spdlog
)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_PROJECT_NAME}
	PRIVATE
	glad
)
