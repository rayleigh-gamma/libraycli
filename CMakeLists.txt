cmake_minimum_required(VERSION 3.10)

if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (MSVC)
	set(COMPILE_FLAGS /W4 /WX)
endif()

set(NAME "raycli")

add_subdirectory("./third_party/librayutils/")

project(${NAME} VERSION 0.1.0 LANGUAGES C)

include_directories("./include/")
add_library(${NAME})

target_sources(${NAME} PRIVATE
	"./src/lexer/lexer.c"
	"./src/parser/cmdline/cmdline.c"
	"./src/parser/parser.c"
)

target_link_libraries(${NAME} PRIVATE rayutils)

if (CMAKE_VERSION VERSION_GREATER 3.10)
	set_property(TARGET ${NAME} PROPERTY CXX_STANDARD 20)
endif()
