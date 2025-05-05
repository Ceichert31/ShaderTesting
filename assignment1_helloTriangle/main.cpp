//Modified by Christopher Eichert

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ew/shader.h>
#include <ew/texture.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float fillAmount = 0;

float vertices[] = {
		0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
		0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
	   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom left
	   -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,  // first Triangle
	1, 2, 3   // second Triangle
};

const int STRIDE = 8;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	srand(time(NULL));

	//Create vertex buffer, Element Buffer and vertex array object
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//Bind Vertex Array and Vertex Buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Load data into vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Link vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Link color attributes
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	//Unbind Vertex Array and Vertex Buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//Create shader program
	ew::Shader customShader("assets/VertexShader.vert", "assets/FragmentShader.frag");

	unsigned int blackTexture = ew::loadTexture("assets/Sprite-0002.png", GL_REPEAT, GL_NEAREST);
	unsigned int whiteTexture = ew::loadTexture("assets/CoverMod.png", GL_REPEAT, GL_NEAREST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blackTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, whiteTexture);

	customShader.use();
	customShader.setInt("_TextureOne", 0);
	customShader.setInt("_TextureTwo", 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Run Shader program
		customShader.use();

		//Calculate sin value and get the current uniform information
		float timeValue = glfwGetTime();

		float fragColorX = cos(timeValue);

		float fragColorY = cos(timeValue);

		float fragColorZ = cos(timeValue);

		float backgroundColor = sin(timeValue) + 1;

		//Create matrix
		glm::mat4 rotationMatrix = glm::mat4(1.0f);

		//Move to position
		rotationMatrix = glm::translate(rotationMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

		float rotationTime = sin(timeValue);

		//Rotation around z-axis
		//rotationMatrix = glm::rotate(rotationMatrix, rotationTime, glm::vec3(0.0f, 0.0f, 2.0f));

		customShader.setFloat("_FillAmount", fillAmount);

		//Rotate
		//customShader.setMat4("_Rotation", rotationMatrix);

		//Clear framebuffer
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniform to our sin value
		//customShader.setFloat("_Time", timeValue);

		//customShader.setVec3("_FragmentColors", fragColorX, fragColorY, fragColorZ);

		//Bind new Vertex array to update any change
		glBindVertexArray(VAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
		//Draw to screen
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//Start drawing ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// ImGui Settings by Christopher Eichert
		//Create a window called Settings.
		ImGui::Begin("Settings");

		if (ImGui::CollapsingHeader("Shader Settings")) {
			ImGui::SliderFloat("Fill Amount", &fillAmount, 0, 1);
		}

		ImGui::End();

		//Actually render IMGUI elements using OpenGL
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return 0;
}
