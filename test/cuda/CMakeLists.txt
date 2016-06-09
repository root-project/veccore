
set(CUDA_SEPARABLE_COMPILATION OFF)

cuda_add_executable(hello hello.cu)
add_test(CUDAHelloWorld hello)
