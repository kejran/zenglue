#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCMesh.h>
#include <zenload/zCProgMeshProto.h>

#define EXPORT extern "C" __declspec(dllexport)
//#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

EXPORT VDFS::FileIndex* zg_vdfs_init(char *argv0) {
	VDFS::FileIndex::initVDFS(argv0);
	return new VDFS::FileIndex();
}

static std::vector<std::string> filenames;

EXPORT uint32_t zg_vdfs_load(VDFS::FileIndex *vdfs, const char *archive_path) {
	return vdfs->loadVDF(archive_path) ? 1 : 0;
}

EXPORT void zg_vdfs_finalize(VDFS::FileIndex *vdfs) {
	vdfs->finalizeLoad();
	filenames = vdfs->getKnownFiles();
}

EXPORT void zg_vdfs_deinit(VDFS::FileIndex *vdfs) {
	delete vdfs;
	filenames.clear();
}

EXPORT uint32_t zg_vdfs_file_count(VDFS::FileIndex *vdfs) {
	return static_cast<uint32_t>(filenames.size());
}

EXPORT uint32_t zg_vdfs_file_exists(VDFS::FileIndex* vdfs, const char *name) {
	return vdfs->hasFile(name) ? 1 : 0;
}

EXPORT const char *zg_vdfs_file_name_get(VDFS::FileIndex *vdfs, uint32_t index) {
	return filenames[index].c_str();
}

struct ZenWrapper {
	ZenLoad::ZenParser parser;
	std::unique_ptr<ZenLoad::oCWorldData> _data;

	ZenWrapper(VDFS::FileIndex *vdfs, const char *name) : parser(name, *vdfs) {}
	ZenLoad::oCWorldData *data() {
		if (!_data) {
			_data.reset(new ZenLoad::oCWorldData());
			parser.readWorld(*_data, false);
		}
		return _data.get(); 
	}
	ZenLoad::PackedMesh *mesh() { 
		data();
		auto zmesh = parser.getWorldMesh();
		auto result = new ZenLoad::PackedMesh();
		zmesh->packMesh(*result);
		return result;
	}
};

EXPORT ZenWrapper *zg_zen_init(VDFS::FileIndex *vdfs, const char *name) {
	try {
		auto w = new ZenWrapper(vdfs, name);
		w->parser.readHeader();
		return w;
	}
	catch (const std::exception &e) {
		return nullptr;
	}
}

EXPORT void zg_zen_deinit(ZenWrapper *zen) {
	delete zen;
}

EXPORT ZenLoad::PackedMesh *zg_zen_mesh(ZenWrapper* zen) {
	return zen->mesh();
}

EXPORT ZenLoad::oCWorldData *zg_zen_data(ZenWrapper* zen) {
	return zen->data();
}

EXPORT ZenLoad::PackedMesh* zg_mesh_init(VDFS::FileIndex* vdfs, const char* name) {
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

EXPORT const char* zg_mesh_submesh_material_name(ZenLoad::PackedMesh* mesh, uint32_t index) {
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

using VOB = ZenLoad::zCVobData;
using VOBArray = std::vector<VOB>;

EXPORT VOBArray *zg_zen_vobs(ZenLoad::oCWorldData *data) {
	return &data->rootVobs;
}

EXPORT uint32_t zg_vobs_count(VOBArray *data) {
	return static_cast<uint32_t>(data->size());
}

EXPORT VOB *zg_vobs_get(VOBArray *data, uint32_t index) {
	return &data->at(index);
}

EXPORT VOBArray *zg_vob_children(VOB *vob) {
	return &vob->childVobs;
}

EXPORT const char *zg_vob_visual(VOB *vob) {
	return vob->visual.c_str();
}

EXPORT const char *zg_vob_name(VOB *vob) {
	return vob->vobName.c_str();
}

EXPORT uint32_t zg_vob_type(VOB *vob) {
	return static_cast<uint32_t>(vob->vobType);
}

EXPORT float *zg_vob_position(VOB *vob) {
	return vob->position.v;
}

EXPORT float* zg_vob_rotation(VOB* vob) {
	return vob->rotationMatrix3x3.v[0];
}

EXPORT uint32_t zg_vob_light_color(VOB *vob) {
	return vob->zCVobLight.color;
}
