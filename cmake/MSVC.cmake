
# Note: MSVC does not have an option equivalent to -march=native
# https://docs.microsoft.com/en-us/cpp/build/reference/arch-x86?view=vs-2019

# Disable annoying warnings about unsafe type conversions and MSVC's headers.
# We know the type conversions we do in the tests are safe.
add_compile_options(/wd4244 /wd4267 /wd4305 /wd4756)

set(FLAGS_SSE    "/arch:SSE")
set(FLAGS_SSE2   "/arch:SSE2")
set(FLAGS_AVX    "/arch:AVX")
set(FLAGS_AVX2   "/arch:AVX2")
set(FLAGS_AVX512 "/arch:AVX512")

