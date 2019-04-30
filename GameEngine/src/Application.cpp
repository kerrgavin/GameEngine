#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Character.h"
#include "Line.h"
#include "TLine.h"
#include "Poly.h"
#include "BSP.h"

struct DrawElement {
	std::vector<float> points;
	std::vector<unsigned int> indicies;
	double minX;
	double maxX;
	std::string texture;
};

DrawElement genVBO(const std::vector<double>& v, const std::string texture);
float genTexCoord(const float& num, const float& min, const float& max);

bool lengthCovered(std::vector<Vec> lengths) {
	double covered = 0;
	for (int i = 1; i < lengths.size(); i++) {
		if (lengths[i].getX() >= -1 && lengths[i].getY() <= 1) {
			covered += lengths[i].getY() - lengths[i].getX();
		}
		else if (lengths[i].getX() < -1) {
			covered += lengths[i].getY() - (-1);
		}
		else if (lengths[i].getY() > 1) {
			covered += 1 - lengths[i].getX();
		}
	}
	std::cout << "Coverd?: " << (covered >= 2) << std::endl;
	return covered >= 2;
}

std::vector<Vec> addLength(std::vector<Vec> lengths, Vec length) {
	cout << "New Length: " << length << endl;
	for (int i = 0; i < lengths.size(); i++) {
		if (lengths[i].getX() > length.getX() || lengths[i].getY() < length.getY()) {
			Vec temp = Vec(lengths[i]);
			lengths.erase(lengths.begin() + i);
			if (temp.getX() > length.getX() && temp.getY() < length.getY()) {
				return addLength(lengths, length);
			}
			else if (temp.getX() > length.getX()) {
				return addLength(lengths, Vec(length.getX(), temp.getY()));
			}
			else if (temp.getY() < length.getY()) {
				return addLength(lengths, Vec(temp.getX(), length.getY()));
			}
		}
	}
	lengths.push_back(length);
	return lengths;
}

shared_ptr<BSPNode> goRight(shared_ptr<BSPNode> node, BSPTree tree) {
	for (int i = 0; i < tree.getLeafs().size(); i++) {
		
		if (tree.getLeafs()[i] == node) {
			if (i == 0) {
				return nullptr;
			}
			return tree.getLeafs()[i - 1];
		}
	}
	return nullptr;
}

shared_ptr<BSPNode> goLeft(shared_ptr<BSPNode> node, BSPTree tree) {
	for (int i = 0; i < tree.getLeafs().size(); i++) {
		
		if (tree.getLeafs()[i] == node) {
			if (i == tree.getLeafs().size() - 1) {
				return nullptr;
			}
			return tree.getLeafs()[i + 1];
		}
	}
	return nullptr;
}

vector<DrawElement> makeStack(BSPTree tree, shared_ptr<BSPNode> node, Character c) {
	std::vector<DrawElement> elements;
	std::vector<Vec> lengths;

	while (!lengthCovered(lengths) && node != nullptr) {
		Poly temp = node->getPolygons()[0];
		std::cout << "Working Polygon: \n" << temp << temp.getVertices().size() << std::endl;
		std::cout << "Verticies: " << c.genPoly(temp, temp.getFloorHeight()).size() << std::endl;
		elements.push_back(genVBO(c.genPoly(temp, temp.getFloorHeight()), temp.getFloor()));
		elements.push_back(genVBO(c.genPoly(temp, temp.getCeilingHeight()), temp.getCeiling()));
		for (auto const &i : temp.getLines()) {
			std::cout << "Working On lines" << std::endl;
			if (c.inView(i)) {
				cout << "Middle?: " << i.getMiddle().compare("") << endl;
				if (i.getMiddle().compare("") != 0) {
					elements.push_back(genVBO(c.genWall(i, temp.getFloorHeight(), temp.getCeilingHeight()), i.getMiddle()));
					std::cout << "Adding length" << std::endl;
					std::cout << "Adding Line: " << i << std::endl;
					lengths = addLength(lengths, Vec(elements[elements.size() - 1].minX, elements[elements.size() - 1].maxX));
				}
			}
		}
		if (c.getAngleZ() >= 0 && c.getAngleZ() <= M_PI) {
			node = goLeft(node, tree);
		}
		else {
			node = goRight(node, tree);
		}
	}
	return elements;
}

