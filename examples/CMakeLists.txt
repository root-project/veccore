add_executable(simple simple.cc)

string(TOUPPER "FLAGS_${TARGET_ISA}" _target_isa_flags)

set_target_properties(simple
    PROPERTIES COMPILE_FLAGS "${${_target_isa_flags}}")

if (SIMD)
    target_link_libraries(simple Vc)
endif()
