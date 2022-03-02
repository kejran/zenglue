#include <cstdint>

#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>
#include <zenload/zCMesh.h>
#include <zenload/zCProgMeshProto.h>
#include <zenload/zCModelMeshLib.h>
#include <zenload/zTypes.h>

#define EXPORT extern "C" __declspec(dllexport)

// adapted from https://github.com/Try/ZenLib/commit/0b1117b3a76a5e58ba9d7a0c4bcd07ae680ce080

void packMesh__(ZenLoad::zCMesh const &source, ZenLoad::PackedMesh &mesh, float scale, bool removeDoubles)
{

    auto &sIndices = source.getIndices();
    auto &sFIndices = source.getFeatureIndices();
    auto &sTLIndices = source.getTriangleLightmapIndices();
    auto &sVertices = source.getVertices();
    auto &sFeatures = source.getFeatures();
    auto &sMaterials = source.getMaterials();
    auto &sTMIndices = source.getTriangleMaterialIndices();

    std::vector<ZenLoad::WorldVertex> &newVertices = mesh.vertices;
    std::vector<uint32_t> newIndices;
    newIndices.reserve(sIndices.size());


    // Map of vertex-indices and their used feature-indices to a vertex in "newVertices"
    std::map<std::tuple<uint32_t, uint32_t, int16_t>, size_t> vfToNewVx;

    // Map of the new indices and the old indices to the index-vector
    //std::unordered_map<uint32_t, uint32_t> newToOldIdxIdx;


    if (removeDoubles)
    {
        // Get vertices
        for (size_t i = 0, end = sIndices.size(); i < end; i++)
        {
            uint32_t featidx = sFIndices[i];
            uint32_t vertidx = sIndices[i];
            int16_t lightmap = sTLIndices[i / 3];

            // Check if we already got this pair of vertex/feature
            auto it = vfToNewVx.find(std::make_tuple(vertidx, featidx, lightmap));
            if (it == vfToNewVx.end())
            {
                // Add new entry
                vfToNewVx[std::make_tuple(vertidx, featidx, lightmap)] = newVertices.size();
                ZenLoad::WorldVertex vx;

                // Extract vertex information
                vx.Position = sVertices[vertidx] * scale;
                vx.Color = sFeatures[featidx].lightStat;
                vx.TexCoord = ZMath::float2(sFeatures[featidx].uv[0], sFeatures[featidx].uv[1]);
                vx.Normal = sFeatures[featidx].vertNormal;

                // Add index to this very vertex
                newIndices.push_back(uint32_t(newVertices.size()));
                newVertices.push_back(vx);
            }
            else
            {
                // Simply put an index to the existing new vertex
                newIndices.push_back(uint32_t((*it).second));
            }

            // Store what this new index was before
            //newToOldIdxIdx[newIndices.back()] = i;
        }
    }
    else
    {
        // Just add them as triangles
        newVertices.reserve(sIndices.size());
        for (size_t i = 0, end = sIndices.size(); i < end; i++)
        {
            uint32_t featidx = sFIndices[i];
            uint32_t vertidx = sIndices[i];
            int16_t lightmap = sTLIndices[i / 3];

            ZenLoad::WorldVertex vx;

            // Extract vertex information
            vx.Position = sVertices[vertidx] * scale;
            vx.Color = sFeatures[featidx].lightStat;
            vx.TexCoord = ZMath::float2(sFeatures[featidx].uv[0], sFeatures[featidx].uv[1]);
            vx.Normal = sFeatures[featidx].vertNormal;

            newVertices.push_back(vx);
            newIndices.push_back(uint32_t(newVertices.size() - 1));
        }
    }

    // Filter textures
    std::unordered_map<std::string, uint32_t> materialsByTexture;
    std::unordered_map<uint32_t, uint32_t> newMaterialSlotsByMatIndex;
    for (size_t i = 0, end = sMaterials.size(); i < end; i++)
    {
        materialsByTexture[sMaterials[i].texture] = uint32_t(i);
    }

    // Assign materials to packed mesh
    uint32_t mc = 0;
    for (auto &m : materialsByTexture)
    {
        newMaterialSlotsByMatIndex[m.second] = mc++;
        mesh.subMeshes.emplace_back();
        mesh.subMeshes.back().material = sMaterials[m.second];
    }

    std::vector<std::vector<uint32_t>> tempIndices(mc);

    mesh.subMeshes.resize(materialsByTexture.size());
    // Add triangles
    for (size_t i = 0, end = newIndices.size(); i < end; i += 3)
    {
        // Get material info of this triangle
        uint32_t matIdx = sTMIndices[i / 3];

        // Find texture of this material
        const auto &matByTex = materialsByTexture.find(sMaterials[matIdx].texture);
        if (matByTex != materialsByTexture.end())
        {
            const auto &matSlotByIndex = newMaterialSlotsByMatIndex.find(matByTex->second);
            if (matSlotByIndex != newMaterialSlotsByMatIndex.end())
            {
                // Copy lightmap index
                mesh.subMeshes[matSlotByIndex->second].triangleLightmapIndices.push_back(sTLIndices[i / 3]);

                // Add this triangle to its submesh
                for (size_t j = 0; j < 3; j++)
                    tempIndices[matSlotByIndex->second].push_back(newIndices[i + j]);
                    //mesh.subMeshes[matSlotByIndex->second].indices.push_back(newIndices[i + j]);
            }
        }
    }

    for (int i = 0; i < mc; ++i) {
        mesh.subMeshes[i].indexOffset = mesh.indices.size();
        mesh.subMeshes[i].indexSize = tempIndices[i].size();
        mesh.indices.insert(mesh.indices.end(), tempIndices[i].begin(), tempIndices[i].end());
    }
}

