#ifndef PATH_HPP
#define PATH_HPP

#include <algorithm>
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
  Vertex() = default;
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
  Path() = delete;
  Path(uint bounceLimit) { vertices.reserve(bounceLimit * 2); }
  Path(const std::vector<Vertex>& vertices) : vertices(vertices) {}
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

struct Contribution {
  Color color;
  double x, y;

  Contribution() : color(), x(0), y(0) {}
  Contribution(const Color& color, const int x, const int y) : color(color), x(x), y(y) {}
};

class PathContribution {
  std::vector<Contribution> contributions;

 public:
  float scalarContrib;

  PathContribution() = delete;
  PathContribution(uint maxEvents) : scalarContrib(0) { contributions.reserve(maxEvents * maxEvents); }
  Contribution& operator[](int index) { return contributions[index]; }
  Contribution operator[](int index) const { return contributions[index]; }

  void add(const Contribution& contrib) { contributions.push_back(contrib); }

  Color accumulatePathContribution(const float scale) {
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

  void accumulatePathContribution(const float scale, Image& image) {
    int imageWidth = image.getWidth();
    if (scalarContrib == 0) return;
    for (auto contribution : contributions) {
      const int ix = int(contribution.x);
      const int iy = int(contribution.y);
      const Color color = contribution.color * scale;
      image[iy * imageWidth + ix] += color;
    }
  }
};

#endif