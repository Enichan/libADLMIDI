# OPBinaryLib
add_library(
    OPBinaryLib
    OPBinaryLib/opblib.c
    OPBinaryLib/opblib.h
)

target_include_directories(OPBinaryLib INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# CaptureOPL
add_executable(CaptureOPL captureOPL.cpp)

# Normally `capture_cp.hpp` would be split into it's own module that both
# CaptureOPL and adlmidi would depend on. But for one file that's just
# yakshaving. So instead let CaptureOPL have visibility into adlmidi's internals
# to get visability of `capture_cp.hpp`
target_include_directories(
    CaptureOPL
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../src
)

target_link_libraries(
    CaptureOPL
    PRIVATE
        OPBinaryLib
        ADLMIDI
)

set_nopie(CaptureOPL)

install(TARGETS CaptureOPL
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
