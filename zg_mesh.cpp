#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCProgMeshProto.h>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT ZenLoad::PackedMesh *zg_mesh_init(VDFS::FileIndex *vdfs, const char *name) {
	ZenLoad::zCProgMeshProto proto(name, *vdfs);
	auto result = new ZenLoad::PackedMesh();
	proto.packMesh(*result);
	return result;
}

EXPORT void zg_mesh_deinit(ZenLoad::PackedMesh *mesh) {
	delete mesh;
}

EXPORT uint32_t zg_mesh_vertex_count(ZenLoad::PackedMesh *mesh) {
	return static_cast<uint32_t>(mesh->vertices.size());
}

EXPORT float *zg_mesh_vertex_position_get(ZenLoad::PackedMesh *mesh, uint32_t index) {
	return mesh->vertices[index].Position.v;
}

EXPORT float *zg_mesh_vertex_normal_get(ZenLoad::PackedMesh *mesh, uint32_t index) {
	return mesh->vertices[index].Normal.v;
}

EXPORT float *zg_mesh_vertex_texcoord_get(ZenLoad::PackedMesh *mesh, uint32_t index) {
	return mesh->vertices[index].TexCoord.v;
}

EXPORT uint32_t zg_mesh_submesh_count(ZenLoad::PackedMesh *mesh) {
	return static_cast<uint32_t>(mesh->subMeshes.size());
}

EXPORT const char *zg_mesh_submesh_material_texture(ZenLoad::PackedMesh *mesh, uint32_t index) {
	return mesh->subMeshes[index].material.texture.c_str();
}

EXPORT const char *zg_mesh_submesh_material_name(ZenLoad::PackedMesh *mesh, uint32_t index) {
	return mesh->subMeshes[index].material.matName.c_str();
}

EXPORT uint32_t zg_mesh_submesh_material_color(ZenLoad::PackedMesh *mesh, uint32_t index) {
	return mesh->subMeshes[index].material.color;
}

EXPORT uint32_t zg_mesh_submesh_element_count(ZenLoad::PackedMesh *mesh, uint32_t index) {
	return static_cast<uint32_t>(mesh->subMeshes[index].indexSize);
}

EXPORT uint32_t zg_mesh_submesh_element_get(ZenLoad::PackedMesh *mesh, uint32_t submeshindex, uint32_t elementindex) {
	return mesh->indices[mesh->subMeshes[submeshindex].indexOffset + elementindex];
}

EXPORT uint32_t zg_mesh_vertexindex_get(ZenLoad::PackedMesh *mesh, uint32_t index) {
	return mesh->verticesId[index];
}
