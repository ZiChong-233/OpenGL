import { C, bg, box, footer, pill, text, thinLine } from "./theme.mjs";

export async function slide01(presentation, ctx) {
  const slide = presentation.slides.add();
  bg(slide, ctx, C.charcoal);
  text(slide, ctx, "OPENGL PROJECT ASSESSMENT", 68, 54, 520, 28, { size: 15, bold: true, color: C.amber2, valign: "middle" });
  text(slide, ctx, "你的项目已经具备小型渲染框架骨架，下一道门槛是资源生命周期。", 68, 124, 910, 112, { size: 42, bold: true, color: C.white, valign: "middle" });
  text(slide, ctx, "窗口与输入、场景树、几何、材质、Shader、纹理、Assimp、Renderer、FBO 与 ImGui 均已成形。当前核心风险不是功能缺口，而是裸指针所有权、共享纹理状态和 GPU 资源复制风险。", 72, 264, 780, 94, { size: 22, color: "#E7E2D8" });
  box(slide, ctx, 908, 108, 252, 324, "#35434B", "#5D6A72");
  text(slide, ctx, "总体判断", 936, 134, 196, 28, { size: 18, bold: true, color: C.amber2, align: "center", valign: "middle" });
  thinLine(slide, ctx, 940, 180, 188, 2, C.amber);
  text(slide, ctx, "可运行框架", 936, 204, 196, 34, { size: 24, bold: true, color: C.white, align: "center" });
  text(slide, ctx, "但还不是成熟引擎", 936, 246, 196, 34, { size: 18, color: "#E8EAED", align: "center" });
  pill(slide, ctx, "内存释放成熟度：2/5", 934, 312, 200, C.amber2, C.charcoal);
  pill(slide, ctx, "优先优化：所有权模型", 934, 358, 200, C.green2, C.charcoal);
  text(slide, ctx, "交付物：Word 评估报告 + 项目概况 PPT", 68, 588, 760, 28, { size: 18, color: "#E7E2D8" });
  text(slide, ctx, "2026-05-25", 1010, 588, 150, 28, { size: 18, color: "#E7E2D8", align: "right" });
  footer(slide, ctx, 1);
  return slide;
}
