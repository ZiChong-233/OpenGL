export const C = {
  paper: "#F7F4EA",
  paper2: "#ECE7D8",
  ink: "#17202A",
  muted: "#68707A",
  blue: "#245F8D",
  blue2: "#DDEAF3",
  green: "#2E7D68",
  green2: "#DDEDE7",
  amber: "#B7791F",
  amber2: "#F3E2BE",
  red: "#B64B4B",
  red2: "#F1D8D5",
  charcoal: "#263238",
  white: "#FFFFFF",
  transparent: "#00000000",
};

export function bg(slide, ctx, fill = C.paper) {
  ctx.addShape(slide, { x: 0, y: 0, width: ctx.W, height: ctx.H, fill, line: ctx.line(C.transparent, 0) });
}

export function text(slide, ctx, value, x, y, width, height, opts = {}) {
  return ctx.addText(slide, {
    text: value,
    x, y, width, height,
    fontSize: opts.size ?? 24,
    color: opts.color ?? C.ink,
    bold: opts.bold ?? false,
    align: opts.align ?? "left",
    valign: opts.valign ?? "top",
    typeface: "Microsoft YaHei",
    fill: opts.fill ?? C.transparent,
    line: opts.line ?? ctx.line(C.transparent, 0),
    insets: opts.insets ?? { left: 0, right: 0, top: 0, bottom: 0 },
    name: opts.name,
  });
}

export function title(slide, ctx, kicker, claim) {
  text(slide, ctx, kicker, 68, 42, 280, 24, { size: 14, bold: true, color: C.blue, valign: "middle" });
  ctx.addShape(slide, { x: 68, y: 73, width: 64, height: 4, fill: C.blue, line: ctx.line(C.transparent, 0) });
  text(slide, ctx, claim, 68, 92, 900, 70, { size: 34, bold: true, color: C.ink, valign: "middle" });
}

export function footer(slide, ctx, page) {
  ctx.addShape(slide, { x: 68, y: 670, width: 1144, height: 1, fill: "#D1C9B8", line: ctx.line(C.transparent, 0) });
  text(slide, ctx, "基于源码静态分析 | 未重新编译验证：当前终端未找到 cmake", 68, 682, 720, 24, { size: 12, color: C.muted, valign: "middle" });
  text(slide, ctx, String(page).padStart(2, "0"), 1162, 682, 50, 24, { size: 12, bold: true, color: C.muted, align: "right", valign: "middle" });
}

export function box(slide, ctx, x, y, width, height, fill, stroke = "#D8D0BF") {
  return ctx.addShape(slide, {
    x, y, width, height,
    fill,
    line: ctx.line(stroke, 1),
    geometry: "roundRect",
  });
}

export function labelBox(slide, ctx, label, x, y, width, height, color, fill) {
  box(slide, ctx, x, y, width, height, fill, color);
  text(slide, ctx, label, x + 14, y + 8, width - 28, height - 16, { size: 17, bold: true, color, valign: "middle" });
}

export function pill(slide, ctx, label, x, y, width, fill, color = C.ink) {
  box(slide, ctx, x, y, width, 34, fill, C.transparent);
  text(slide, ctx, label, x + 16, y + 6, width - 32, 22, { size: 14, bold: true, color, align: "center", valign: "middle" });
}

export function bar(slide, ctx, label, score, x, y, width, color) {
  text(slide, ctx, label, x, y, 220, 26, { size: 16, color: C.ink, valign: "middle" });
  ctx.addShape(slide, { x: x + 240, y: y + 7, width, height: 12, fill: "#DED6C6", line: ctx.line(C.transparent, 0) });
  ctx.addShape(slide, { x: x + 240, y: y + 7, width: width * score / 5, height: 12, fill: color, line: ctx.line(C.transparent, 0) });
  text(slide, ctx, `${score}/5`, x + 250 + width, y, 70, 26, { size: 14, bold: true, color, valign: "middle" });
}

export function thinLine(slide, ctx, x, y, width, height, fill = "#CFC6B3") {
  ctx.addShape(slide, { x, y, width, height, fill, line: ctx.line(C.transparent, 0) });
}
