import { C, bg, box, footer, text, thinLine, title } from "./theme.mjs";

function milestone(slide, ctx, x, y, num, heading, body, color) {
  box(slide, ctx, x, y, 250, 92, "#FFFFFF", color);
  text(slide, ctx, num, x + 16, y + 16, 44, 44, { size: 22, bold: true, color: C.white, align: "center", valign: "middle", fill: color, line: ctx.line(color, 1), insets: { left: 0, right: 0, top: 0, bottom: 0 } });
  text(slide, ctx, heading, x + 72, y + 14, 154, 24, { size: 17, bold: true, color });
  text(slide, ctx, body, x + 72, y + 44, 150, 32, { size: 13, color: C.ink });
}

export async function slide07(presentation, ctx) {
  const slide = presentation.slides.add();
  bg(slide, ctx);
  title(slide, ctx, "ROADMAP", "优化方案应该先收敛生命周期，再扩展渲染功能。");
  thinLine(slide, ctx, 128, 348, 1024, 6, "#C9BEAA");
  milestone(slide, ctx, 80, 190, "0", "资源清单", "标注 owned / borrowed，整理 shutdown 顺序。", C.blue);
  milestone(slide, ctx, 374, 190, "1", "禁用拷贝", "GL 句柄类 delete copy，必要时实现 move。", C.red);
  milestone(slide, ctx, 668, 190, "2", "修失败路径", "Assimp 加载失败、窗口销毁、单例释放。", C.amber);
  milestone(slide, ctx, 962, 190, "3", "拆纹理单元", "bind(unit)，不把 unit 存在共享对象里。", C.green);
  milestone(slide, ctx, 80, 430, "4", "材质 handle", "Material 持有 TextureHandle / shared_ptr。", C.green);
  milestone(slide, ctx, 374, 430, "5", "Scene unique_ptr", "Object children 与 Mesh 资源独占所有权。", C.blue);
  milestone(slide, ctx, 668, 430, "6", "资源管理器", "统一缓存、卸载、reload 与泄漏报告。", C.amber);
  milestone(slide, ctx, 962, 430, "7", "压力测试", "重复加载卸载、FBO resize、退出清零。", C.red);
  footer(slide, ctx, 7);
  return slide;
}
