#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCProgMeshProto.h>
#include <zenload/zCModelMeshLib.h>
#include <zenload/modelScriptParser.h>

#define EXPORT extern "C" __declspec(dllexport)

// SKINNED

EXPORT ZenLoad::PackedSkeletalMesh *zg_skinnedmesh_init(ZenLoad::zCModelMeshLib *lib) {
	auto result = new ZenLoad::PackedSkeletalMesh();
	lib->packMesh(*result);
	return result;
}

EXPORT void zg_skinnedmesh_deinit(ZenLoad::PackedSkeletalMesh *mesh) {
	delete mesh;
}

EXPORT uint32_t zg_skinnedmesh_vertex_count(ZenLoad::PackedSkeletalMesh *mesh) {
	return static_cast<uint32_t>(mesh->vertices.size());
}

EXPORT float *zg_skinnedmesh_vertex_positions_get(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index) {
	return mesh->vertices[index].LocalPositions[0].v;
}

EXPORT unsigned char *zg_skinnedmesh_vertex_boneindices_get(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index) {
	return mesh->vertices[index].BoneIndices;
}

EXPORT float *zg_skinnedmesh_vertex_normal_get(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index) {
	return mesh->vertices[index].Normal.v;
}

EXPORT float *zg_skinnedmesh_vertex_texcoord_get(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index) {
	return mesh->vertices[index].TexCoord.v;
}

EXPORT float *zg_skinnedmesh_vertex_weights_get(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index) {
	return mesh->vertices[index].Weights;
}

EXPORT uint32_t zg_skinnedmesh_submesh_count(ZenLoad::PackedSkeletalMesh *mesh) {
	return static_cast<uint32_t>(mesh->subMeshes.size());
}

EXPORT const char *zg_skinnedmesh_submesh_material_texture(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index) {
	return mesh->subMeshes[index].material.texture.c_str();
}

EXPORT const char *zg_skinnedmesh_submesh_material_name(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index) {
	return mesh->subMeshes[index].material.matName.c_str();
}

EXPORT uint32_t zg_skinnedmesh_submesh_material_color(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index) {
	return mesh->subMeshes[index].material.color;
}

EXPORT uint32_t zg_skinnedmesh_submesh_element_count(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index) {
	return static_cast<uint32_t>(mesh->subMeshes[index].indexSize);
}

EXPORT uint32_t zg_skinnedmesh_submesh_element_get(ZenLoad::PackedSkeletalMesh *mesh, uint32_t index, uint32_t index2) {
	return mesh->indices[mesh->subMeshes[index].indexOffset + index2];
}
