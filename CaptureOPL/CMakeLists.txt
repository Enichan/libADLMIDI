add_executable(CaptureOPL CaptureOPL.cpp)

add_subdirectory(OPBinaryLib)

target_link_libraries(CaptureOPL PUBLIC opb ADLMIDI_static)