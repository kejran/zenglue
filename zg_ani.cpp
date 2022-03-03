#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCModelAni.h>
#include <zenload/modelAnimationParser.h>

#define EXPORT extern "C" __declspec(dllexport)

using Parser = ZenLoad::ModelAnimationParser;

EXPORT Parser *zg_ani_init(VDFS::FileIndex *vdfs, const char *name) {
	ZenLoad::ZenParser zen(name, *vdfs);
	auto parser = new Parser(zen);
	Parser::EChunkType type;
	while ((type = parser->parse()) != Parser::CHUNK_EOF) {}
	return parser;
}

EXPORT void zg_ani_deinit(Parser *parser) {
	delete parser;
}

EXPORT uint32_t zg_ani_node_count(Parser *parser) {
	return parser->getHeader().numNodes;
}

EXPORT uint32_t zg_ani_frame_count(Parser *parser) {
	return parser->getHeader().numFrames;
}

EXPORT float zg_ani_fps(Parser *parser) {
	return parser->getHeader().fpsRate;
}

EXPORT uint32_t zg_ani_nodeindex_get(Parser *parser, uint32_t index) {
	return parser->getNodeIndex()[index];
}

EXPORT float *zg_ani_sample_position_get(Parser *parser, uint32_t index) {
	return const_cast<float *>(parser->getSamples()[index].position.v);
}

EXPORT float *zg_ani_sample_rotation_get(Parser *parser, uint32_t index) {
	return const_cast<float *>(parser->getSamples()[index].rotation.v);
}

EXPORT const char *zg_ani_next(Parser *parser) {
	return parser->getHeader().nextAniName.c_str();
}

EXPORT uint32_t zg_ani_layer(Parser *parser) {
	return parser->getHeader().layer;
}