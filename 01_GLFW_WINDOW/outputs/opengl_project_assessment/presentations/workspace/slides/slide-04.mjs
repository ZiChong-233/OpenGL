import { C, bg, box, footer, text, title } from "./theme.mjs";

function riskCard(slide, ctx, x, y, heading, body, color, fill, tag) {
  box(slide, ctx, x, y, 536, 116, fill, color);
  text(slide, ctx, tag, x + 22, y + 18, 62, 24, { size: 15, bold: true, color, align: "center", valign: "middle", fill: "#FFFFFF", line: ctx.line(color, 1), insets: { left: 0, right: 0, top: 0, bottom: 0 } });
  text(slide, ctx, heading, x + 104, y + 18, 390, 26, { size: 20, bold: true, color: C.ink });
  text(slide, ctx, body, x + 104, y + 52, 390, 42, { size: 15, color: C.ink });
}

export async function slide04(presentation, ctx) {
  const slide = presentation.slides.add();
  bg(slide, ctx);
  title(slide, ctx, "GAPS", "欠缺项主要是引擎工程化，而不是 OpenGL API 数量。");
  riskCard(slide, ctx, 72, 190, "所有权不由类型系统保证", "Scene、Mesh、Material、Texture、Renderer 大量依赖裸指针约定。", C.red, C.red2, "P0");
  riskCard(slide, ctx, 672, 190, "Texture 兼任资源和绑定状态", "共享 Texture 被 setUnit 修改，多个材质复用时容易互相影响。", C.red, C.red2, "P0");
  riskCard(slide, ctx, 72, 336, "GPU 句柄对象默认可拷贝", "Geometry、Texture、Shader、Framebuffer 持有 GL id，但没有禁用 copy。", C.amber, C.amber2, "P0");
  riskCard(slide, ctx, 672, 336, "失败路径和验证工具不足", "模型加载失败、构建复现、泄漏报告、资源计数还没有形成体系。", C.amber, C.amber2, "P1");
  box(slide, ctx, 72, 512, 1136, 56, "#FFFFFF", "#D1C9B8");
  text(slide, ctx, "判断：先把生命周期规则补稳，再扩展 PBR、阴影、后处理等高层功能。否则功能越多，释放责任越难追。", 102, 526, 1076, 30, { size: 18, bold: true, color: C.ink, align: "center", valign: "middle" });
  footer(slide, ctx, 4);
  return slide;
}
