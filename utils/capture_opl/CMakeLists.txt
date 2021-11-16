# OPBinaryLib
add_library(
    OPBinaryLib
    OPBinaryLib/opblib.c
    OPBinaryLib/opblib.h
)

target_include_directories(OPBinaryLib INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# CaptureOPL
add_executable(CaptureOPL captureOPL.cpp)

target_link_libraries(
    CaptureOPL
    PRIVATE
        OPBinaryLib
        ADLMIDI
)

set_nopie(CaptureOPL)

install(TARGETS CaptureOPL
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")