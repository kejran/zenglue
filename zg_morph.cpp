#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCMorphMesh.h>

#define EXPORT extern "C" __declspec(dllexport)

using Mesh = ZenLoad::zCMorphMesh;

EXPORT Mesh *zg_morph_init(VDFS::FileIndex *vdfs, const char *name) {
	return new Mesh(name, *vdfs);
}

EXPORT void zg_morph_deinit(Mesh *mesh) {
	delete mesh;
}

EXPORT uint32_t zg_morph_blend_count(Mesh *mesh) {
	return static_cast<uint32_t>(mesh->aniList.size());
}

EXPORT uint32_t zg_morph_frame_count(Mesh *mesh, uint32_t index) {
	return static_cast<uint32_t>(mesh->aniList[index].numFrames);
}

EXPORT uint32_t zg_morph_vertex_count(Mesh *mesh, uint32_t index) {
	return static_cast<uint32_t>(mesh->aniList[index].vertexIndex.size());
}

EXPORT uint32_t zg_morph_vertex_index_get(Mesh *mesh, uint32_t aniindex, uint32_t vertexindex) {
	return static_cast<uint32_t>(mesh->aniList[aniindex].vertexIndex[vertexindex]);
}

EXPORT float *zg_morph_vertex_sample_get(Mesh *mesh, uint32_t aniindex, uint32_t sampleindex) {
	return mesh->aniList[aniindex].samples[sampleindex].v;
}

EXPORT const char *zg_morph_name_get(Mesh *mesh, uint32_t index) {
	return mesh->aniList[index].name.c_str();
}

EXPORT ZenLoad::PackedMesh *zg_morph_mesh(Mesh *mesh) {
	auto result = new ZenLoad::PackedMesh();
	mesh->getMesh().packMesh(*result, false);
	return result;
}
