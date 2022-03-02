#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/modelScriptParser.h>

#define EXPORT extern "C" __declspec(dllexport)

struct anidata {
	uint32_t layer;
	float blendIn = 0;
	float blendOut = 0;
	uint32_t flags = 0;
	uint32_t dirReverse;
	int32_t firstFrame;
	int32_t lastFrame;
	float maxFps;
	float speed;
	float colVolScale;
};

struct ani {
	anidata data;
	std::string name;
	std::string asc;
	std::string next;
};

struct script {
	ZenLoad::MdsParser *parser;
	std::vector<ani> anis;
};

EXPORT script *zg_script_init(VDFS::FileIndex *vdfs, const char *name) {
	ZenLoad::ZenParser parser(name, *vdfs);
	auto result = new script();
	result->parser = nullptr;
	auto len = strlen(name);
	if (0 == strncmp(name + len - 4, ".MDS", 4))
		result->parser = new ZenLoad::MdsParserTxt(parser);
	if (0 == strncmp(name + len - 4, ".MDH", 4))
		result->parser = new ZenLoad::MdsParserBin(parser);
	if (result->parser == nullptr) 
		return result;
	ZenLoad::MdsParser::Chunk type;
	while (type = result->parser->parse(), type != ZenLoad::MdsParser::CHUNK_EOF) {
		switch (type) {
		case ZenLoad::MdsParser::CHUNK_ANI: {
			result->anis.emplace_back();
			auto &b = result->anis.back();
			auto &d = b.data;
			auto &a = result->parser->ani;
			d.layer = a.m_Layer;
			d.blendIn = a.m_BlendIn;
			d.blendOut = a.m_BlendOut;
			d.flags = a.m_Flags;
			d.dirReverse = a.m_Dir == ZenLoad::EModelScriptAniDir::MSB_FORWARD ? 0 : 1;
			d.firstFrame = a.m_FirstFrame;
			d.lastFrame = a.m_LastFrame;
			d.maxFps = a.m_MaxFps;
			d.speed = a.m_Speed;
			d.colVolScale - a.m_ColVolScale;
			b.asc = a.m_Asc;
			b.name = a.m_Name;
			b.next = a.m_Next;
		} break;
		default: break;
		}
	}

	return result;
}

EXPORT void zg_script_deinit(script *mds) {
	if (mds->parser)
		delete mds->parser;
	delete mds;
}

EXPORT uint32_t zg_script_registeredmesh_count(script *mds) {
	return mds->parser->meshesASC.size();
}

EXPORT const char *zg_script_registeredmesh_get(script *mds, uint32_t index) {
	return mds->parser->meshesASC[index].c_str();
}

EXPORT const char *zg_script_meshtree(script *mds) {
	return mds->parser->meshAndThree.m_Name.c_str();
}

EXPORT uint32_t zg_script_ani_count(script *mds) {
	return static_cast<uint32_t>(mds->anis.size());
}

EXPORT const char *zg_script_ani_name_get(script *mds, uint32_t index) {
	return mds->anis[index].name.c_str();
}

EXPORT const char *zg_script_ani_nexr_get(script *mds, uint32_t index) {
	return mds->anis[index].next.c_str();
}

EXPORT const char *zg_script_ani_asc_get(script *mds, uint32_t index) {
	return mds->anis[index].asc.c_str();
}

EXPORT const anidata *zg_script_ani_data_get(script *mds, uint32_t index) {
	return &mds->anis[index].data;
}
