#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

// Basic structures
struct Normal {
    float x, y, z;

    Normal() : x(0), y(0), z(0) {}
    Normal(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    // Normalize the normal vector
    void normalize() {
        float length = sqrt(x * x + y * y + z * z);
        if (length > 0.00001f) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    // Add another normal to this one (for averaging)
    Normal& operator+=(const Normal& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
};

struct Vertex {
    float x, y, z;
};

struct Edge {
    int v1, v2; // Indices into vertices vector
};

struct Face {
    std::vector<int> vertices; // indices into vertices vector
};

class Mesh {
public:
    Mesh();
    void addVertex(float x, float y, float z);
    void addEdge(int v1, int v2);
    void addFace(const std::vector<int>& vertexIndices);

    const std::vector<Vertex>& getVertices() const;
    const std::vector<Edge>& getEdges() const;
    const std::vector<Face>& getFaces() const;

    // Call to compute normals after building the mesh
    void computeNormals();

    // Getters for normals
    const std::vector<Normal>& getFaceNormals() const { return faceNormals; }
    const std::vector<Normal>& getVertexNormals() const { return vertexNormals; }

    // New static factory methods
    static Mesh createPlane(float size = 1.0f);
    static Mesh createCube(float size = 1.0f);

private:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;

    // Normals per face and per vertex
    std::vector<Normal> faceNormals;
    std::vector<Normal> vertexNormals;
};

#endif
#pragma once
