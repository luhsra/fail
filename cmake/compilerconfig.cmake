##### Verbose make ####
option( VERBOSE_MAKE "Verbose Makefile output" OFF) # defaults to OFF
	set(CMAKE_VERBOSE_MAKEFILE ${VERBOSE_MAKE})


##### Compilers #####
SET( COMPILER "ag++" CACHE STRING "Use clang/gcc/ag++") # Defaults to ag++

if(${COMPILER} STREQUAL "clang")
  set(CMAKE_C_COMPILER "clang")
  set(CMAKE_CXX_COMPILER "clang++")

elseif(${COMPILER} STREQUAL "gcc")
  set(CMAKE_C_COMPILER "gcc")
  set(CMAKE_CXX_COMPILER "g++")

elseif(${COMPILER} STREQUAL "ag++")
  set(CMAKE_C_COMPILER "ag++")
  set(CMAKE_CXX_COMPILER "ag++")
  set(CMAKE_AGPP_FLAGS "--real-instances" CACHE STRING "Additional ag++ flags, e.g. --real-instances --keep_woven")
  ## Here we add the build dir holding the generated header files (protobuf)
  add_definitions("-p ${CMAKE_SOURCE_DIR}" ${CMAKE_AGPP_FLAGS} --Xcompiler)

else(${COMPILER} STREQUAL "clang")
  message(FATAL_ERROR "COMPILER must be exactly one of clang/gcc/ag++.  If unsure, use 'ag++'.")
endif(${COMPILER} STREQUAL "clang")

add_definitions(-D_FILE_OFFSET_BITS=64)

message(STATUS "[${PROJECT_NAME}] Compiler: ${CMAKE_C_COMPILER}/${CMAKE_CXX_COMPILER}" )
