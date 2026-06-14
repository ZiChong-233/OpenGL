#pragma once
#include "core.h"


class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void begin();
	void end();

	void setFloat(const std::string& name, float value);//用于设置一个float类型的uniform变量
	void setVector3f(const std::string& name, float v0, float v1, float v2);//用于设置一个vec3类型的uniform变量
	void setVector3fv(const std::string& name, const float* value);
	void setVector3(const std::string& name, const glm::vec3 value);

	void setInt(const std::string& name, int value);//用于设置一个int类型的uniform变量

	void setMatrix4x4(const std::string& name, const glm::mat4& value);
	void setMatrix3x3(const std::string& name, const glm::mat3& value);


private:
	//shader program
	//type:COMPILELINK
	void checkShaderErrors(GLuint target,std::string type);
private:
	GLuint mProgram = 0;
};