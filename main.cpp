#include "Window.hpp"
#include <glm/vec3.hpp>
#include <chrono>
#include <vector>

#include "Shader.hpp"
#include "glm/glm.hpp"
#include "Noise.hpp"

float near = 1.0;
float far = 3000.0;
void renderQuad();
struct Mesh {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
};

Mesh createTerrainMesh(int width, int length) {
	Mesh mesh;
	float maxHeight = 0;
	float minHeight = 1;

	auto start = std::chrono::high_resolution_clock::now();

	mesh.vertices.reserve(width * length);
	mesh.indices.reserve(width * length);
	for (int x = 0; x < width; x++) {
		for (int z = 0; z < length; z++) {
			float height = fractionalBrownianMotion(Noise::Perlin, glm::vec2(x, z), 10, 250);
			if (height > maxHeight) maxHeight = height;
			if (height < minHeight) minHeight = height;
			mesh.vertices.push_back(glm::vec3(x, height * 100, z));
		}
	}
	std::cout << "max: " << maxHeight << ", min: " << minHeight << std::endl;

	for (int x = 0; x < width - 1; x++) {
		for (int z = 0; z < length - 1; z++) {
			mesh.indices.push_back(x * length + z);
			mesh.indices.push_back(x * length + z + 1);
			mesh.indices.push_back((x + 1) * length + z);

			mesh.indices.push_back((x + 1) * length + z);
			mesh.indices.push_back(x * length + z + 1);
			mesh.indices.push_back((x + 1) * length + z + 1);
		}
	}

	std::vector<glm::vec3> normals(mesh.vertices.size(), glm::vec3(0.0f));
	for (int i = 0; i < mesh.indices.size(); i+=3) {
		int i0 = mesh.indices[i];
		int i1 = mesh.indices[i + 1];
		int i2 = mesh.indices[i + 2];

		glm::vec3 edge1 = mesh.vertices[i1] - mesh.vertices[i0];
		glm::vec3 edge2 = mesh.vertices[i2] - mesh.vertices[i0];

		glm::vec3 normal = glm::cross(edge1, edge2);

		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	for (glm::vec3 &n : normals) {
		n = glm::normalize(n);
	}

	mesh.normals = normals;

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Terrain generation took: " << duration.count() << "ms" << std::endl;

	return mesh;
}

int main()
{
	Window window;
	if (!window.initialized)
		return -1;

	Shader shader("C:/Users/Konrad/source/repos/ProceduralTerrain/Shader Files/shader.vert", "C:/Users/Konrad/source/repos/ProceduralTerrain/Shader Files/shader.frag");
	Shader depthShader("C:/Users/Konrad/source/repos/ProceduralTerrain/Shader Files/depth.vert");
	Shader debugTexture("C:/Users/Konrad/source/repos/ProceduralTerrain/Shader Files/viewTexture.vert", "C:/Users/Konrad/source/repos/ProceduralTerrain/Shader Files/viewTexture.frag");

	glm::mat4 projection = glm::perspective((float)glm::radians(45.0), 1.0f, near, far);

	Mesh map = createTerrainMesh(1000, 1000);

	GLuint VAOs[2];
	GLuint buffer[3];
	glCreateVertexArrays(2, VAOs);
	glCreateBuffers(3, buffer);

	glNamedBufferStorage(buffer[0], map.vertices.size() * sizeof(glm::vec3), map.vertices.data(), GL_MAP_READ_BIT);
	glVertexArrayVertexBuffer(VAOs[0], 0, buffer[0], 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(VAOs[0], 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(VAOs[0], 0, 0);
	glEnableVertexArrayAttrib(VAOs[0], 0);

	glNamedBufferStorage(buffer[1], map.normals.size() * sizeof(glm::vec3), map.normals.data(), GL_MAP_READ_BIT);
	glVertexArrayVertexBuffer(VAOs[0], 1, buffer[1], 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(VAOs[0], 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(VAOs[0], 1, 1);
	glEnableVertexArrayAttrib(VAOs[0], 1);

	glNamedBufferStorage(buffer[2], map.indices.size() * sizeof(unsigned int), map.indices.data(), GL_MAP_READ_BIT);
	glVertexArrayElementBuffer(VAOs[0], buffer[2]);

	GLuint depthMapFBO;
	GLuint depthMap;
	int SHADOW_DIM = 8192;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_DIM, SHADOW_DIM, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::vec3 lightDirection = glm::vec3(-5, -4, -3);
	glm::vec3 middle = glm::vec3(500, 50, 500);
	glm::vec3 lightPosition = -lightDirection * 100.f + middle;
	glm::mat4 lightProjection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, near, far);
	glm::mat4 lightView = glm::lookAt(lightPosition, middle, glm::vec3(0, 1, 0));

	window.camera.position = lightPosition;
	window.camera.lookAt(middle);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	while (!window.shouldClose())
	{
		if (window.debugInformation.showWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// render depth from light for shadow mapping
		depthShader.use();
		depthShader.uniformMatrix4("lightMatrix", lightProjection * lightView);
		glViewport(0, 0, SHADOW_DIM, SHADOW_DIM);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(VAOs[0]);
			glDrawElements(GL_TRIANGLES, map.indices.size(), GL_UNSIGNED_INT, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// reset viewport
		glViewport(0, 0, window.height, window.width);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.uniformMatrix4("projection", projection);
		shader.uniformMatrix4("view", window.camera.getView());

		shader.uniformVector3("cameraPos", window.camera.position);
		shader.uniformVector3("lightDirection", lightDirection);
		shader.uniformMatrix4("lightMatrix", lightProjection * lightView);

		shader.uniformFloat("near", near);
		shader.uniformFloat("far", far);
		shader.uniformBool("stylized", window.debugInformation.stylized);

		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDrawElements(GL_TRIANGLES, map.indices.size(), GL_UNSIGNED_INT, 0);

		debugTexture.use();
		debugTexture.uniformFloat("near_plane", near);
		debugTexture.uniformFloat("far_plane", far);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		// renderQuad();

		window.runLoopFunctions();
	}
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
