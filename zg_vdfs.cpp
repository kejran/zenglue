#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCMesh.h>
#include <zenload/zCProgMeshProto.h>
#include <zenload/zCModelMeshLib.h>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT VDFS::FileIndex *zg_vdfs_init(char *argv0) {
	VDFS::FileIndex::initVDFS(argv0);
	return new VDFS::FileIndex();
}

static std::vector<std::string> filenames;
// todo: create a custom caching wrapper 

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

EXPORT uint32_t zg_vdfs_file_exists(VDFS::FileIndex *vdfs, const char *name) {
	return vdfs->hasFile(name) ? 1 : 0;
}

EXPORT const char *zg_vdfs_file_name_get(VDFS::FileIndex *vdfs, uint32_t index) {
	return filenames[index].c_str();
}
