add_executable(backend backend.cc)
add_executable(geometry geometry.cc)
add_executable(transform transform.cc)

string(TOUPPER "FLAGS_${TARGET_ISA}" _target_isa_flags)

set_target_properties(backend geometry transform
    PROPERTIES COMPILE_FLAGS "${${_target_isa_flags}}")

if (SIMD)
    target_link_libraries(geometry Vc)
    target_link_libraries(transform Vc)
    target_link_libraries(backend Vc)
endif()
