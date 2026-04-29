declare global {
  function nativeDrawRect(x: number, y: number, w: number, h: number, r: number, g: number, b: number): void;
  function nativeDrawText(text: string, x: number, y: number): void;
  var render: (dt: number) => void; // Added dt
  var onKeyPress: (key: string) => void;
  var windowHeight: number;
}
export { };
