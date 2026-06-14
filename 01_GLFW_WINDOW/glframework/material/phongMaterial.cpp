#include "phongMaterial.h"


PhongMaterial::PhongMaterial() {
	mType = MaterialType::PhongMaterial;
	std::cout << "[Ctor] PhongMaterial " << this << std::endl;
}
PhongMaterial::~PhongMaterial() {
	std::cout << "[Dtor] PhongMaterial " << this << std::endl;
    // 当前暂不在此处释放 mDiffuse / mSpecularMask。
   // 原因：
   // 1. Texture* 的来源尚未统一，可能来自显式 new、texture cache 或其他共享路径；
   // 2. 在 owner 规则未最终固定前，贸然 delete 可能引入 double delete 或悬空指针。
   //
   // 未来方案：
   // - 若确认该材质实例独占这些 Texture*，则在此处负责 delete；
   // - 若贴图来自 Texture::createTexture(...) 的缓存系统，则由 Texture::clearCache() 统一释放；
   // - 若贴图来自 Framebuffer 附件，则应由 Framebuffer 负责释放
   // - 可能引入智能指针（如 std::shared_ptr<Texture>）以自动管理生命周期，避免手动 delete 的复杂性。
}