#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCMesh.h>
#include <zenload/zCProgMeshProto.h>

#define EXPORT extern "C" __declspec(dllexport)

using VOB = ZenLoad::zCVobData;
using VOBArray = std::vector<VOB>;

EXPORT VOBArray *zg_zen_data_vobs(ZenLoad::oCWorldData *data) {
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

EXPORT float *zg_vob_rotation(VOB *vob) {
	return vob->rotationMatrix3x3.v[0];
}

EXPORT float *zg_vob_transform(VOB *vob) {
	return vob->worldMatrix.mv;
}

EXPORT uint32_t zg_vob_show(VOB *vob) {
	return vob->showVisual ? 1 : 0;
}

EXPORT uint32_t zg_vob_light_color(VOB *vob) {
	return vob->zCVobLight.color;
}

EXPORT float zg_vob_light_range(VOB *vob) {
	return vob->zCVobLight.range;
}

EXPORT const char* zg_vob_inter_item(VOB* vob) {
	return vob->oCMobInter.useWithItem.c_str();
}

EXPORT const char *zg_vob_item_id(VOB *vob) {
	return vob->oCItem.instanceName.c_str();
}

EXPORT const char* zg_vob_container_contents(VOB *vob) {
	return vob->oCMobContainer.contains.c_str();
}

EXPORT uint32_t zg_vob_lock_locked(VOB *vob) {
	return vob->oCMobLockable.locked ? 1 : 0;
}

EXPORT const char *zg_vob_lock_key(VOB *vob) {
	return vob->oCMobLockable.keyInstance.c_str();
}

EXPORT const char* zg_vob_lock_code(VOB *vob) {
	return vob->oCMobLockable.pickLockStr.c_str();
}
