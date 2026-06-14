#include<iostream>

#include "shader.h"
#include"../wrapper/checkError.h"

#include<string>//Shader代码需要存储在字符串内
#include<fstream>
#include<sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::cout << "[Ctor] Shader " << this
		<< " vs=" << vertexPath
		<< " fs=" << fragmentPath << std::endl;

	//声明装入shader代码字符串的两个string
    std::string vertexCode;
    std::string fragmentCode;
    //声明用于读取vs跟fs文件的inFileStream
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    //保证ifstream遇到问题的时候可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        //1 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        
        //2 将文件输入流当中的字符串输入到stringStream里面
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        //3 关闭文件
        vShaderFile.close();
        fShaderFile.close();

        //4 将字符串从stringStream中读取出来，转化到code String当中
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR:Shader File Error:" << e.what() << std::endl;
		return;//如果shader文件读取失败，直接返回，后续的shader编译链接等步骤都不执行了
    }

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();
	//创建shader程序
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//为shader程序输入代码
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);



	//执行shader代码编译
	glCompileShader(vertex);
	checkShaderErrors(vertex,"COMPILE");

	glCompileShader(fragment);
	checkShaderErrors(fragment,"COMPILE");

	//创建一个program壳子
	mProgram = glCreateProgram();

	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);

	glLinkProgram(mProgram);

	//检查链接错误
	checkShaderErrors(mProgram, "LINK");


	//清理
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	std::cout << "[Dtor] Shader " << this
		<< " program=" << mProgram << std::endl;

	if (mProgram != 0) {
		glDeleteProgram(mProgram);
		mProgram = 0;
	}
}

void Shader::begin() {
    GL_CALL(glUseProgram(mProgram));
}

void Shader::end() {
    GL_CALL(glUseProgram(0));
}

void Shader::setFloat(const std::string& name, float value) {
	//1 通过名称拿到Uniform变量的位置Location
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	//2 通过Location更新Uniform变量的值
	GL_CALL(glUniform1f(location, value));
}

void Shader::setVector3f(const std::string& name, float v0, float v1, float v2) {
	//1 通过名称拿到Uniform变量的位置Location
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	//2 通过Location更新Uniform变量的值
	GL_CALL(glUniform3f(location, v0, v1, v2));
}

void Shader::setVector3fv(const std::string& name, const float* value) {
	//1 通过名称拿到Uniform变量的位置Location
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	//2 通过Location更新Uniform变量的值
	//第二个参数：当前要更新的uniform变量。如果是数组，数组内包括多少个向量vec3
	GL_CALL(glUniform3fv(location, 1, value));
}
void Shader::setVector3(const std::string& name, const glm::vec3 value) {
	//1 通过名称拿到Uniform变量的位置Location
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	//2 通过Location更新Uniform变量的值
	GL_CALL(glUniform3f(location, value.x, value.y, value.z));
}

void Shader::setInt(const std::string& name, int value) {
	//1 通过名称拿到Uniform变量的位置Location
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	//2 通过Location更新Uniform变量的值
	glUniform1i(location, value);
}
void Shader::setMatrix4x4(const std::string& name, const glm::mat4& value){
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	/*GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));*/
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMatrix3x3(const std::string& name, const glm::mat3& value) {
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}



void Shader::checkShaderErrors(GLuint target, std::string type) {
	int success = 0;
	char infoLog[1024];

	if (type == "COMPILE") {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(target, 1024, NULL, infoLog);
			std::cout << "ERROR: SHADER COMPILE ERROR\n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK") {
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infoLog);
			std::cout << "ERROR: SHADER LINK ERROR\n" << infoLog << std::endl;
		}
	}
	else {
		std::cout << "Error: Check shader errors Type is wrong" << std::endl;
	}

}