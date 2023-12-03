#version 460 core

out vec4 FragColor;

uniform vec2 u_resolution;
uniform vec2 u_center;
uniform float u_zoom;
uniform float u_iterations;

uniform vec3 u_input1;

vec3 mandelbrot(vec2 uv) {
  vec2 c = uv;
  vec2 z = vec2(0.0, 0.0);
  float i = 0.0;
  for (int j = 0; j < u_iterations; j++) {
    if (dot(z, z) > 4.0) { break; }
    z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
    i += 1.0;
  }
  // Smooth color
  float norm_i = i - log(log(dot(z, z))) / log(2.0);
  norm_i /= u_iterations;
  return vec3(norm_i);
}

void main() {
  // Setup uv
  vec2 uv = gl_FragCoord.xy / u_resolution.xy - vec2(0.5);
  uv *= 2.0 * vec2(u_resolution.x / u_resolution.y, 1.0);

  // Render
  float iterations = u_iterations;
  vec3 color = mandelbrot(uv * u_zoom + u_center);
  if (length(uv) < 0.01) { color = u_input1; }
  // Output
  FragColor = vec4(color, 1.0);
}