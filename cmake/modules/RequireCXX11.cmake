include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(--std=c++11 SUPPORTS_STD_CXX11)
check_cxx_compiler_flag(--std=c++0x SUPPORTS_STD_CXX01)

if(SUPPORTS_STD_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
elseif(SUPPORTS_STD_CXX01)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
else()
	message(ERROR "Compiler does not support required C++11 standard.")
endif()
