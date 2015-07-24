
find_package(Vc REQUIRED)

add_executable(backend backend.cc)
target_link_libraries(backend Vc)
