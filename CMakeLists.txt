cmake_minimum_required(VERSION 3.25)
project(VirtualMidiTranslator)

set(CMAKE_CXX_STANDARD 20)

add_executable(VirtualMidiTranslator main.cpp MidiMapper.cpp MidiMapper.h)

find_package(PkgConfig)

pkg_check_modules(rtmidi REQUIRED)
pkg_check_modules(jsoncpp REQUIRED)

target_link_libraries(VirtualMidiTranslator rtmidi)
target_include_directories(VirtualMidiTranslator PUBLIC ${RTMIDI_INCLUDE_DIRS})

target_link_libraries(VirtualMidiTranslator jsoncpp)
