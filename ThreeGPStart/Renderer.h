#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

class Renderer
{
private:
	// Program object - to host shaders
	GLuint m_program{ 0 };
	GLuint SkyProgram{ 0 };
	GLuint CubeProgram{ 0 };
	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };
	GLuint SkyVAO{ 0 };
	GLuint JeepVAO{ 0 };
	GLuint CubeVAO{ 0 };
	// Number of elments to use when rendering
	GLuint m_numElements{ 0 };
	GLuint JeepNumElements{ 0 };
	GLuint CubeNumElements{ 0 };
	GLuint Terraintex;
	GLuint Jeeptex;
	GLuint SkyBoxTex;
	bool m_wireframe{ false };




	std::vector<glm::vec3> Corners;
	std::vector<glm::vec3> TerrainVerts;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec3> CubeNormals;
	std::vector<glm::vec2> UV;
	std::vector<GLint> elements;
	std::vector<GLint> Cube_Elements;


	bool CreateProgram();
	bool CreateSkyProgram();
	bool CreateCubeProgram();
public:
	Renderer();
	~Renderer();

	// Draw GUI
	void DefineGUI();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

