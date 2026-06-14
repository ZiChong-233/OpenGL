import { C, bg, box, footer, text, thinLine, title } from "./theme.mjs";

function step(slide, ctx, x, y, titleText, body, color) {
  box(slide, ctx, x, y, 190, 116, "#FFFFFF", color);
  text(slide, ctx, titleText, x + 16, y + 16, 158, 24, { size: 18, bold: true, color });
  text(slide, ctx, body, x + 16, y + 50, 158, 42, { size: 14, color: C.ink });
}

export async function slide06(presentation, ctx) {
  const slide = presentation.slides.add();
  bg(slide, ctx);
  title(slide, ctx, "ENGINE STANDARD", "成熟引擎把释放问题变成一条可验证的资源流水线。");
  const xs = [78, 296, 514, 732, 950];
  step(slide, ctx, xs[0], 220, "Acquire", "通过 ResourceManager 按 key 获取资源。", C.blue);
  step(slide, ctx, xs[1], 220, "Handle", "材质持有 handle，引用和所有权分开。", C.green);
  step(slide, ctx, xs[2], 220, "Use", "Renderer 在 pass 内决定纹理单元和状态。", C.amber);
  step(slide, ctx, xs[3], 220, "Release", "引用归零或场景卸载后进入释放队列。", C.red);
  step(slide, ctx, xs[4], 220, "Report", "帧末或退出时输出 live count 和泄漏。", C.charcoal);
  [268, 486, 704, 922].forEach((x) => thinLine(slide, ctx, x, 274, 28, 4, "#B9AF9E"));
  box(slide, ctx, 106, 402, 462, 104, C.green2, C.green);
  text(slide, ctx, "CPU 对象层", 134, 422, 180, 28, { size: 20, bold: true, color: C.green });
  text(slide, ctx, "Object、Mesh、Material、Asset metadata 使用 unique_ptr / shared_ptr / handle 表达所有权。", 134, 456, 376, 34, { size: 15, color: C.ink });
  box(slide, ctx, 710, 402, 462, 104, C.blue2, C.blue);
  text(slide, ctx, "GPU 句柄层", 738, 422, 180, 28, { size: 20, bold: true, color: C.blue });
  text(slide, ctx, "VAO、Buffer、Texture、ShaderProgram、Framebuffer 是不可拷贝、可移动的 RAII 对象。", 738, 456, 376, 34, { size: 15, color: C.ink });
  footer(slide, ctx, 6);
  return slide;
}
