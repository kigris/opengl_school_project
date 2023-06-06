#ifndef __MESH_H__
#define __MESH_H__ 1

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader;

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
};

struct TextureA {
  uint32_t id;
  std::string type;
  std::string path;
};

class Mesh {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<TextureA> textures);

  void render(const Shader& shader) const;

  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
  std::vector<TextureA> textures_;
  uint32_t VAO_;
private:
  void setupMesh();

  uint32_t VBO_, EBO_;
};

#endif