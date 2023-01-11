#ifndef _model_h_
#define _model_h_

#include <mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

class Model
{
public:
	Model(std::string const& path);
	Model(std::string const& path, glm::vec3& translate, glm::vec3& scale);

	void Draw(Shader& shader);

	int getMeshNum();

	glm::mat4 getModelMatrix();

	glm::vec3 GetPosition();

	glm::vec3 GetScale();

protected:
	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

	glm::vec3 position;
	glm::vec3 scale;

private:
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(std::string const& path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
};

#endif // !_model_h_
