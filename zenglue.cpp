#include <cstdint>

#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCMesh.h>
#include <zenload/zCProgMeshProto.h>
#include <zenload/zCModelMeshLib.h>

#include <zenload/ztex.h>
#include <zenload/ztex2dds.h>
#include <lib/libsquish/squish.h>

#define EXPORT extern "C" __declspec(dllexport)

struct ZenWrapper {
	ZenLoad::ZenParser parser;
	std::unique_ptr<ZenLoad::oCWorldData> _data;
	bool g2;

	ZenWrapper(VDFS::FileIndex *vdfs, const char *name, bool g2_) : parser(name, *vdfs), g2(g2_) {}
	ZenLoad::oCWorldData *data() {
		if (!_data) {
			_data.reset(new ZenLoad::oCWorldData());
			parser.readWorld(*_data, g2);
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

EXPORT ZenWrapper *zg_zen_init(VDFS::FileIndex *vdfs, const char *name, bool g2_world) {
	try {
		auto w = new ZenWrapper(vdfs, name, g2_world);
		w->parser.readHeader();
		return w;
	}
	catch (const std::exception &) {
		return nullptr;
	}
}

EXPORT void zg_zen_deinit(ZenWrapper *zen) {
	delete zen;
}

EXPORT ZenLoad::PackedMesh *zg_zen_mesh(ZenWrapper *zen) {
	return zen->mesh();
}

EXPORT ZenLoad::oCWorldData *zg_zen_data(ZenWrapper *zen) {
	return zen->data();
}

EXPORT uint8_t *zg_tex_init(VDFS::FileIndex *archive, const char *name) {
	std::vector<uint8_t> data;
	archive->getFileData(name, data);
	std::vector<uint8_t> ddsData;
	int width, height;
	int r = ZenLoad::convertZTEX2DDS(data, ddsData, true, &width, &height);
	if (r != 0) return nullptr;
	auto result = new unsigned char[ddsData.size()];
	memcpy(result, &ddsData[0], ddsData.size());
	return result;
}

EXPORT uint32_t zg_tex_fourcc(uint8_t *data) {
	return *(uint32_t *)data;
}

EXPORT unsigned char *zg_tex_header(uint8_t *data) {
	return data + 4;
}

EXPORT uint8_t *zg_tex_payload(uint8_t *data) {
	return data + 4 + sizeof(ZenLoad::DDSURFACEDESC2);
}

EXPORT void zg_tex_deinit(uint8_t *array) {
	delete[] array;
}

EXPORT uint8_t *zg_tex_dds3_decompress(uint8_t *ddsData) {
	size_t seek = 0;
	seek += sizeof(uint32_t);  // Skip magic number
	const auto *desc = reinterpret_cast<const ZenLoad::DDSURFACEDESC2 *>(&ddsData[seek]);
	seek += sizeof(ZenLoad::DDSURFACEDESC2);

	int squishDxtLevel = squish::kDxt3;
	std::size_t px = desc->dwWidth;
	std::size_t py = desc->dwHeight;
	auto result = new uint8_t[px * py * sizeof(uint32_t)];

	squish::DecompressImage(result, px, py, ddsData + seek, squishDxtLevel);
	
	return result;
}