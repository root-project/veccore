add_executable(simple simple.cc)
add_executable(quadsolve quadsolve.cc)

string(TOUPPER "FLAGS_${TARGET_ISA}" _target_isa_flags)

set_target_properties(simple quadsolve
    PROPERTIES COMPILE_FLAGS "${${_target_isa_flags}}")

if (SIMD)
    target_link_libraries(simple Vc)
    target_link_libraries(quadsolve Vc)
endif()
