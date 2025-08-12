#include "pch.h"
#include "MeshRenderer.h"
#include <glut.h>

MeshRenderer::MeshRenderer() {}

void MeshRenderer::renderWireframe(const Mesh& mesh) {
    glBegin(GL_LINES);
    for (auto& edge : mesh.getEdges()) {
        auto& v1 = mesh.getVertices()[edge.v1];
        auto& v2 = mesh.getVertices()[edge.v2];
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
    }
    glEnd();
}

void MeshRenderer::renderSolid(const Mesh& mesh) {
    const auto& vertices = mesh.getVertices();
    const auto& faces = mesh.getFaces();
    const auto& vertexNormals = mesh.getVertexNormals();

    for (size_t f = 0; f < faces.size(); ++f) {
        glBegin(GL_POLYGON);
        for (int idx : faces[f].vertices) {
            const auto& normal = vertexNormals[idx];
            glNormal3f(normal.x, normal.y, normal.z);

            const auto& v = vertices[idx];
            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
    }
}

