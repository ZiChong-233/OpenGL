#pragma once
#include "../core.h"

enum class MaterialType {
	PhongMaterial,
	WhiteMaterial,
	DepthMaterial,
	OpacityMaskMaterial,
	ScreenMaterial,
	CubeMaterial,
	PhongEnvMaterial
};

class Material {
public:
	Material();
	virtual ~Material();//只保证多态delete安全，无法确认在material基类需要释放什么资源
public:
	MaterialType mType;

	//深度检测相关
	bool	mDepthTest{ true };
	GLenum	mDepthFunc{ GL_LEQUAL };
	bool	mDepthWrite{ true };

	//PolygonOffset相关
	bool mPolygonOffsetEnable{ false };
	unsigned int mPolygonOffsetType{ GL_POLYGON_OFFSET_FILL };
	float mPolygonOffsetFactor{ 0.0f };
	float mPolygonOffsetUnits{ 0.0f };

	//stencil相关
	//glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//glStencilMask(0xFF);//保证模版缓冲可以被清理
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);//如何测试

	bool mStencilTestEnable{ false };//是否开启模版测试
	unsigned int mSFail{ GL_KEEP };//模板测试失败
	unsigned int mZFail{ GL_KEEP };//模板测试通过但是深度检测没通过
	unsigned int mZPass{ GL_KEEP };//模版+深度测试都通过
	unsigned int mStencilMask{ 0xFF };//模版缓冲写入掩码

	unsigned int mStencilFunc{ GL_ALWAYS };//模版测试函数	
	unsigned int mStencilRef{ 0 };//参考值
	unsigned int mStencilFuncMask{ 0xFF };//模版测试掩码

	//颜色混合
	bool mBlend{ false };
	unsigned int mSFactor{ GL_SRC_ALPHA };	
	unsigned int mDFactor{ GL_ONE_MINUS_SRC_ALPHA };

	float mOpacity{ 1.0f };//透明度

	//Face Culling
	bool mFaceCullingEnable{ false };
	unsigned int mFrontFace{ GL_CCW };//定义正面
	unsigned int mCullFace{ GL_BACK };//剔除哪一面
};