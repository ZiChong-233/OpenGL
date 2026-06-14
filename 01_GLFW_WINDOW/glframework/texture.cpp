#include"texture.h"	
#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"//注意要先从framework跳出来

std::map<std::string, Texture*> Texture::mTextureCache{};

void Texture::clearCache() {
	std::cout << "[Texture] clearCache begin, count=" << mTextureCache.size() << std::endl;

	for (auto& pair : mTextureCache) {
		std::cout << "[Texture] clear cached texture path=" << pair.first
			<< " ptr=" << pair.second << std::endl;
		delete pair.second;
	}

	mTextureCache.clear();

	std::cout << "[Texture] clearCache end" << std::endl;
}

Texture* Texture::createTexture(const std::string& path, unsigned int unit) {
	//1 检查是否缓存过本路径对应的纹理对象
	auto iter = mTextureCache.find(path);
	if(iter!= mTextureCache.end()) {
		//对于Iterater（迭代器），使用->second来获取map中对应的value，使用->first来获取key
		return iter->second;
	}

	//2 如果本路径对应的texture没有生成过，则重新生成
	auto texture = new Texture(path, unit);
	mTextureCache[path] = texture;

    return texture;
}

Texture* Texture::createTextureFromMemory(
	const std::string& path,
	unsigned int unit,
	unsigned char* dataIn,
	uint32_t widthIn,
	uint32_t heightIn
) {
	//1 检查是否缓存过本路径对应的纹理对象
	auto iter = mTextureCache.find(path);
	if (iter != mTextureCache.end()) {
		//对于Iterater（迭代器），使用->second来获取map中对应的value，使用->first来获取key
		return iter->second;
	}

	//2 如果本路径对应的texture没有生成过，则重新生成
	auto texture = new Texture(unit, dataIn, widthIn, heightIn);

	mTextureCache[path] = texture;

	return texture;	
}

Texture* Texture::createColorAttachment(unsigned int width, unsigned int height, unsigned int unit) {
	
	return new Texture(width, height, unit);
}

Texture* Texture::createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit) {
	Texture* dsTex = new Texture();

	unsigned int depthStencil;
	glGenTextures(1, &depthStencil);
	glBindTexture(GL_TEXTURE_2D, depthStencil);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//采用最邻近采样（不需要进行混合

	dsTex->mTexture = depthStencil;
	dsTex->mWidth = width;
	dsTex->mHeight = height;
	dsTex->mUnit = unit;

	return dsTex;
}

Texture::Texture() {
	std::cout << "[Ctor] Texture(default) " << this << std::endl;
}

Texture::Texture(const std::string& path, unsigned int unit) {
	std::cout << "[Ctor-Begin] Texture(file) " << this
		<< " path=" << path << " unit=" << unit << std::endl;

	mUnit = unit;
	//1 stbImage 读取图片
	int  channels;

	stbi_set_flip_vertically_on_load(true);//y轴翻转(图片左上角00，OpenGL左下角00)
	unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	//2 生成纹理并且激活单元绑定
	glGenTextures(1, &mTexture);
	//--激活纹理单元--
	glActiveTexture(GL_TEXTURE0 + mUnit);//默认就是GL_TEXTURE0
	//--绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, mTexture);

	//3 传输纹理数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D); //生成mipmap

	//**释放数据
	stbi_image_free(data);

	//设置纹理的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);//修改为mipmap的过滤方式

	//设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v

	std::cout << "[Ctor-End] Texture(file) " << this
		<< " id=" << mTexture << std::endl;
}

Texture::Texture(
	unsigned int unit,
	unsigned char* dataIn,
	uint32_t widthIn,
	uint32_t heightIn
) {
	std::cout << "[Ctor-Begin] Texture(memory) " << this
		<< " unit=" << unit
		<< " widthIn=" << widthIn
		<< " heightIn=" << heightIn
		<< std::endl;

	mUnit = unit;
	//1 stbImage 读取图片
	int  channels;

	stbi_set_flip_vertically_on_load(true);//y轴翻转(图片左上角00，OpenGL左下角00)

	//计算整张图片的大小
	//如果内嵌纹理是jpg、png等压缩格式，height = 0, width 就代表了图片大小***不是特别理解
	uint32_t dataInSize = 0;//图片数据大小，非图片大小
	if (!heightIn) {
		dataInSize = widthIn;
	}
	else {
		//如果内嵌图片不是压缩格式，height = 正常高度, width = 正常宽度
		dataInSize = widthIn * heightIn * 4; //4个通道RGBA 统一认为数据格式都是RGBA
	}

	unsigned char* data = stbi_load_from_memory(dataIn, dataInSize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	//2 生成纹理并且激活单元绑定
	glGenTextures(1, &mTexture);
	//--激活纹理单元--
	glActiveTexture(GL_TEXTURE0 + mUnit);//默认就是GL_TEXTURE0
	//--绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, mTexture);

	//3 传输纹理数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D); //生成mipmap

	//**释放数据
	stbi_image_free(data);

	//设置纹理的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);//修改为mipmap的过滤方式

	//设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v

	std::cout << "[Ctor-End] Texture(memory) " << this
		<< " id=" << mTexture
		<< " size=" << mWidth << "x" << mHeight
		<< std::endl;
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int unit) {
	std::cout << "[Ctor-Begin] Texture(colorAttachment) " << this
		<< " unit=" << unit
		<< " size=" << width << "x" << height
		<< std::endl;

	mWidth = width;
	mHeight = height;
	mUnit = unit;

	glGenTextures(1, &mTexture);
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	std::cout << "[Ctor-End] Texture(colorAttachment) " << this
		<< " id=" << mTexture
		<< std::endl;
}

//传入顺序：右、左、上、下、后、前(+x/-x, +y/-y, +z/-z)
Texture::Texture(const std::vector<std::string>& paths, unsigned int unit) {
	std::cout << "[Ctor-Begin] Texture(cubemap) " << this
		<< " unit=" << unit
		<< " faces=" << paths.size()
		<< std::endl;

	mUnit = unit;
	mTextureTarget = GL_TEXTURE_CUBE_MAP;

	stbi_set_flip_vertically_on_load(false);//这一行的作用是*******************？复位此前可能的在texture中加入的反转

	//先创建CubeMap对象，然后循环读取贴图,设置纹理参数
	glGenTextures(1, &mTexture);
	glActiveTexture(GL_TEXTURE0 + mUnit);//默认就是GL_TEXTURE0
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

	int  channels;
	int width = 0, height = 0;
	unsigned char* data = nullptr;
	for (unsigned int i = 0; i < paths.size(); i++) {
		data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if(data != nullptr) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Error!！: Cubemap texture failed to load at path: " << paths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	//设置纹理的过滤方式
	glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);//修改为mipmap的过滤方式
	//设置纹理的包裹方式
	glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);//v

	std::cout << "[Ctor-End] Texture(cubemap) " << this
		<< " id=" << mTexture
		<< std::endl;
}


Texture::~Texture() {
	std::cout << "[Dtor] Texture " << this
		<< " id=" << mTexture << std::endl;

	if(mTexture != 0){
		glDeleteTextures(1, &mTexture);
		mTexture = 0;//*自动填充的，不一定有用
	}
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(mTextureTarget, mTexture);
}