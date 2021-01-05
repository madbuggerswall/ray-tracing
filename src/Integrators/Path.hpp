#ifndef PATH_HPP
#define PATH_HPP

#include <vector>

#include "../Core/SurfaceInteraction.hpp"
#include "../Geometry/Point3.hpp"
#include "../Geometry/Vector3.hpp"

class Vertex {
 public:
  Point3 point;
  Vector3 normal;
  Color color;
  std::shared_ptr<Material> materialPtr;
  bool hitLight = false;

  Vertex(const Point3& point, const Vector3& normal, const bool hitLight) :
      point(point),
      normal(normal),
      color(0, 0, 0),
      hitLight(hitLight) {}

  Vertex(const Point3& point, const Vector3& normal, const Color& color, const bool hitLight) :
      point(point),
      normal(normal),
      color(color),
      hitLight(hitLight) {}

  Vertex(const Point3& point, const Vector3& normal, const Color& color, const bool hitLight,
         const std::shared_ptr<Material> materialPtr) :
      point(point),
      normal(normal),
      color(color),
      hitLight(hitLight),
      materialPtr(materialPtr) {}

  Vertex(const SInteraction& interaction, const Color color, const bool hitLight) :
      Vertex(interaction.point, interaction.normal, color, hitLight, interaction.materialPtr) {}
};

class Path {
 private:
  std::vector<Vertex> vertices;

 public:
  Color contribution;
  ushort pixelX, pixelY;

  Path() {}
  void add(const Vertex& vertex) { vertices.push_back(vertex); }

  Vertex& operator[](int index) { return vertices[index]; }
  Vertex operator[](int index) const { return vertices[index]; }

  Vertex last() const { return vertices.back(); }
  Vertex first() const { return vertices.front(); }
  int length() const { return vertices.size(); }
  bool empty() const { return vertices.empty(); }
  void removeLast() { vertices.pop_back(); }
  void append(const Path& path) { vertices.insert(vertices.end(), path.vertices.begin(), path.vertices.end()); }
  void reverse() { std::reverse(vertices.begin(), vertices.end()); }
};

#endif