struct WorldWrapper {
    ZenLoad::ZenParser parser;
    std::unique_ptr<ZenLoad::oCWorldData> _data;
    bool g2;

    WorldWrapper(VDFS::FileIndex *vdfs, const char *name, bool g2_) : parser(name, *vdfs), g2(g2_) {}
    ZenLoad::oCWorldData *data() {
        if (!_data) {
            _data.reset(new ZenLoad::oCWorldData());
            parser.readWorld(*_data, g2 ?
                ZenLoad::ZenParser::FileVersion::Gothic2 :
                ZenLoad::ZenParser::FileVersion::Gothic1);
        }
        return _data.get();
    }
    ZenLoad::PackedMesh *mesh() {
        data();
        auto zmesh = parser.getWorldMesh();
        auto result = new ZenLoad::PackedMesh();
        packMesh__(*zmesh, *result, 1, true);
        return result;
    }
};

EXPORT WorldWrapper *zg_zen_init(VDFS::FileIndex *vdfs, const char *name, bool g2_world) {
    try {
        auto w = new WorldWrapper(vdfs, name, g2_world);
        w->parser.readHeader();
        return w;
    }
    catch (const std::exception &) {
        return nullptr;
    }
}

EXPORT void zg_zen_deinit(WorldWrapper *zen) {
    delete zen;
}

EXPORT ZenLoad::PackedMesh *zg_zen_mesh(WorldWrapper *zen) {
    return zen->mesh();
}

EXPORT ZenLoad::oCWorldData *zg_zen_data(WorldWrapper *zen) {
    return zen->data();
}

EXPORT uint32_t zg_zen_waynet_node_count(ZenLoad::oCWorldData *data) {
    return static_cast<uint32_t>(data->waynet.waypoints.size());
}

EXPORT const char *zg_zen_waynet_node_name_get(ZenLoad::oCWorldData *data, uint32_t index) {
    return data->waynet.waypoints[index].wpName.c_str();
}

EXPORT float *zg_zen_waynet_node_position_get(ZenLoad::oCWorldData *data, uint32_t index) {
    return data->waynet.waypoints[index].position.v;
}

EXPORT float *zg_zen_waynet_node_direction_get(ZenLoad::oCWorldData *data, uint32_t index) {
    return data->waynet.waypoints[index].direction.v;
}

EXPORT uint32_t zg_zen_waynet_edge_count(ZenLoad::oCWorldData *data) {
    return static_cast<uint32_t>(data->waynet.edges.size());
}
    
EXPORT uint32_t zg_zen_waynet_edge_1_get(ZenLoad::oCWorldData *data, uint32_t index) {
    return static_cast<uint32_t>(data->waynet.edges[index].first);
}

EXPORT uint32_t zg_zen_waynet_edge_2_get(ZenLoad::oCWorldData *data, uint32_t index) {
    return static_cast<uint32_t>(data->waynet.edges[index].second);
}
