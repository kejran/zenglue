#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCProgMeshProto.h>
#include <zenload/zCModelMeshLib.h>
#include <zenload/modelScriptParser.h>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT ZenLoad::zCModelMeshLib *zg_meshlib_init(VDFS::FileIndex *vdfs, const char *name) {
	return new ZenLoad::zCModelMeshLib(name, *vdfs);
}

//EXPORT ZenLoad::zCModelMeshLib *zg_meshlib_init_ex(VDFS::FileIndex *vdfs, const char *name) {
//	std::string msfile(name);
//
//	// try to find msb
//	auto dot = msfile.find_last_of(".");
//	auto base = msfile.substr(0, dot);
//	auto new_name = base + ".MSB";
//	if (vdfs->hasFile(new_name))
//		msfile = new_name;
//
//	// load hierarchy
//	auto hierarchy = base + ".MDH";
//	ZenLoad::zCModelMeshLib h_meshlib(hierarchy.c_str(), *vdfs);
//	if (!h_meshlib.isValid())
//		return nullptr;
//
//	// todo make skeleton...
//
//	// make parser
//	ZenLoad::ModelScriptParser *parser = nullptr;
//	auto ext = msfile.substr(msfile.size() - 3);
//	if (ext == "MSB")
//
//	ZenLoad::ZenParser zen(msfile.c_str(), *vdfs);
//
//
//	return new ZenLoad::zCModelMeshLib(name, *vdfs);
//}

EXPORT void zg_meshlib_deinit(ZenLoad::zCModelMeshLib *lib) {
	delete lib;
}

EXPORT uint32_t zg_meshlib_attached_count(ZenLoad::zCModelMeshLib *lib) {
	return static_cast<uint32_t>(lib->getAttachments().size());
}

EXPORT const char *zg_meshlib_attached_name_get(ZenLoad::zCModelMeshLib *lib, uint32_t index) {
	return lib->getAttachments()[index].first.c_str();
}

EXPORT ZenLoad::PackedMesh *zg_meshlib_attached_mesh_get(ZenLoad::zCModelMeshLib *lib, uint32_t index) {
	auto mesh = lib->getAttachments()[index].second;
	auto *result = new ZenLoad::PackedMesh();
	mesh.packMesh(*result);
	return result;
}

EXPORT uint32_t zg_meshlib_node_count(ZenLoad::zCModelMeshLib *lib) {
	return static_cast<uint32_t>(lib->getNodes().size());
}

EXPORT const char *zg_meshlib_node_name_get(ZenLoad::zCModelMeshLib *lib, uint32_t index) {
	return lib->getNodes()[index].name.c_str();
}

EXPORT uint32_t zg_meshlib_node_parent_get(ZenLoad::zCModelMeshLib *lib, uint32_t index) {
	return lib->getNodes()[index].parentIndex;
}

EXPORT float *zg_meshlib_node_transform_get(ZenLoad::zCModelMeshLib *lib, uint32_t index) {
	return const_cast<float *>(lib->getNodes()[index].transformLocal.mv);
}

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
	return static_cast<uint32_t>(mesh->subMeshes[index].indices.size());
}

EXPORT uint32_t zg_mesh_submesh_element_get(ZenLoad::PackedMesh *mesh, uint32_t index, uint32_t index2) {
	return mesh->subMeshes[index].indices[index2];
}