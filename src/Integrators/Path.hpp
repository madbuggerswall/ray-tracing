#ifndef PATH_HPP
#define PATH_HPP

#include <vector>

#include "../Core/Image.hpp"
#include "../Core/SurfaceInteraction.hpp"
#include "../Geometry/Point3.hpp"
#include "../Geometry/Vector3.hpp"

class Vertex {
 public:
  Point3 point;
  Vector3 normal;
  SInteraction interaction;
  std::shared_ptr<Material> materialPtr;

  Vertex(const Point3& point, const Vector3& normal) : point(point), normal(normal) {}
  Vertex(const Point3& point, const Vector3& normal, const std::shared_ptr<Material> materialPtr) :
      point(point),
      normal(normal),
      materialPtr(materialPtr) {}

  Vertex(const SInteraction& interaction) : Vertex(interaction.point, interaction.normal, interaction.materialPtr) {
    this->interaction = interaction;
  }
};

class Path {
 private:
  std::vector<Vertex> vertices;

 public:
  Path() { vertices.reserve(32); }
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
  Path reverse() const {
    Path reversed;
    reversed.vertices = vertices;
    std::reverse(reversed.vertices.begin(), reversed.vertices.end());
    return reversed;
  }
};

struct Contribution {
  Color color;
  int x, y;

  Contribution(const Color& color, const int x, const int y) : color(color), x(x), y(y) {}
};

class PathContribution {
  std::vector<Contribution> contributions;

 public:
  double scalarContrib;
  PathContribution() { contributions.reserve(16); }
  Contribution& operator[](int index) { return contributions[index]; }
  Contribution operator[](int index) const { return contributions[index]; }

  void add(const Contribution& contrib) { contributions.push_back(contrib); }

  Color accumulatePathContribution(const double scale) {
    if (scalarContrib == 0) return Color(0, 0, 0);
    Color result;
    for (auto contribution : contributions) {
      // const int ix = int(contributions[i].x);
      // const int iy = int(contributions[i].y);
      const Color color = contribution.color * scale;
      result += color;
    }
    return result;
  }
};

struct KahanAdder {
  double sum, carry, y;
  KahanAdder(const double b = 0.0) {
    sum = b;
    carry = 0.0;
    y = 0.0;
  }
  inline void add(const double b) {
    y = b - carry;
    const double t = sum + y;
    carry = (t - sum) - y;
    sum = t;
  }
};
#endif