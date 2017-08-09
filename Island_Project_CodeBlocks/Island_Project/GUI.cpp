#include "GUI.h"



GUI::GUI()
{
	
}

GUI::GUI(GLuint texture, GLuint& program)
{
	this->width = width;
	this->height = height;
	tex = texture;

	//fill quad vertices
	textureCoords.push_back(vec2(0, 1));
	textureCoords.push_back(vec2(0, 0));
	textureCoords.push_back(vec2(1, 1));
	textureCoords.push_back(vec2(1, 0));

	//fill textureCoords
	vertices.push_back(vec4(-1, 1, 0, 1));
	vertices.push_back(vec4(-1, -1, 0, 1));
	vertices.push_back(vec4(1, 1, 0, 1));
	vertices.push_back(vec4(1, -1, 0, 1));

	loadGUI(program);
}

void GUI::loadGUI(GLuint& program)
{
	glUseProgram(program);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	int size = (vertices.size() * sizeof(vec4));
	int sizeT = (textureCoords.size() * sizeof(vec2));

	//color and vertices go in the same buffer
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, size + sizeT,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, &textureCoords[0]);


	// plumbing
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void GUI::drawGUI(GLuint& program)
{


	GLuint MAmbientLoc, MDiffuseLoc, MSpecularLoc, ShininessLoc, textBoolLoc, texLoc;

	MAmbientLoc = glGetUniformLocation(program, "MAmbient");
	MDiffuseLoc = glGetUniformLocation(program, "MDiffuse");
	MSpecularLoc = glGetUniformLocation(program, "MSpecular");
	ShininessLoc = glGetUniformLocation(program, "Shininess");
	textBoolLoc = glGetUniformLocation(program, "mapText");

	//send over material settings to the shader
	glUniform3fv(MAmbientLoc, 1, vec3(1.0, 1.0, 1.0));
	glUniform3fv(MDiffuseLoc, 1, vec3(1.0, 1.0, 1.0));
	glUniform3fv(MSpecularLoc, 1, vec3(1.0, 1.0, 1.0));
	glUniform1f(ShininessLoc, 1000);
	glUniform1i(textBoolLoc, true);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	 texLoc = glGetUniformLocation(program, "tex");
	glUniform1ui(texLoc, tex);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

GUI::~GUI()
{
}
