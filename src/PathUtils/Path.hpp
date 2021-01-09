#ifndef PATH_HPP
#define PATH_HPP

#include <algorithm>
#include <vector>

#include "../Core/Image.hpp"
#include "../Core/SurfaceInteraction.hpp"
#include "../Geometry/Point3.hpp"
#include "../Geometry/Vector3.hpp"
#include "Vertex.hpp"

class Path {
 private:
  std::vector<Vertex> vertices;

 public:
  Path() = delete;
  Path(uint bounceLimit) { vertices.reserve(bounceLimit * 2); }
  Path(const std::vector<Vertex>& vertices) : vertices(vertices) {}

  // Copy constructor
  Path(const Path& other) : vertices(other.vertices) {}
  // Move constructor
  Path(Path&& other) noexcept : vertices(std::move(other.vertices)) {}
  // Copy assignement
  Path& operator=(const Path& other) {
    vertices = other.vertices;
    return *this;
  }
  // Move assignement
  Path& operator=(Path&& other) noexcept {
    vertices = std::move(other.vertices);
    return *this;
  }

  void add(const Vertex& vertex) { vertices.push_back(vertex); }

  Vertex& operator[](int index) { return vertices[index]; }
  Vertex operator[](int index) const { return vertices[index]; }

  Vertex last() const { return vertices.back(); }
  Vertex first() const { return vertices.front(); }
  std::vector<Vertex> firstN(int vertexCount) const {
    std::vector<Vertex> result;
    std::copy_n(vertices.begin(), vertexCount, std::back_inserter(result));
    return result;
  }

  int length() const { return vertices.size(); }
  bool empty() const { return vertices.empty(); }
  void removeLast() { vertices.pop_back(); }
  void append(const Path& path) { vertices.insert(vertices.end(), path.vertices.begin(), path.vertices.end()); }
  void reverse() { std::reverse(vertices.begin(), vertices.end()); }
  Path reversed() const {
    std::vector<Vertex> reversed = vertices;
    std::reverse(reversed.begin(), reversed.end());
    return Path(reversed);
  }
};

#endif