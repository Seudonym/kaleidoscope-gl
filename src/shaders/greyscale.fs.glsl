#version 460 core

out vec4 FragColor;

uniform vec2 u_resolution;
uniform vec2 u_center;
uniform float u_zoom;
uniform float u_iterations;
uniform float u_power;


vec3 mandelbrot(vec2 uv) {
  const float radius = 2.0;

  vec2 c = uv;
  vec2 z = vec2(0.0, 0.0);
  float i = 0.0;

  // TODO: Anti-aliasing

  for (int j = 0; j < u_iterations; j++) {
    if (u_power == 2.0) z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
    else {
      float r = length(z);
      float theta = atan(z.y, z.x);
      z = pow(r, u_power) * vec2(cos(u_power * theta), sin(u_power * theta)) + c;
    }
    if (dot(z, z) > radius * radius) { break; }
    i += 1.0;
  }
  i = i / u_iterations;
  return vec3(1.0 - i, 1.0 - i, 1.0 -i);
}

void main() {
  // Setup uv
  vec2 uv = gl_FragCoord.xy / u_resolution.xy - vec2(0.5);
  uv *= 2.0 * vec2(u_resolution.x / u_resolution.y, 1.0);

  // Render
  vec3 color = mandelbrot(uv * u_zoom + u_center);
  if (length(uv) < 0.01) { color = vec3(1.0, 0.0, 0.0); }
  // Output
  FragColor = vec4(color, 1.0);
}
