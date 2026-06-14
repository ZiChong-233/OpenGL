import { C, bg, box, footer, text, title, thinLine } from "./theme.mjs";

function lane(slide, ctx, x, titleText, color, items) {
  box(slide, ctx, x, 190, 256, 372, "#FFFFFF", "#D1C9B8");
  text(slide, ctx, titleText, x + 22, 212, 212, 30, { size: 20, bold: true, color });
  thinLine(slide, ctx, x + 22, 252, 88, 4, color);
  items.forEach((it, idx) => {
    const y = 278 + idx * 66;
    box(slide, ctx, x + 22, y, 212, 48, idx % 2 === 0 ? "#F7F4EA" : "#F0EADA", "#E0D7C5");
    text(slide, ctx, it, x + 36, y + 10, 184, 28, { size: 15, color: C.ink, valign: "middle" });
  });
}

export async function slide02(presentation, ctx) {
  const slide = presentation.slides.add();
  bg(slide, ctx);
  title(slide, ctx, "ARCHITECTURE", "项目已经从单文件练习拆成了可识别的框架层。");
  lane(slide, ctx, 68, "入口与平台层", C.blue, ["main.cpp", "Application", "GLFW / GLAD", "ImGui"]);
  lane(slide, ctx, 366, "场景与资源层", C.green, ["Object / Scene", "Mesh / Geometry", "Material", "Texture cache"]);
  lane(slide, ctx, 664, "渲染执行层", C.amber, ["Renderer", "Shader program", "Render state", "透明队列"]);
  lane(slide, ctx, 962, "资产与工具层", C.red, ["AssimpLoader", "stb_image", "assets/shaders", "assets/textures"]);
  thinLine(slide, ctx, 196, 584, 870, 4, "#C9BEAA");
  text(slide, ctx, "结构价值：后续优化不必推倒重来，应围绕现有模块补齐所有权、资源管理器和验证工具。", 224, 602, 832, 32, { size: 18, bold: true, color: C.ink, align: "center" });
  footer(slide, ctx, 2);
  return slide;
}
