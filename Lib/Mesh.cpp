#include "pch.h"
#include "Mesh.h"

Mesh::Mesh() {}

void Mesh::addVertex(float x, float y, float z) {
    vertices.push_back({ x, y, z });
}

void Mesh::addEdge(int v1, int v2) {
    edges.push_back({ v1, v2 });
}

void Mesh::addFace(const std::vector<int>& vertexIndices) {
    faces.push_back({ vertexIndices });
}

const std::vector<Vertex>& Mesh::getVertices() const { return vertices; }
const std::vector<Edge>& Mesh::getEdges() const { return edges; }
const std::vector<Face>& Mesh::getFaces() const { return faces; }

Mesh Mesh::createPlane(float size) {
    Mesh mesh;
    float half = size / 2.0f;

    mesh.addVertex(-half, -half, 0.0f);
    mesh.addVertex(half, -half, 0.0f);
    mesh.addVertex(half, half, 0.0f);
    mesh.addVertex(-half, half, 0.0f);

    mesh.addEdge(0, 1);
    mesh.addEdge(1, 2);
    mesh.addEdge(2, 3);
    mesh.addEdge(3, 0);

    mesh.addFace({ 0, 1, 2, 3 });

    mesh.computeNormals();  // compute normals before returning

    return mesh;
}

Mesh Mesh::createCube(float size) {
    Mesh mesh;
    float half = size / 2.0f;

    mesh.addVertex(-half, -half, -half);
    mesh.addVertex(half, -half, -half);
    mesh.addVertex(half, half, -half);
    mesh.addVertex(-half, half, -half);
    mesh.addVertex(-half, -half, half);
    mesh.addVertex(half, -half, half);
    mesh.addVertex(half, half, half);
    mesh.addVertex(-half, half, half);

    mesh.addEdge(0, 1); mesh.addEdge(1, 2); mesh.addEdge(2, 3); mesh.addEdge(3, 0);
    mesh.addEdge(4, 5); mesh.addEdge(5, 6); mesh.addEdge(6, 7); mesh.addEdge(7, 4);
    mesh.addEdge(0, 4); mesh.addEdge(1, 5); mesh.addEdge(2, 6); mesh.addEdge(3, 7);

    mesh.addFace({ 0, 1, 2, 3 });
    mesh.addFace({ 4, 5, 6, 7 });
    mesh.addFace({ 0, 1, 5, 4 });
    mesh.addFace({ 1, 2, 6, 5 });
    mesh.addFace({ 2, 3, 7, 6 });
    mesh.addFace({ 3, 0, 4, 7 });

    mesh.computeNormals();  // compute normals before returning

    return mesh;
}

void Mesh::computeNormals() {
    faceNormals.clear();
    vertexNormals.clear();
    vertexNormals.resize(vertices.size(), Normal(0, 0, 0));

    // Compute face normals
    for (const auto& face : faces) {
        if (face.vertices.size() < 3) {
            faceNormals.push_back(Normal(0, 0, 0));
            continue;
        }

        // Use first 3 vertices to compute normal
        Vertex v0 = vertices[face.vertices[0]];
        Vertex v1 = vertices[face.vertices[1]];
        Vertex v2 = vertices[face.vertices[2]];

        // Compute vectors
        float ux = v1.x - v0.x;
        float uy = v1.y - v0.y;
        float uz = v1.z - v0.z;

        float vx = v2.x - v0.x;
        float vy = v2.y - v0.y;
        float vz = v2.z - v0.z;

        // Cross product u x v
        float nx = uy * vz - uz * vy;
        float ny = uz * vx - ux * vz;
        float nz = ux * vy - uy * vx;

        Normal normal(nx, ny, nz);
        normal.normalize();
        faceNormals.push_back(normal);

        // Add this face normal to each vertex normal of the face
        for (int idx : face.vertices) {
            vertexNormals[idx] += normal;
        }
    }

    // Normalize vertex normals
    for (auto& n : vertexNormals) {
        n.normalize();
    }
}