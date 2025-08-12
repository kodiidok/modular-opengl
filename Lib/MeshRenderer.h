#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Mesh.h"

class MeshRenderer {
public:
    MeshRenderer();
    void renderWireframe(const Mesh& mesh);
    void renderSolid(const Mesh& mesh);
};

#endif
#pragma once
