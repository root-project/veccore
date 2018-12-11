
set(CUDA_SEPARABLE_COMPILATION OFF)

cuda_add_executable(hello hello.cu)
target_link_libraries(hello VecCore)
add_test(CUDAHelloWorld hello)