DrawElement genVBO(const std::vector<double>& v, const std::string texture) {
	vector<float> points;
	vector<unsigned int> indicies;
	float minX = (float)v[0];
	float maxX = (float)v[0];
	float minY = (float)v[1];
	float maxY = (float)v[1];

	for (int i = 0; i < v.size(); i++) {
		if (i % 2 == 0) {
			if (v[i] < minX) {
				minX = v[i];
			}
			if (v[i] > maxX) {
				maxX = v[i];
			}
		}
		else {
			if (v[i] < minY) {
				minY = v[i];
			}
			if (v[i] > maxY) {
				maxY = v[i];
			}
		}
	}

	//std::cout << minX << " " << maxX << " " << minY << " " << maxY << std::endl;

	for (int i = 0; i < v.size() - 1; i += 2) {
		points.push_back(v[i]);
		points.push_back(v[i + 1]);
		points.push_back(genTexCoord((float)v[i], minX, maxX));
		points.push_back(genTexCoord((float)v[i + 1], minY, maxY));
	}

	for (int i = 0; i < v.size() / 4; i++) {
		indicies.push_back(i * 2);
		indicies.push_back((i * 2) + 1);
		if (i != (v.size() / 4) - 1 || v.size() / 4 == 1) {
			indicies.push_back((i * 2) + 2);
		}
		else {
			indicies.push_back(0);
		}
	}

	return { points, indicies, minX, maxX, texture };
}

