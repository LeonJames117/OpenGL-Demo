#include "Renderer.h"
#include "Camera.h"
#include "ImageLoader.h"
Renderer::Renderer() 
{

}

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	// TODO: clean up any memory used including OpenGL objects via glDelete* calls
	glDeleteProgram(m_program);
	glDeleteProgram(SkyProgram);
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &SkyVAO);
	
}

// Use IMGUI for a simple on screen GUI
void Renderer::DefineGUI()
{
	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("3GP");						// Create a window called "3GP" and append into it.

	ImGui::Text("Visibility.");					// Display some text (you can use a format strings too)	

	ImGui::Checkbox("Wireframe", &m_wireframe);	// A checkbox linked to a member variable

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
	ImGui::End();
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()
{
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program))
		return false;

	return true;
}




bool Renderer::CreateSkyProgram()
{
	// Create a new program (returns a unqiue id)
	SkyProgram = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/Sky_Vert.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/Sky_Frag.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(SkyProgram, vertex_shader);

	// Attach the fragment shader (copies it)
	glAttachShader(SkyProgram, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(SkyProgram))
		return false;

	return true;
}

bool Renderer::CreateCubeProgram()
{
	// Create a new program (returns a unqiue id)
	CubeProgram = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/Cube_vertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/Cube_fragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(CubeProgram, vertex_shader);

	// Attach the fragment shader (copies it)
	glAttachShader(CubeProgram, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(CubeProgram))
		return false;

	return true;
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;
	if (!CreateSkyProgram())
		return false;
	if (!CreateCubeProgram())
		return false;

	glm::vec3 FrontCubevertices[4] =
	{
		{-10, -10, 10},//0
		{10, -10 , 10 },//1
		{-10, 10, 10},//2
		{10, 10, 10},//3
	};

	glm::vec3 BackCubevertices[4] =
	{
		{-10.0f, -10.0f, -10.0f},//4
		{10.0f, -10.0f , -10.0f},//5
		{-10.0f, 10.0f, -10.0f},//6
		{10.0f, 10.0f, -10.0f},//7
	};

	glm::vec3 RightCubevertices[4] =
	{
		{10, -10 , 10 },//1
		{10.0f, -10.0f , -10.0f},//5
		{10, 10, 10},//3
		{10.0f, 10.0f, -10.0f},//7
	};


	glm::vec3 LeftCubevertices[4] =
	{
		{-10, -10, 10},//0
		{-10.0f, -10.0f, -10.0f},//4
		{-10, 10, 10},//2
		{-10.0f, 10.0f, -10.0f},//6
	};

	glm::vec3 TopCubevertices[4] =
	{
		{-10, 10, 10},//2
		{10, 10, 10},//3
		{-10.0f, 10.0f, -10.0f},//6
		{10.0f, 10.0f, -10.0f},//7
	};


	glm::vec3 BottomCubevertices[4] =
	{
		{-10, -10, 10},//0
		{10, -10 , 10 },//1
		{-10, -10, -10},//4
		{10, -10 , -10},//5
	};


	std::vector<GLfloat> colors =
	{
		//Front (Green)
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		//Back (Blue)
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		//Right (Red)
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		//Left (Orange)
		1.0f, 0.6f, 0.0f,
		1.0f, 0.6f, 0.0f,
		1.0f, 0.6f, 0.0f,
		1.0f, 0.6f, 0.0f,

		//Top (White)
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		//Bottom (Yellow)
		1.0f, 3.0f, 0.0f,
		1.0f, 3.0f, 0.0f,
		1.0f, 3.0f, 0.0f,
		1.0f, 3.0f, 0.0f,
	};

	std::vector<glm::vec3> CubeCorners;
	//Front Face
	CubeCorners.push_back(FrontCubevertices[0]);
	CubeCorners.push_back(FrontCubevertices[1]);
	CubeCorners.push_back(FrontCubevertices[2]);
	CubeCorners.push_back(FrontCubevertices[3]);
	//Back Face
	CubeCorners.push_back(BackCubevertices[0]);
	CubeCorners.push_back(BackCubevertices[1]);
	CubeCorners.push_back(BackCubevertices[2]);
	CubeCorners.push_back(BackCubevertices[3]);
	//Right Face
	CubeCorners.push_back(RightCubevertices[0]); //Point 1 : Vector 8
	CubeCorners.push_back(RightCubevertices[1]); //Point 5 : Vector 9
	CubeCorners.push_back(RightCubevertices[2]); //Point 3 : Vector 10
	CubeCorners.push_back(RightCubevertices[3]); //Point 7 : Vector 11
	//Left Face 
	CubeCorners.push_back(LeftCubevertices[0]); //Point 0 : Vector 12
	CubeCorners.push_back(LeftCubevertices[1]); //Point 4 : Vector 13
	CubeCorners.push_back(LeftCubevertices[2]); //Point 2 : Vector 14
	CubeCorners.push_back(LeftCubevertices[3]); //Point 6 : Vector 15
	//Top Face
	CubeCorners.push_back(TopCubevertices[0]); //Point 2 : Vector 16
	CubeCorners.push_back(TopCubevertices[1]); //Point 3 : Vector 17
	CubeCorners.push_back(TopCubevertices[2]); //Point 6 : Vector 18
	CubeCorners.push_back(TopCubevertices[3]); //Point 7 : Vector 19
	//Bottom Face
	CubeCorners.push_back(BottomCubevertices[0]); //Point 0 : Vector 20
	CubeCorners.push_back(BottomCubevertices[1]); //Point 1 : Vector 21
	CubeCorners.push_back(BottomCubevertices[2]); //Point 4 : Vector 22
	CubeCorners.push_back(BottomCubevertices[3]); //Point 5 : Vector 23 

	std::vector<GLint> Cube_Elements;
	//Front Face
	Cube_Elements.push_back(0);
	Cube_Elements.push_back(1);
	Cube_Elements.push_back(2);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	Cube_Elements.push_back(1);
	Cube_Elements.push_back(3);
	Cube_Elements.push_back(2);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	//Back Face
	Cube_Elements.push_back(7);
	Cube_Elements.push_back(5);
	Cube_Elements.push_back(4);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	Cube_Elements.push_back(4);
	Cube_Elements.push_back(6);
	Cube_Elements.push_back(7);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	//Right Face
	Cube_Elements.push_back(10);
	Cube_Elements.push_back(8);
	Cube_Elements.push_back(9);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	Cube_Elements.push_back(9);
	Cube_Elements.push_back(11);
	Cube_Elements.push_back(10);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	//Left Face
	Cube_Elements.push_back(13);
	Cube_Elements.push_back(12);
	Cube_Elements.push_back(14);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	Cube_Elements.push_back(14);
	Cube_Elements.push_back(15);
	Cube_Elements.push_back(13);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	//Top Face
	Cube_Elements.push_back(16);
	Cube_Elements.push_back(17);
	Cube_Elements.push_back(19);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	Cube_Elements.push_back(19);
	Cube_Elements.push_back(18);
	Cube_Elements.push_back(16);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	//Bottom Face

	Cube_Elements.push_back(20);
	Cube_Elements.push_back(22);
	Cube_Elements.push_back(21);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	Cube_Elements.push_back(21);
	Cube_Elements.push_back(22);
	Cube_Elements.push_back(23);

	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));
	CubeNormals.push_back(glm::vec3(0, 1, 0));

	

	float NumberofCellsX = 100;
	float NumberofCellsZ = 100;

	int NumberofVertsX = NumberofCellsX + 1;
	int NumberofVertsZ = NumberofCellsZ + 1;
	bool Diamond = true;


	Helpers::ImageLoader IMLoader;
	Helpers::ImageLoader IMLSkyBack;
	Helpers::ImageLoader IMLSkyBottom;
	Helpers::ImageLoader IMLSkyFront;
	Helpers::ImageLoader IMLSkyLeft;
	Helpers::ImageLoader IMLSkyRight;
	Helpers::ImageLoader IMLSkyTop;

	std::vector<GLfloat> SkyBoxVerts =
	{
		// positions          
		-5.0f,  5.0f, -5.0f,
		-5.0f, -5.0f, -5.0f,
		 5.0f, -5.0f, -5.0f,
		 5.0f, -5.0f, -5.0f,
		 5.0f,  5.0f, -5.0f,
		-5.0f,  5.0f, -5.0f,

		-5.0f, -5.0f,  5.0f,
		-5.0f, -5.0f, -5.0f,
		-5.0f,  5.0f, -5.0f,
		-5.0f,  5.0f, -5.0f,
		-5.0f,  5.0f,  5.0f,
		-5.0f, -5.0f,  5.0f,

		 5.0f, -5.0f, -5.0f,
		 5.0f, -5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f, -5.0f,
		 5.0f, -5.0f, -5.0f,

		-5.0f, -5.0f,  5.0f,
		-5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f, -5.0f,  5.0f,
		-5.0f, -5.0f,  5.0f,

		-5.0f,  5.0f, -5.0f,
		 5.0f,  5.0f, -5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		-5.0f,  5.0f,  5.0f,
		-5.0f,  5.0f, -5.0f,

		-5.0f, -5.0f, -5.0f,
		-5.0f, -5.0f,  5.0f,
		 5.0f, -5.0f, -5.0f,
		 5.0f, -5.0f, -5.0f,
		-5.0f, -5.0f,  5.0f,
		 5.0f, -5.0f,  5.0f
	};

	if (IMLoader.Load("Data\\Heightmaps\\Heightmap.jpg"))
	{
		float VertXToIm = IMLoader.Width() / NumberofVertsX;
		float VertZToIm = IMLoader.Height() / NumberofVertsZ;

		BYTE* HeightMapData = IMLoader.GetData();
		for (size_t x = 0; x < NumberofVertsX; x++)
		{
			for (size_t z = 0; z < NumberofVertsZ; z++)
			{
				float ImageX = VertXToIm * x;
				float ImageZ = VertXToIm * z;

				size_t Offset = ((size_t)ImageX + (size_t)ImageZ * IMLoader.Width()) * 4;
				BYTE height = HeightMapData[Offset]; 

				Corners.push_back(glm::vec3(x * 8, height, z * 8));
				UV.push_back(glm::vec2(z / NumberofCellsX, x / NumberofCellsZ));
				Normals.push_back(glm::vec3(0, 1, 0));
			}
		}
	}
	else
	{
		for (size_t i = 0; i < NumberofVertsX; i++)
		{
			for (size_t x = 0; x < NumberofVertsZ; x++)
			{
				Corners.push_back(glm::vec3(i * 8, 0, x * 8));
				UV.push_back(glm::vec2(x / NumberofCellsX, i / NumberofCellsZ));
				Normals.push_back(glm::vec3(0, 1, 0));
			}
		}
	}


	for (size_t CellZ = 0; CellZ < NumberofCellsZ; CellZ++)
	{
		for (size_t CellX = 0; CellX < NumberofCellsX; CellX++)
		{
			int StartVertex = (CellZ * NumberofVertsX) + CellX;
			if (Diamond)
			{
				//first triangle
				elements.push_back(StartVertex);
				elements.push_back(StartVertex + 1);
				elements.push_back(StartVertex + NumberofVertsX);
				//	second triangle
				elements.push_back(StartVertex + 1);
				elements.push_back(StartVertex + NumberofVertsX + 1);
				elements.push_back(StartVertex + NumberofVertsX);
		
				std::cout << Diamond << "\n";
			}
			else
			{
				//first triangle
				elements.push_back(StartVertex);
				elements.push_back(StartVertex + NumberofVertsX + 1);
				elements.push_back(StartVertex + NumberofVertsX);
			//	second triangle
				elements.push_back(StartVertex + 1);
				elements.push_back(StartVertex + NumberofVertsX + 1);
				elements.push_back(StartVertex);
			
				std::cout << Diamond << "\n";
			}
			Diamond = !Diamond;
		}
		Diamond = !Diamond;
	}


	for (size_t i = 0; i < elements.size(); i += 3)
	{
		glm::vec3 v0{ Corners [elements[i]] };
		glm::vec3 v1{ Corners[elements[i+1]] };
		glm::vec3 v2{ Corners[elements[i + 2]] };

		glm::vec3 Side1 = v1-v0;
		glm::vec3 Side2 = v2 - v0;

		glm::vec3 TriangleNormal = glm::normalize(glm::cross(Side1, Side2));
		
		Normals[elements[i]] += TriangleNormal;
		Normals[elements[i+1]] += TriangleNormal;
		Normals[elements[i+2]] += TriangleNormal;
	}
	
	

	//Skybox Prep
	if (!IMLSkyBack.Load("Data\\Models\\Sky\\Hills\\SkyBox_Back.JPG"))
	{
		MessageBox(NULL, L"Can't Load Skybox Back", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else if (!IMLSkyBottom.Load("Data\\Models\\Sky\\Hills\\SkyBox_Bottom.JPG"))
	{
		MessageBox(NULL, L"Can't Load Skybox Bottom", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else if (!IMLSkyFront.Load("Data\\Models\\Sky\\Hills\\SkyBox_Front.JPG"))
	{
		MessageBox(NULL, L"Can't Load Skybox Front", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else if (!IMLSkyLeft.Load("Data\\Models\\Sky\\Hills\\SkyBox_Left.JPG"))
	{
		MessageBox(NULL, L"Can't Load Skybox Left", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else if (!IMLSkyRight.Load("Data\\Models\\Sky\\Hills\\SkyBox_Right.JPG"))
	{
		MessageBox(NULL, L"Can't Load Skybox Right", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else if (!IMLSkyTop.Load("Data\\Models\\Sky\\Hills\\SkyBox_Top.JPG"))
	{
		MessageBox(NULL, L"Can't Load Skybox Top", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else
	{
		//Creates the Texure object
		glGenTextures(1, &SkyBoxTex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxTex);
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, IMLSkyRight.Width(), IMLSkyRight.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, IMLSkyRight.GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, IMLSkyLeft.Width(), IMLSkyLeft.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, IMLSkyLeft.GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, IMLSkyBottom.Width(), IMLSkyBottom.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, IMLSkyBottom.GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, IMLSkyTop.Width(), IMLSkyTop.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, IMLSkyTop.GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, IMLSkyFront.Width(), IMLSkyFront.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, IMLSkyFront.GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, IMLSkyBack.Width(), IMLSkyBack.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, IMLSkyBack.GetData());
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glGenerateMipmap(GL_TEXTURE_2D);

	}


	if (!IMLoader.Load("Data\\Textures\\dirt_earth-n-moss_df_.DDS"))
	{
		MessageBox(NULL, L"Can't Load Grass Texture", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else
	{
		//Creates the Texure object
		glGenTextures(1, &Terraintex);
		glBindTexture(GL_TEXTURE_2D, Terraintex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMLoader.Width(), IMLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, IMLoader.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	/*
		TODO 2: Next you need to create VBOs for the vertices and the colours
		You can look back to last week for examples
	*/



	Helpers::ModelLoader loader;
	if (!loader.LoadFromFile("Data\\Models\\Jeep\\Jeep.obj"))
	{
		MessageBox(NULL, L"Can't Load Jeep Model", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}



	for (const Helpers::Mesh& mesh : loader.GetMeshVector())
	{

		GLuint JeepPositonVBO;
		glGenBuffers(1, &JeepPositonVBO);
		glBindBuffer(GL_ARRAY_BUFFER, JeepPositonVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint JeepNormalsVBO;
		glGenBuffers(1, &JeepNormalsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, JeepNormalsVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.normals.size(), mesh.normals.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint JeepElementsBuffer;
		glGenBuffers(1, &JeepElementsBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, JeepElementsBuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.elements.size(), mesh.elements.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		JeepNumElements = mesh.elements.size();

		GLuint JeepUVVBO;
		glGenBuffers(1, &JeepUVVBO);
		glBindBuffer(GL_ARRAY_BUFFER, JeepUVVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvCoords.size(), mesh.uvCoords.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glGenVertexArrays(1, &JeepVAO);
		glBindVertexArray(JeepVAO);

		glBindBuffer(GL_ARRAY_BUFFER, JeepPositonVBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, JeepElementsBuffer);

		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, JeepElementsBuffer);
		
		glBindBuffer(GL_ARRAY_BUFFER, JeepUVVBO);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, JeepNormalsVBO);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindVertexArray(0);


	}

	//Load Jeep Texture
	Helpers::ImageLoader JeepIMLoader;
	if (!JeepIMLoader.Load("Data\\Models\\Jeep\\Jeep_rood.jpg"))
	{
		MessageBox(NULL, L"Can't Load Jeep Texture", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else
	{
		//Creates the Texure object
		glGenTextures(1, &Jeeptex);
		glBindTexture(GL_TEXTURE_2D, Jeeptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, JeepIMLoader.Width(), JeepIMLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, JeepIMLoader.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);

	}



	GLuint TerrainPositonVBO;
	glGenBuffers(1, &TerrainPositonVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TerrainPositonVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Corners.size(), Corners.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint PositonVBO;
	glGenBuffers(1, &PositonVBO);
	glBindBuffer(GL_ARRAY_BUFFER, PositonVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * CubeCorners.size(), CubeCorners.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint ColourVBO;
	glGenBuffers(1, &ColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, ColourVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), colors.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint CubeNormalsVBO;
	glGenBuffers(1, &CubeNormalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, CubeNormalsVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* CubeNormals.size(), CubeNormals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint SkyVBO;
	glGenBuffers(1, &SkyVBO);
	glBindBuffer(GL_ARRAY_BUFFER, SkyVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* SkyBoxVerts.size(), SkyBoxVerts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint UVVBO;
	glGenBuffers(1, &UVVBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * UV.size(), UV.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint NormalsVBO;
	glGenBuffers(1, &NormalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NormalsVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* Normals.size(), Normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint ElementsBuffer;
	glGenBuffers(1, &ElementsBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementsBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::vec3) * elements.size(), elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_numElements = elements.size();


	GLuint CubeElementsBuffer;
	glGenBuffers(1, &CubeElementsBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeElementsBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::vec3)* Cube_Elements.size(), Cube_Elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	CubeNumElements = Cube_Elements.size();
	

	////////////////////// VAOS ////////////////////

	glGenVertexArrays(1, &SkyVAO);
	glBindVertexArray(SkyVAO); //start binding to VAO

	glBindBuffer(GL_ARRAY_BUFFER, SkyVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0); //end

	glGenVertexArrays(1, &CubeVAO);
	glBindVertexArray(CubeVAO); //start binding to VAO
	
	glBindBuffer(GL_ARRAY_BUFFER, PositonVBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeElementsBuffer);


	glBindBuffer(GL_ARRAY_BUFFER, ColourVBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, CubeNormalsVBO);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glBindVertexArray(0); //end


	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO); //start binding to VAO


	glBindBuffer(GL_ARRAY_BUFFER, TerrainPositonVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementsBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, UVVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, NormalsVBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0); //end

	return true;
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{			
	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, 1.0f, 1500.0f); // Last 2 are near and far plane
	// Compute camera view matrix and combine with projection matrix for passing to shader
	
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform = projection_xform * view_xform;


	// SkyBox Draw
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(SkyProgram);

	glm::mat4 view_xform2 = glm::mat4(glm::mat3(view_xform));
	glm::mat4 combined_xform2 = projection_xform * view_xform2;
	GLuint Skycombined_xform_id = glGetUniformLocation(SkyProgram, "combined_xform2");
	glUniformMatrix4fv(Skycombined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform2));

	glm::mat4 model_xform = glm::mat4(1);
	GLuint Skymodel_xform_id = glGetUniformLocation(SkyProgram, "model_xform");
	glUniformMatrix4fv(Skymodel_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

	GLuint SkyboxUniformID = glGetUniformLocation(SkyProgram, "Skybox");
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxTex);
	
	
	glBindVertexArray(SkyVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);


	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);

	// Send the combined matrix to the shader in a uniform
	GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));
	
	// Send the model matrix to the shader in a uniform
	GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");
	glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));
	glActiveTexture(GL_TEXTURE0);
	

	//Terrain Draw
	glBindTexture(GL_TEXTURE_2D, Terraintex);
	glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

	
	//Jeep Draw
	glBindTexture(GL_TEXTURE_2D, Jeeptex);
	glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);
	glBindVertexArray(JeepVAO);

	glDrawElements(GL_TRIANGLES, JeepNumElements, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

	glUseProgram(CubeProgram);

	// Send the combined matrix to the shader in a uniform
	GLuint Cube_combined_xform_id = glGetUniformLocation(CubeProgram, "combined_xform");
	glUniformMatrix4fv(Cube_combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	model_xform = glm::translate(model_xform, glm::vec3{ 500.0f, 450.0f, 500.0f });
	static float angle = 0;
	static bool rotateY = true;

	if (rotateY) // Rotate around y axis		
		model_xform = glm::rotate(model_xform, angle, glm::vec3{ 0 ,1,0 });
	else // Rotate around x axis		
		model_xform = glm::rotate(model_xform, angle, glm::vec3{ 1 ,0,0 });

	angle += 0.001f;
	if (angle > glm::two_pi<float>())
	{
		angle = 0;
		rotateY = !rotateY;
	}
	// Send the model matrix to the shader in a uniform
	GLuint Cube_model_xform_id = glGetUniformLocation(CubeProgram, "model_xform");
	glUniformMatrix4fv(Cube_model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

	glBindVertexArray(CubeVAO);
	glDrawElements(GL_TRIANGLES, CubeNumElements, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);


	 //Uncomment all the lines below to rotate cube first round y then round x
	
	



}

