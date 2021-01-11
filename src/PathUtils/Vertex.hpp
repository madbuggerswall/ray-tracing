#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "../Core/SurfaceInteraction.hpp"
#include "../Math/Point3.hpp"

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

  // Copy constructor
  Vertex(const Vertex& other) :
      point(other.point),
      normal(other.normal),
      interaction(other.interaction),
      materialPtr(other.materialPtr) {}
  // Move constructor
  Vertex(Vertex&& other) noexcept :
      point(std::move(other.point)),
      normal(std::move(other.normal)),
      interaction(std::move(other.interaction)),
      materialPtr(std::move(other.materialPtr)) {}
  // Copy assignment
  Vertex& operator=(const Vertex& other) {
    point = other.point;
    normal = other.normal;
    interaction = other.interaction;
    materialPtr = other.materialPtr;
    return *this;
  }
  // Move assignment
  Vertex& operator=(Vertex&& other) noexcept {
    point = std::move(other.point);
    normal = std::move(other.normal);
    interaction = std::move(other.interaction);
    materialPtr = std::move(other.materialPtr);
    return *this;
  }

  Vertex(const SInteraction& interaction) : Vertex(interaction.point, interaction.normal, interaction.materialPtr) {
    this->interaction = interaction;
  }
};

#endif