float genTexCoord(const float& num, const float& min, const float& max) {
	std::cout << "Generating Text Coord: " << num << "\t" << min << "\t" << max << std::endl;
	return (num - min) / (max - min);
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << "\n";
	{
		Character c;
		c.setHeight(1);
		c.setPosition(1.5, 1, 1);
		c.setDirection(M_PI_2, 0);

		TLine l1(0, 0, 0, 3, "wall", "", "");
		TLine l2(0, 3, 3, 3);
		TLine l3(3, 3, 3, 0, "wall", "", "");
		TLine l4(3, 0, 0, 0, "wall", "", "");

		Poly p1;
		p1.addLine(l1);
		p1.addLine(l2);
		p1.addLine(l3);
		p1.addLine(l4);
		p1.setFloorHeight(0);
		p1.setCeilingHeight(3);
		p1.setCeiling("ceiling");
		p1.setFloor("floor");

		TLine l5(0, 3, 1.5, 4.5, "wall", "", "");
		TLine l6(1.5, 4.5, 3, 3);
		TLine l7(3, 3, 0, 3);

		Poly p2;
		p2.addLine(l5);
		p2.addLine(l6);
		p2.addLine(l7);
		p2.setFloorHeight(0);
		p2.setCeilingHeight(3);
		p2.setCeiling("ceiling");
		p2.setFloor("floor");

		TLine l8(1.5, 3.5, 6, 4.5, "wall", "", "");
		TLine l9(6, 4.5, 6, 3, "wall", "", "");
		TLine l10(6, 3, 3, 3, "wall", "", "");
		TLine l11(3, 3, 1.5, 4.5);

		Poly p3;
		p3.addLine(l8);
		p3.addLine(l9);
		p3.addLine(l10);
		p3.addLine(l11);
		p3.setFloorHeight(0);
		p3.setCeilingHeight(3);
		p3.setCeiling("ceiling");
		p3.setFloor("floor");

		p1.genVertices();
		p2.genVertices();
		p3.genVertices();

		cout << p1 << "\n";
		cout << p2 << "\n";
		cout << p3 << "\n";

		BSPTree tree;
		shared_ptr<BSPNode> root = make_shared<BSPNode>();
		root->addPolygon(p1);
		root->addPolygon(p2);
		root->addPolygon(p3);
		tree.setRoot(root);

		BSPTree::generateTree(tree.getRoot());

		shared_ptr<BSPNode> found = tree.findCharacter(tree.getRoot(), c);

		vector<DrawElement> elements = makeStack(tree, found, c);

		std::cout << "Number of elements to draw: " << elements.size() << std::endl;
		for (const auto& draw : elements) {
			cout << "Points: " << draw.texture << endl;
			for (int i = 0; i < draw.points.size(); i++) {
				std::cout << draw.points[i];
				i += 1;
				std::cout << " " << draw.points[i];
				i += 1;
				std::cout << " " << draw.points[i];
				i += 1;
				std::cout << " " << draw.points[i] << std::endl;
			}
		}

		for (const auto& draw : elements) {
			cout << "Indicies: " << draw.texture << endl;
			for (int i = 0; i < draw.indicies.size(); i++) {
				std::cout << draw.indicies[i];
				i += 1;
				std::cout << " " << draw.indicies[i];
				i += 1;
				std::cout << " " << draw.indicies[i] << std::endl;
			}
		}
		/*std::cout << "\nChecking" << std::endl;
		for (int i = 0; i < draw.points.size(); i++) {
			std::cout << draw.points[i];
			i += 1;
			std::cout << " " << draw.points[i];
			i += 1;
			std::cout << " " << draw.points[i];
			i += 1;
			std::cout << " " << draw.points[i] << std::endl;
		}

		for (int i = 0; i < draw.indicies.size(); i++) {
			std::cout << draw.indicies[i];
			i += 1;
			std::cout << " " << draw.indicies[i];
			i += 1;
			std::cout << " " << draw.indicies[i] << std::endl;
		}*/

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		/*VertexArray va;
		VertexBuffer vb(&draw.points[0], draw.points.size() * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(&draw.indicies[0], 6);
*/
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		//glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float)480 / (float)640, 0.1f, 100.0f);

		//glm::mat4 View = glm::lookAt(
		//	glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		//	glm::vec3(0, 0, 0), // and looks at the origin
		//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		//);

		//glm::mat4 Model = glm::mat4(1.0f);

		//glm::mat4 mvp = Projection * View * Model;

		//shader.SetUniform4f("u_MVP", mvp);

		Texture texture1("res/textures/floor.png");
		Texture texture2("res/textures/ceiling.png");
		Texture texture3("res/textures/wall.png");
		texture1.Bind();

		shader.SetUniform1i("u_Texture", 0);

		//va.Unbind();
		shader.Unbind();
		//vb.Unbind();
		//ib.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			//elements = makeStack(tree, found, c);
			while(elements.size() > 0) {
				DrawElement draw = elements[elements.size()-1];
				VertexArray va;
				VertexBuffer vb(&draw.points[0], draw.points.size() * 4 * sizeof(float));
				VertexBufferLayout layout;
				layout.Push<float>(2);
				layout.Push<float>(2);
				va.AddBuffer(vb, layout);

				IndexBuffer ib(&draw.indicies[0], draw.indicies.size());
				texture1.Unbind();
				texture2.Unbind();
				texture3.Unbind();

				if (draw.texture.compare("wall") == 0) {
					texture3.Bind();
				}
				if (draw.texture.compare("floor") == 0) {
					texture1.Bind();
				}
				if (draw.texture.compare("ceiling") == 0) {
					texture2.Bind();
				}

				shader.Bind();
				shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

				va.Bind();

				ib.Bind();

				renderer.Draw(va, ib, shader);
				//glDrawArrays(GL_TRIANGLES, 0, 6);
				elements.pop_back();
				va.Unbind();
				ib.Unbind();
			}
			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}

			r += increment;
			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}
	}
	glfwTerminate();
	return 0;
}