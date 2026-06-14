import { C, bg, box, footer, text, thinLine, title } from "./theme.mjs";

function node(slide, ctx, x, y, w, h, label, fill, color = C.ink) {
  box(slide, ctx, x, y, w, h, fill, color);
  text(slide, ctx, label, x + 12, y + 8, w - 24, h - 16, { size: 15, bold: true, color, align: "center", valign: "middle" });
}

export async function slide05(presentation, ctx) {
  const slide = presentation.slides.add();
  bg(slide, ctx);
  title(slide, ctx, "MEMORY NOW", "当前已经有释放动作，但生命周期模型还不可靠。");
  node(slide, ctx, 80, 220, 170, 60, "Scene\n析构 Object 树", C.green2, C.green);
  node(slide, ctx, 314, 220, 170, 60, "Object\n递归 delete child", C.green2, C.green);
  node(slide, ctx, 548, 220, 170, 60, "Mesh\ndelete Geometry/Material", C.green2, C.green);
  node(slide, ctx, 782, 184, 170, 60, "Geometry\nglDelete VAO/VBO/EBO", C.green2, C.green);
  node(slide, ctx, 782, 292, 170, 60, "Material\nTexture* 不释放", C.amber2, C.amber);
  node(slide, ctx, 1016, 292, 170, 60, "Texture cache\nclearCache 统一释放", C.amber2, C.amber);
  [250, 484, 718, 952].forEach((x) => thinLine(slide, ctx, x, 248, 64, 4, "#B9AF9E"));
  thinLine(slide, ctx, 718, 320, 64, 4, "#B9AF9E");
  thinLine(slide, ctx, 952, 320, 64, 4, "#B9AF9E");
  box(slide, ctx, 80, 420, 320, 92, "#FFFFFF", "#D1C9B8");
  text(slide, ctx, "做得对", 108, 440, 120, 24, { size: 19, bold: true, color: C.green });
  text(slide, ctx, "VAO/VBO/EBO、Shader program、Texture id、FBO 都有析构释放路径。", 108, 472, 250, 34, { size: 15, color: C.ink });
  box(slide, ctx, 480, 420, 320, 92, "#FFFFFF", "#D1C9B8");
  text(slide, ctx, "主要问题", 508, 440, 120, 24, { size: 19, bold: true, color: C.red });
  text(slide, ctx, "谁拥有 Texture、谁只是引用，还没有由类型系统表达。", 508, 472, 250, 34, { size: 15, color: C.ink });
  box(slide, ctx, 880, 420, 306, 92, "#FFFFFF", "#D1C9B8");
  text(slide, ctx, "核心改法", 908, 440, 120, 24, { size: 19, bold: true, color: C.blue });
  text(slide, ctx, "RAII + handle/resource manager + 绑定状态外置。", 908, 472, 236, 34, { size: 15, color: C.ink });
  footer(slide, ctx, 5);
  return slide;
}
