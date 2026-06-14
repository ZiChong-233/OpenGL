import { C, bg, box, footer, text, title } from "./theme.mjs";

function row(slide, ctx, y, p, action, check, color, fill) {
  box(slide, ctx, 92, y, 108, 60, fill, color);
  text(slide, ctx, p, 92, y + 14, 108, 30, { size: 21, bold: true, color, align: "center", valign: "middle" });
  box(slide, ctx, 220, y, 442, 60, "#FFFFFF", "#D1C9B8");
  text(slide, ctx, action, 244, y + 12, 390, 34, { size: 16, bold: true, color: C.ink, valign: "middle" });
  box(slide, ctx, 682, y, 506, 60, "#FFFFFF", "#D1C9B8");
  text(slide, ctx, check, 706, y + 12, 458, 34, { size: 15, color: C.ink, valign: "middle" });
}

export async function slide08(presentation, ctx) {
  const slide = presentation.slides.add();
  bg(slide, ctx);
  title(slide, ctx, "NEXT", "下一步的验收标准应当是资源可追踪、可释放、可复现。");
  text(slide, ctx, "优先级", 112, 188, 80, 24, { size: 15, bold: true, color: C.muted, align: "center" });
  text(slide, ctx, "建议事项", 244, 188, 160, 24, { size: 15, bold: true, color: C.muted });
  text(slide, ctx, "验收口径", 706, 188, 160, 24, { size: 15, bold: true, color: C.muted });
  row(slide, ctx, 224, "P0", "禁用 GL 资源类拷贝；拆掉共享 Texture 的 unit 状态。", "不会因为值拷贝产生重复 glDelete；材质复用纹理时绑定单元互不串扰。", C.red, C.red2);
  row(slide, ctx, 304, "P0", "确定 Texture / Material / Framebuffer 的所有权合同。", "所有裸指针都能说明 owned 或 borrowed，并逐步替换为 handle / unique_ptr。", C.red, C.red2);
  row(slide, ctx, 384, "P1", "修复 Assimp 失败泄漏、Application 窗口销毁和调试材质替换方式。", "失败路径和退出路径不残留 CPU 对象或 GL 对象。", C.amber, C.amber2);
  row(slide, ctx, 464, "P2", "接入 ResourceTracker 与 ImGui 资源面板。", "能看到 Texture、Shader、Geometry、FBO 当前数量和退出泄漏报告。", C.green, C.green2);
  box(slide, ctx, 92, 580, 1096, 42, C.charcoal, C.charcoal);
  text(slide, ctx, "最终目标：从“依靠记忆释放资源”升级为“类型和工具共同保证生命周期”。", 112, 588, 1056, 24, { size: 18, bold: true, color: C.white, align: "center", valign: "middle" });
  footer(slide, ctx, 8);
  return slide;
}
