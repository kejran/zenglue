#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCModelMeshLib.h>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT ZenLoad::zCModelMeshLib *zg_meshlib_init(VDFS::FileIndex *vdfs, const char *name) {
	return new ZenLoad::zCModelMeshLib(name, *vdfs);
}

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

EXPORT void zg_meshlib_root_transform(ZenLoad::zCModelMeshLib* lib, float *dest) {
	*reinterpret_cast<ZMath::float3 *>(dest) = lib->getRootNodeTranslation();
}
