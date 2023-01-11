#include <common.h>

void processInput(GLFWwindow* window, Camera& camera)
{
	// window control
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// camera control
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		camera.Move(FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		camera.Move(BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		camera.Move(LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		camera.Move(RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		camera.Move(UP);
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		camera.Move(DOWN);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.Rotate(PITCH, CLOCKWISE);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.Rotate(PITCH, COUNTERCLOCKWISE);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.Rotate(YAW, COUNTERCLOCKWISE);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.Rotate(YAW, CLOCKWISE);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.Rotate(ROLL, CLOCKWISE);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.Rotate(ROLL, COUNTERCLOCKWISE);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		unsigned int stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

void bindTexture(Shader shader, const char* name, unsigned int& texture, unsigned int& index)
{
	shader.setInt(name, index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned int gen_frame_buffer_object()
{
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);

	return fbo;
}

unsigned int gen_render_buffer_object()
{
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);

	return rbo;
}

unsigned int gen_depthmap_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return texture;
}

void render_depthmap_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height, float near, float far, DirectionLight& light, std::vector<Model>& scene, Shader& shader)
{
	glm::mat4 lightSpaceMatrix = light.setLightSpaceMatrix(near, far);
	shader.use();
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (unsigned int i = 0; i < scene.size(); i++)
	{
		shader.setMat4("model", scene[i].getModelMatrix());
		scene[i].Draw(shader);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int gen_depthcubemap_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height)
{
	unsigned int cubmapTexture;
	glGenTextures(1, &cubmapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubmapTexture);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubmapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return cubmapTexture;
}

void render_depthcubemap_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height, PointLight& light, std::vector<Model>& scene, Shader& shader)
{
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	std::vector<glm::mat4> lightSpaceMatrixs = light.getLightSpaceMatrix(aspect);

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	shader.use();
	shader.setVec3("lightPos", light.getPosition());
	shader.setFloat("far_plane", light.getRange());

	for (unsigned int i = 0; i < 6; i++)
	{
		shader.setMat4(("lightSpaceMatrixs[" + std::to_string(i) + "]").c_str(), lightSpaceMatrixs[i]);
	}

	for (unsigned int i = 0; i < scene.size(); i++)
	{
		shader.setMat4("model", scene[i].getModelMatrix());
		scene[i].Draw(shader);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int gen_hdr16_texture(unsigned int& fbo, unsigned int& rbo, const unsigned int& width, const unsigned int& height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// depth buffer
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// attach buffers
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return texture;
}

void render_hdr16_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height, unsigned int& depthMapTexture, std::vector<unsigned int>& depthCubmappingTextures, Camera& camera, DirectionLight& dlight, std::vector<PointLight> plights, std::vector<Model>& scene, Shader& shader, unsigned int& skyboxTexture, Shader& skyboxShader)
{
	glm::mat4 projection = camera.GetProjectionMatrix(width, height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera.GetPosition());
	shader.setMat4("lightSpaceMatrix", dlight.getLightSpaceMatrix());

	unsigned int nTexture = 0;
	shader.setInt("shadowMap", nTexture);
	glActiveTexture(GL_TEXTURE0 + nTexture);
	nTexture += 1;
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	shader.setVec3("lightDir", dlight.getDirection());
	shader.setVec3("dirctLightCol", dlight.getColor());
	shader.setFloat("dirIrradiance", dlight.getIrradiance());

	for (int i = 0; i < 2; i++)
	{
		shader.setInt(("shadowCubmap[" + std::to_string(i) + "]").c_str(), nTexture);
		glActiveTexture(GL_TEXTURE0 + nTexture);
		nTexture += 1;
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubmappingTextures[i]);
		shader.setVec3(("lightPos[" + std::to_string(i) + "]").c_str(), plights[i].getPosition());
		shader.setVec3(("lightCol[" + std::to_string(i) + "]").c_str(), plights[i].getColor());
		shader.setFloat(("Irradiance[" + std::to_string(i) + "]").c_str(), plights[i].getIrradiance());
		shader.setFloat(("far_plane[" + std::to_string(i) + "]").c_str(), plights[i].getRange());
		shader.setFloat(("near_plane[" + std::to_string(i) + "]").c_str(), plights[i].getSize());
	}

	// scene
	glm::mat4 model = glm::mat4(1.0f);
	for (unsigned int i = 0; i < scene.size(); i++)
	{
		model = scene[i].getModelMatrix();
		shader.setMat4("model", model);
		scene[i].Draw(shader);
	}

	// skybox
	glDepthFunc(GL_LEQUAL);

	skyboxShader.use();
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	skyboxShader.setInt("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	renderCube();

	glDepthFunc(GL_LESS);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void render_hdr16_texture_pbr_metalness
(	unsigned int& fbo,
	const unsigned int& width,
	const unsigned int& height,
	unsigned int& depthMapTexture,
	std::vector<unsigned int>& depthCubmappingTextures,
	Camera& camera,
	DirectionLight& dlight,
	std::vector<PointLight> plights,
	std::vector<Model>& scene,
	unsigned int& diffuseIrradianceMap,
	unsigned int& specularIrradianceMap,
	unsigned int& brdfLut,
	MetalnessSettings& settings,
	Shader& shader,
	unsigned int& skyboxTexture,
	Shader& skyboxShader	)
{
	glm::mat4 projection = camera.GetProjectionMatrix(width, height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera.GetPosition());
	shader.setMat4("lightSpaceMatrix", dlight.getLightSpaceMatrix());

	unsigned int nTexture = 0;
	shader.setInt("shadowMap", nTexture);
	glActiveTexture(GL_TEXTURE0 + nTexture);
	nTexture += 1;
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	shader.setVec3("lightDir", dlight.getDirection());
	shader.setVec3("dirctLightCol", dlight.getColor());
	shader.setFloat("dirIrradiance", dlight.getIrradiance());

	for (int i = 0; i < 2; i++)
	{
		shader.setInt(("shadowCubmap[" + std::to_string(i) + "]").c_str(), nTexture);
		glActiveTexture(GL_TEXTURE0 + nTexture);
		nTexture += 1;
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubmappingTextures[i]);
		shader.setVec3(("lightPos[" + std::to_string(i) + "]").c_str(), plights[i].getPosition());
		shader.setVec3(("lightCol[" + std::to_string(i) + "]").c_str(), plights[i].getColor());
		shader.setFloat(("Irradiance[" + std::to_string(i) + "]").c_str(), plights[i].getIrradiance());
		shader.setFloat(("far_plane[" + std::to_string(i) + "]").c_str(), plights[i].getRange());
		shader.setFloat(("near_plane[" + std::to_string(i) + "]").c_str(), plights[i].getSize());
	}

	shader.setInt("diffuseIrradiance", nTexture);
	glActiveTexture(GL_TEXTURE0 + nTexture);
	nTexture += 1;
	glBindTexture(GL_TEXTURE_CUBE_MAP, diffuseIrradianceMap);
	shader.setInt("prefilterMap", nTexture);
	glActiveTexture(GL_TEXTURE0 + nTexture);
	nTexture += 1;
	glBindTexture(GL_TEXTURE_CUBE_MAP, specularIrradianceMap);
	shader.setInt("brdfLut", nTexture);
	glActiveTexture(GL_TEXTURE0 + nTexture);
	nTexture += 1;
	glBindTexture(GL_TEXTURE_2D, brdfLut);
	shader.setVec3("albedo", settings.getAlbedo());
	shader.setFloat("metallic", settings.getMetallic());
	shader.setFloat("roughness", settings.getRoughness());

	// scene
	glm::mat4 model = glm::mat4(1.0f);
	for (unsigned int i = 0; i < scene.size(); i++)
	{
		model = scene[i].getModelMatrix();
		shader.setMat4("model", model);
		scene[i].Draw(shader);
	}

	// skybox
	glDepthFunc(GL_LEQUAL);

	skyboxShader.use();
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	skyboxShader.setInt("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	renderCube();

	glDepthFunc(GL_LESS);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int load_hdr_env(const char* file)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(file, &width, &height, &nrComponents, 0);

	unsigned int hdrTexture;

	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Failed to load HDR image." << std::endl;
	}

	return hdrTexture;
}

glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 captureViews[] =
{
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};

unsigned int gen_envcubemap_texture(unsigned int& fbo, unsigned int& rbo, const unsigned int& width, const unsigned int& height, unsigned int& hdrTexture, Shader& shader)
{
	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);// enable pre-filter mipmap sampling (combatting visible dots artifact)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	shader.use();
	shader.setInt("equirectangularMap", 0);
	shader.setMat4("projection", captureProjection);
	
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer not complete!" << std::endl;

	for (unsigned int i = 0; i < 6; ++i)
	{
		shader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	return envCubemap;
}

unsigned int gen_diffuseIrradianceMap(unsigned int& fbo, unsigned int& rbo, unsigned int& envCubemap, Shader& shader)
{
	unsigned int irradianceMap;
	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	shader.use();
	shader.setInt("environmentMap", 0);
	shader.setMat4("projection", captureProjection);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer not complete!" << std::endl;

	for (unsigned int i = 0; i < 6; ++i)
	{
		shader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return irradianceMap;
}

unsigned int gen_prefilterMap(unsigned int& fbo, unsigned int& rbo, unsigned int& envCubemap, const unsigned int& width, const unsigned int& height, Shader& shader)
{	
	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	shader.use();
	shader.setInt("environmentMap", 0);
	shader.setMat4("projection", captureProjection);
	shader.setFloat("resolution", width);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer not complete!" << std::endl;

	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		shader.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			shader.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return prefilterMap;
}

unsigned int gen_brdfLut(const char* file, unsigned int& fbo, unsigned int& rbo, const unsigned int& width, const unsigned int& height, Shader& shader)
{
	stbi_set_flip_vertically_on_load(true);
	int _width, _height, nrComponents;
	float* data = stbi_loadf(file, &_width, &_height, &nrComponents, 0);

	unsigned int brdfLut;

	glGenTextures(1, &brdfLut);
	glBindTexture(GL_TEXTURE_2D, brdfLut);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, _width, _height, 0, GL_RG, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLut, 0);

		glViewport(0, 0, width, height);
		shader.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderQuad();
		/*
		todo: save brdfLut 
		*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	return brdfLut;
}

unsigned int gen_kcAvgELut(unsigned int& fbo, unsigned int& rbo, unsigned int& brdfLut, const unsigned int& width, std::vector<MicrofacetSettings>& settings, Shader& shader)
{
	unsigned int height = settings.size();
	
	unsigned int kcAvgELut;
	glGenTextures(1, &kcAvgELut);
	glBindTexture(GL_TEXTURE_2D, kcAvgELut);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, kcAvgELut, 0);

	glViewport(0, 0, width, height);
	shader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (unsigned int i = 0; i < height; i++)
	{
		shader.setInt("index", i);
		shader.setVec3("f0", settings[i].getF0());
		bindTexture(shader, "brdfLut", brdfLut, i);
		renderQuad();
	}
	/*
	shader.setVec3("f0", settings[0].getF0());
	unsigned int index = 0;
	bindTexture(shader, "brdfLut", brdfLut, index);

	renderQuad();
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return kcAvgELut;
}

void render_hdr16_texture_pbr_microfacet
(unsigned int& fbo,
	const unsigned int& width,
	const unsigned int& height,
	unsigned int& depthMapTexture,
	std::vector<unsigned int>& depthCubmappingTextures,
	Camera& camera,
	DirectionLight& dlight,
	std::vector<PointLight> plights,
	std::vector<Model>& scene,
	unsigned int& specularIrradianceMap,
	unsigned int& brdfLut,
	unsigned int& kcAvgELut,
	std::vector<MicrofacetSettings>& settings,
	Shader& shader,
	unsigned int& skyboxTexture,
	Shader& skyboxShader)
{
	glm::mat4 projection = camera.GetProjectionMatrix(width, height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera.GetPosition());
	shader.setMat4("lightSpaceMatrix", dlight.getLightSpaceMatrix());

	unsigned int nTexture = 0;
	shader.setInt("shadowMap", nTexture);
	glActiveTexture(GL_TEXTURE0 + nTexture);
	nTexture += 1;
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	shader.setVec3("lightDir", dlight.getDirection());
	shader.setVec3("dirctLightCol", dlight.getColor());
	shader.setFloat("dirIrradiance", dlight.getIrradiance());

	for (int i = 0; i < 2; i++)
	{
		shader.setInt(("shadowCubmap[" + std::to_string(i) + "]").c_str(), nTexture);
		glActiveTexture(GL_TEXTURE0 + nTexture);
		nTexture += 1;
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubmappingTextures[i]);
		shader.setVec3(("lightPos[" + std::to_string(i) + "]").c_str(), plights[i].getPosition());
		shader.setVec3(("lightCol[" + std::to_string(i) + "]").c_str(), plights[i].getColor());
		shader.setFloat(("Irradiance[" + std::to_string(i) + "]").c_str(), plights[i].getIrradiance());
		shader.setFloat(("far_plane[" + std::to_string(i) + "]").c_str(), plights[i].getRange());
		shader.setFloat(("near_plane[" + std::to_string(i) + "]").c_str(), plights[i].getSize());
	}

	shader.setInt("prefilterMap", nTexture);
	glActiveTexture(GL_TEXTURE0 + nTexture);
	nTexture += 1;
	glBindTexture(GL_TEXTURE_CUBE_MAP, specularIrradianceMap);
	shader.setInt("brdfLut", nTexture);
	glActiveTexture(GL_TEXTURE0 + nTexture);
	nTexture += 1;
	glBindTexture(GL_TEXTURE_2D, brdfLut);
	shader.setInt("kcAvgELut", nTexture);
	glActiveTexture(GL_TEXTURE0 + nTexture);
	nTexture += 1;
	glBindTexture(GL_TEXTURE_2D, kcAvgELut);

	// scene
	glm::mat4 model = glm::mat4(1.0f);
	for (unsigned int i = 0; i < scene.size(); i++)
	{
		model = scene[i].getModelMatrix();
		shader.setVec3("f0", settings[i].getF0());
		shader.setFloat("roughness", settings[i].getRoughness());
		shader.setMat4("model", model);
		shader.setInt("index", i);
		scene[i].Draw(shader);
	}

	// skybox
	glDepthFunc(GL_LEQUAL);

	skyboxShader.use();
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	skyboxShader.setInt("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	renderCube();

	glDepthFunc(GL_LESS);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// imgui window
void camera_imgui_window(Camera& camera)
{
	
	ImGui::Begin("---Camera Settings---");
	float cPos[3] = { camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z };
	if (ImGui::InputFloat3("position", cPos))
	{
		glm::vec3 cPosVec(cPos[0], cPos[1], cPos[2]);
		camera.SetPosition(cPosVec);
	}
	//float cLookAt[3] = { camera. };
	static float sensitive = 0.001f;
	if (ImGui::InputFloat("rotation sensitive", &sensitive, 0.001f, 1.0f, "%.3f"))
	{
		camera.SetSensitive(sensitive);
	}
	ImGui::End();
}

void model_imgui_window(Camera& camera, std::vector<Model>& scene)
{
	ImGui::Begin("---Model Settings---");
	static int i = 0;
	ImGui::Text("Model ID: %d", i);
	ImGui::SameLine();
	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{
		if (i == 0) i = scene.size() - 1;
		else { if (i != 0) i = (i - 1) % scene.size(); }
	}
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{
		i = (i + 1) % scene.size();
	}
	ImGui::Text("Position: %.3f %.3f %.3f", scene[i].GetPosition().x, scene[i].GetPosition().y, scene[i].GetPosition().z);
	ImGui::Text("Scale: %.3f %.3f %.3f", scene[i].GetScale().x, scene[i].GetScale().y, scene[i].GetScale().z);
	if (ImGui::Button("Focus"))
	{
		camera.LookAtNewTargetPos(scene[i].GetPosition());
	}
	ImGui::End();
}

void point_light_imgui_window(std::vector<PointLight>& pointLights)
{
	ImGui::Begin("---Point Light Settings---");
	static int i = 0;
	ImGui::Text("Point Light ID: %d", i);
	ImGui::SameLine();
	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{
		if (i == 0) i = pointLights.size() - 1;
		else { if (i != 0) i = (i - 1) % pointLights.size(); }
	}
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{
		i = (i + 1) % pointLights.size();
	}
	float plPos[3] = { pointLights[i].getPosition().x, pointLights[i].getPosition().y, pointLights[i].getPosition().z };
	if (ImGui::InputFloat3("position", plPos))
	{
		glm::vec3 plPosVec(plPos[0], plPos[1], plPos[2]);
		pointLights[i].setPosition(plPosVec);
	}
	float plCol[3] = { pointLights[i].getColor().x, pointLights[i].getColor().y, pointLights[i].getColor().z };
	if (ImGui::ColorEdit3("color", plCol))
	{
		glm::vec3 plColVec(plCol[0], plCol[1], plCol[2]);
		pointLights[i].setColor(plColVec);
	}
	float irradiance = pointLights[i].getIrradiance();
	if (ImGui::InputFloat("irradiance", &irradiance, 1.0f, 1.0f, "%.3f"))
	{
		pointLights[i].setIrradiance(irradiance);
	}
	ImGui::End();
}

void pbr_imgui_window(MetalnessSettings& settings)
{
	ImGui::Begin("---PBR Settings---");
	float albedof[3] = { settings.getAlbedo().r, settings.getAlbedo().g, settings.getAlbedo().b };
	if (ImGui::ColorEdit3("albedo", albedof))
	{
		glm::vec3 albedoN(albedof[0], albedof[1], albedof[2]);
		settings.setAlbedo(albedoN);
	}
	static float metallic_ = settings.getMetallic();
	if (ImGui::SliderFloat("metallic", &metallic_, 0., 1., "%.3f"))
	{
		settings.setMetallic(metallic_);
	}
	static float roughness_ = settings.getRoughness();
	if (ImGui::SliderFloat("roughness", &roughness_, 0., 1., "%.3f"))
	{
		settings.setRoughness(roughness_);
	}
	ImGui::End();
}