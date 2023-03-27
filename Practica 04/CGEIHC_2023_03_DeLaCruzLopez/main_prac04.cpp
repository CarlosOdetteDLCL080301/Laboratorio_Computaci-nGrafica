/*---------------------------------------------------------*/
/* ----------------   Práctica 4 --------------------------*/
/*-----------------    2023-2   ---------------------------*/
/*------------- Alumno:De La Cruz López Carlos Odette---------------*/
/*------------- No. de cuenta:317121423		---------------*/
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
		movZ = -5.0f;

float	rotX = 0.0f,
		rotY = 0.0f,
		rotZ = 0.0f;

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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4 2023-2", NULL, NULL);
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
	glm::mat4 cuelloTemp = glm::mat4(1.0f);
	glm::mat4 brazoIzquierdo = glm::mat4(1.0f);
	glm::mat4 brazoDerecho = glm::mat4(1.0f);
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
		viewOp = glm::rotate(viewOp,glm::radians(rotX),glm::vec3(1.0f,0.0f,0.0f));		//Esta variable lo que hace es "reescribir" su rotación en el eje X
		viewOp = glm::rotate(viewOp, glm::radians(rotY),glm::vec3(0.0f, 1.0f, 0.0f));	//Esta variable lo que hace es "reescribir" su rotación en el eje Y
		viewOp = glm::rotate(viewOp, glm::radians(rotZ),glm::vec3(0.0f, 0.0f, 1.0f));	//Esta variable lo que hace es "reescribir" su rotación en el eje Z
		// pass them to the shaders
		myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		myShader.setMat4("projection", projectionOp);

		//Model
		glBindVertexArray(VAO);
		modelOp = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f,2.5f,1.0f));//Refiere una escala que desea, nos ayudara a cambiar el tamaño de nuestro objetos para calcularlo se usa la siguiente formula "Factores de escala" = "valor final"/"valor inicial"
		myShader.setMat4("model",modelOp);//Esta operación hará la modificación de nuestra figura que recibe, la linea anterior, solo hace los calculos, este es el que nos lo representa //Si no queremos que la escala se vea afectada por alguna modificación, solo sería colocar un 1 para que este no lo modifique, en caso de poner un 0, eliminaria la componente y quedaría una figura bimensional//Se recomienda que la escala sea lo ultimo en ser modificadó esto es porque...
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Este cubo representa la cabeza (tomando referencia del profesor)

		//modelOp = glm::translate(modelOp, glm::vec3(0.0f/1.5f, -1.5f/2.5f, 0.0f));//Este error lo solucionaba que nos daba la posición muy abajo de su posición que debía tener, pero era la menos eficiente, ya que al tener mas figuras modeladas, tendría que hacer divisiones y es un coste mas grande se hará lo siguiente
		//modelOp = glm::scale(modelOp, glm::vec3(0.5f/1.5f, 0.5f/2.5f, 1.0f));
		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));//Es mejor volver a definir una matriz para ya no realizar divisiones, y mejor trabjamos desde una nueva figura(matriz) para no estar considerando el obtener una vez mas la escala necesaria tomando como referencia la figura anterior
		cuelloTemp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));//Esto guardará el centro del cuello sin guardar la escala que se le aplica a la figura
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.5f, 1.0f));
		myShader.setMat4("model",modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));//Esto cambia el color
		glDrawArrays(GL_TRIANGLES, 0, 36); //Este cubo representa el cuello (tomando referencia del profesor)
		
		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.25f, 0.0f));//Esta opción es considerando el modelado desde el punto de origen
		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.25f, 0.0f));//Esta opción hará una traslación tomando como referencia a nuestro cuello
		modelOp = glm::translate(cuelloTemp, glm::vec3(0.0f, -2.75f, 0.0f));//En esta modificación estamos asignando la ubicación de nuestro torso, considerando como referencia a nuestro cuello, solo se requiero de crear variables temporales para poder operar las traslaciones desde ahí, sin ser afectados por las escala
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 5.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.45f, 0.1f));//Esto cambia el color
		glDrawArrays(GL_TRIANGLES, 0, 36); //Este cubo representa el pecho (tomando referencia del profesor)

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-2.75f, -2.0f, 0.0f));//En esta modificación estamos asignando la ubicación de nuestro torso, considerando como referencia a nuestro cuello, solo se requiero de crear variables temporales para poder operar las traslaciones desde ahí, sin ser afectados por las escala
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.77f, 0.26f, 0.1f));//Esto cambia el color
		glDrawArrays(GL_TRIANGLES, 0, 36); //Este cubo representa el hombro izquierdo (tomando referencia del profesor)

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.75f, -2.0f, 0.0f));//En esta modificación estamos asignando la ubicación de nuestro torso, considerando como referencia a nuestro cuello, solo se requiero de crear variables temporales para poder operar las traslaciones desde ahí, sin ser afectados por las escala
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.77f, 0.26f, 0.1f));//Esto cambia el color
		glDrawArrays(GL_TRIANGLES, 0, 36); //Este cubo representa el hombro DERECHO (tomando referencia del profesor)


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
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 1.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//Cada que detecta que se presiona la tecla A, genera la acción en IF
		movZ -= 1.01f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//Cada que detecta que se presiona la tecla A, genera la acción en IF
		movZ += 1.01f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX-= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 1.08f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY += 1.08f;
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		rotZ -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
		rotZ += 1.08f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}