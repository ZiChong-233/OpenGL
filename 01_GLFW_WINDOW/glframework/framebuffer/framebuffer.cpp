#include "framebuffer.h"

Framebuffer::Framebuffer(unsigned int width, unsigned int height) {
	mWidth = width;
	mHeight = height;
	//1 生成FBO对象并且绑定
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	//2 生成颜色附件并加入FBO
	mColorAttachment = Texture::createColorAttachment(mWidth, mHeight, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment->getTexture(), 0);

	//3 生成depth Stencil附件并加入FBO
	mDepthStencilAttachment = Texture::createDepthStencilAttachment(mWidth, mHeight, 1);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthStencilAttachment->getTexture(), 0);

	//检查当前构建的FBO是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//解绑FBO，回到默认FBO
}


Framebuffer::~Framebuffer() {
	if (mFBO) {
		glDeleteFramebuffers(1, &mFBO);
	}
	if(mColorAttachment!= nullptr){
		delete mColorAttachment;
	}
	if(mDepthStencilAttachment != nullptr){
		delete mDepthStencilAttachment;
	}
}