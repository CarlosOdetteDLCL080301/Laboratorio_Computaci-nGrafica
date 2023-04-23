/*---------------------------------------------------------*/
/* ----------------   Práctica 5 --------------------------*/
/*-----------------    2023-2   ---------------------------*/
/*------------- Alumno: De La Cruz Carlos Odette---------------*/
/*------------- No. de cuenta:317121423---------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow* window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor* monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -15.0f;
float	rotY = 0.0f,
rotX = 0.0f,
rotZ = 0.0f;

float	rotHombro = 0.0f,
rotCodo = 0.0f,

rotDedo1 = 0.0f,
rotDedo1a = 0.0f,

rotDedo2 = 0.0f,
rotDedo2a = 0.0f,

rotDedo3 = 0.0f,
rotDedo3a = 0.0f,

rotDedo4 = 0.0f,
rotDedo4a = 0.0f;


void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat verticesCubo[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		//V0 - Frontal
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, 0.5f, 0.5f,		//V5
		-0.5f, -0.5f, 0.5f,		//V0
		-0.5f, 0.5f, 0.5f,		//V4
		0.5f, 0.5f, 0.5f,		//V5

		0.5f, -0.5f, -0.5f,		//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, -0.5f,		//V7

		-0.5f, 0.5f, 0.5f,		//V4 - Izq
		-0.5f, 0.5f, -0.5f,		//V7
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, -0.5f, 0.5f,		//V0

		0.5f, 0.5f, 0.5f,		//V5 - Der
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		0.5f, -0.5f, -0.5f,		//V2

		-0.5f, 0.5f, 0.5f,		//V4 - Sup
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, 0.5f, -0.5f,		//V6

		-0.5f, -0.5f, 0.5f,		//V0 - Inf
		-0.5f, -0.5f, -0.5f,	//V3
		0.5f, -0.5f, -0.5f,		//V2
		-0.5f, -0.5f, 0.5f,		//V0
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, -0.5f, 0.5f,		//V1
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5 - 20232", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader myShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	myShader.use();

	glm::mat4 viewOp = glm::mat4(1.0f);			//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	glm::mat4 tempA = glm::mat4(1.0f);//Matriz temporal de A
	glm::mat4 tempB = glm::mat4(1.0f);//Matriz temporal de B
	glm::mat4 tempC = glm::mat4(1.0f);//Matriz temporal de C
	glm::mat4 tempD = glm::mat4(1.0f);//Matriz temporal de D
	glm::mat4 tempE = glm::mat4(1.0f);//Matriz temporal de E
	glm::mat4 tempF = glm::mat4(1.0f);//Matriz temporal de F
	glm::mat4 tempG = glm::mat4(1.0f);//Matriz temporal de G
	glm::mat4 tempH = glm::mat4(1.0f);//Matriz temporal de H
	glm::mat4 tempI = glm::mat4(1.0f);//Matriz temporal de I
	glm::mat4 tempJ = glm::mat4(1.0f);//Matriz temporal de J

	//Use "projection" in order to change how we see the information
	projectionOp = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		my_input(window);

		// render
		// Background color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi bloque de dibujo
		/*******************************************/
		glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
		//Use "view" in order to affect all models
		viewOp = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
		viewOp = glm::rotate(viewOp, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

		// pass them to the shaders
		myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		myShader.setMat4("projection", projectionOp);

		//Model
		glBindVertexArray(VAO);

		/*Figura A*/
		modelOp = glm::rotate(glm::mat4(1.0f), glm::radians(rotHombro), glm::vec3(0.0f, 0.0f, 1.0f)	);
		tempA = modelOp = glm::translate(modelOp, glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Figura A

		/*Figura B*/
		modelOp = glm::translate(tempA, glm::vec3(1.0f, 0.0f, 0.0f));//ubicar la posicion de la articulacion
		modelOp = glm::rotate(modelOp, glm::radians(rotCodo), glm::vec3(0.0f, 1.0f, 0.0f));//En que eje rotara la articulacion
		tempB = modelOp = glm::translate(modelOp, glm::vec3(1.25, 0.0f, 0.0f));//Nos moveremos al centro de la siguiente figura
		modelOp = glm::scale(modelOp, glm::vec3(2.5f, 1.0f, 1.0f));//Modelamos la dimension de esta figura
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f));//Damos un color
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Figura B

		//--------------------------------------------------------------------------------------
		/*Figura E*/
		modelOp = glm::translate(tempB, glm::vec3(1.25f, -0.35f, 0.375f));
		modelOp = glm::rotate(modelOp, glm::radians(rotDedo1), glm::vec3(0.0f, 1.0f, 0.0f));
		tempE = modelOp = glm::translate(modelOp, glm::vec3(0.25, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*Figura F*/
		modelOp = glm::translate(tempE, glm::vec3(0.25f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(rotDedo1a), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::translate(modelOp, glm::vec3(0.25, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Figura F


		//--------------------------------------------------------------------------------------
		/*Figura C*/
		modelOp = glm::translate(tempB, glm::vec3(1.25f, 0.35f, 0.375f));//POSICION DE LA ARTICULACIONX
		modelOp = glm::rotate(modelOp, glm::radians(rotDedo2), glm::vec3(0.0f, 0.0f, -1.0f));
		tempC = modelOp = glm::translate(modelOp, glm::vec3(0.25, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*Figura D*/
		modelOp = glm::translate(tempC, glm::vec3(0.25f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(rotDedo2a), glm::vec3(0.0f, 0.0f, -1.0f));
		modelOp = glm::translate(modelOp, glm::vec3(0.375, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.9292f, 0.1f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Figura F

		//--------------------------------------------------------------------------------------
		/*Figura G*/
		modelOp = glm::translate(tempB, glm::vec3(1.25f, 0.35f, 0.0f));//POSICION DE LA ARTICULACION
		modelOp = glm::rotate(modelOp, glm::radians(rotDedo3), glm::vec3(0.0f, 0.0f, -1.0f));
		tempG = modelOp = glm::translate(modelOp, glm::vec3(0.375, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0121212f, 18732.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*Figura H*/
		modelOp = glm::translate(tempG, glm::vec3(0.375f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(rotDedo3a), glm::vec3(0.0f, 0.0f, -1.0f));
		modelOp = glm::translate(modelOp, glm::vec3(0.50, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.12f, 0.1f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//--------------------------------------------------------------------------------------
		/*Figura I*/
		modelOp = glm::translate(tempB, glm::vec3(1.25f, 0.35f, -0.375f));//POSICION DE LA ARTICULACION
		modelOp = glm::rotate(modelOp, glm::radians(rotDedo4), glm::vec3(0.0f, 0.0f, -1.0f));
		tempI = modelOp = glm::translate(modelOp, glm::vec3(0.375, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0121212f, 18732.0f, 0.0123125f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*Figura J*/
		modelOp = glm::translate(tempI, glm::vec3(0.375f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(rotDedo4a), glm::vec3(0.0f, 0.0f, -1.0f));
		modelOp = glm::translate(modelOp, glm::vec3(0.375, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.1999f, 0.974f));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glBindVertexArray(0);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && 90 > rotHombro)
		rotHombro += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && -120 < rotHombro)
		rotHombro -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && 110 > rotCodo)
		rotCodo += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && -3 < rotCodo)
		rotCodo -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && 90 > rotDedo1)
		rotDedo1 += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && -3 < rotDedo1)
		rotDedo1 -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && 90 > rotDedo1a)
		rotDedo1a += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && -3 < rotDedo1a)
		rotDedo1a -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && 90 > rotDedo2)
		rotDedo2 += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && -3 < rotDedo2)
		rotDedo2 -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && 90 > rotDedo2a)
		rotDedo2a += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && -3 < rotDedo2a)
		rotDedo2a -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && 90 > rotDedo3)
		rotDedo3 += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && -3 < rotDedo3)
		rotDedo3 -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && 90 > rotDedo3a)
		rotDedo3a += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && -3 < rotDedo3a)
		rotDedo3a -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && 90 > rotDedo4)
		rotDedo4 += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && -3 < rotDedo4)
		rotDedo4 -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && 90 > rotDedo4a)
		rotDedo4a += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && -3 < rotDedo4a)
		rotDedo4a -= 0.01f;
	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	//2023-1
	glViewport(0, 0, width, height);
}