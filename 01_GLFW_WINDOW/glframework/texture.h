#pragma once 
#include"core.h"
#include<string>

class Texture {
public:
	//从硬盘读取文件
	static Texture* createTexture(const std::string& path, unsigned int unit);
	//从内存读取数据
	static Texture* createTextureFromMemory(
		const std::string& path, 
		unsigned int unit,
		unsigned char* dataIn,
		uint32_t widthIn,
		uint32_t heightIn
	);
	static Texture* createColorAttachment(unsigned int width, unsigned int height, unsigned int unit);
	static Texture* createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit);

	static void clearCache();

	Texture();
	Texture(const std::string& path,unsigned int unit);
	Texture(
		unsigned int unit,
		unsigned char* dataIn,
		uint32_t widthIn,
		uint32_t heightIn
	);
	Texture(unsigned int width, unsigned int height, unsigned int unit);
	Texture(const std::vector<std::string>& paths, unsigned int unit);

	~Texture();
	void bind();
	void setUnit(unsigned int unit) { mUnit = unit; }	

	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }
	GLuint getTexture()const { return mTexture; }//做成const类型，在const环境下也可以调用

private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
	int mHeight{ 0 };
	unsigned int mUnit{ 0 };
	unsigned int mTextureTarget{ GL_TEXTURE_2D };

	static std::map<std::string, Texture*> mTextureCache;//纹理缓存
};