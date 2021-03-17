cmake_minimum_required (VERSION 3.8)

project ("zengluec")
add_subdirectory(lib/ZenLib)
add_library (zenglue SHARED "zenglue.cpp" "zg_vob.cpp" "zg_mesh.cpp" "zg_vdfs.cpp")

target_link_libraries(zenglue PRIVATE zenload vdfs squish)