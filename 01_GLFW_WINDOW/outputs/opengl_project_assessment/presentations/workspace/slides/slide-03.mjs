import { C, bg, bar, box, footer, text, title } from "./theme.mjs";

export async function slide03(presentation, ctx) {
  const slide = presentation.slides.add();
  bg(slide, ctx);
  title(slide, ctx, "COMPLETED", "已完成内容覆盖了渲染框架的主干能力。");
  box(slide, ctx, 74, 194, 604, 388, "#FFFFFF", "#D1C9B8");
  text(slide, ctx, "能力成熟度快照", 106, 222, 260, 30, { size: 22, bold: true, color: C.ink });
  bar(slide, ctx, "窗口与输入", 3, 112, 284, 260, C.blue);
  bar(slide, ctx, "相机控制", 3, 112, 334, 260, C.green);
  bar(slide, ctx, "场景树", 2.5, 112, 384, 260, C.green);
  bar(slide, ctx, "材质/Shader", 3, 112, 434, 260, C.amber);
  bar(slide, ctx, "模型/纹理加载", 2.5, 112, 484, 260, C.red);
  box(slide, ctx, 728, 194, 480, 388, "#FFFFFF", "#D1C9B8");
  text(slide, ctx, "已经具备的实际能力", 760, 222, 280, 30, { size: 22, bold: true, color: C.ink });
  const items = [
    ["Renderer", "按 MaterialType 选择 Shader，并设置深度、模板、混合、面剔除状态。"],
    ["Assimp", "递归解析节点、Mesh、贴图和局部变换。"],
    ["Texture", "支持文件、内存、Cubemap、FBO attachment 和路径缓存。"],
    ["Debug", "已有 ImGui 面板和构造/析构日志。"],
  ];
  items.forEach((it, idx) => {
    const y = 282 + idx * 68;
    text(slide, ctx, it[0], 760, y, 110, 24, { size: 17, bold: true, color: [C.blue, C.green, C.amber, C.red][idx] });
    text(slide, ctx, it[1], 882, y - 2, 282, 42, { size: 14, color: C.ink });
  });
  footer(slide, ctx, 3);
  return slide;
}
