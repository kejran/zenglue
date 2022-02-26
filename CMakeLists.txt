cmake_minimum_required (VERSION 3.8)

project ("zenglue")
add_subdirectory(lib/ZenLib)
add_library (zenglue SHARED "zg_tex.cpp" "zg_vob.cpp" "zg_mesh.cpp" "zg_vdfs.cpp" "zg_world.cpp")
target_link_libraries(zenglue PRIVATE zenload vdfs squish)
