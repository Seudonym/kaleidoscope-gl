#version 460 core

out vec4 FragColor;

uniform vec2 u_resolution;
uniform vec2 u_center;
uniform float u_zoom;
uniform float u_iterations;
uniform float u_power;

uniform int u_time;
uniform float u_gamma;

uniform vec3 input1, input2, input3, input4;

vec3 palette[] = {
  vec3(0.0),
  // vec3(50, 168, 160),
  // vec3(242, 138, 138),
  // vec3(171, 183, 255),
  // vec3(227, 138, 242)
  input1, input2, input3, input4
};

vec3 mandelbrot(vec2 uv) {
  const float radius = 4.0;

  vec2 c = uv;
  vec2 z = c;
  float i = 0.0;

  // TODO: Optimize
  // float q = (z.x - 0.25) * (z.x - 0.25) + z.y * z.y;
  // if (q * (q + (z.x - 0.25)) < 0.25 * z.y * z.y || (z.x + 1.0) * (z.x + 1.0) + z.y * z.y < 0.0625) {
  //   i = u_iterations;
  //   return cos(u_time * 0.01 + vec3(1, 1.2, 1.4) * exp(max(i, 40.0)));
  // }

  for (int j = 0; j < u_iterations; j++) {
    z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
    if (dot(z, z) > radius * radius) { break; }
    i += 1.0;
  }
  float sn = i - log2(log2(dot(z, z))) + 4.0;
  sn = sn / u_iterations;

  float v = pow(1.0 - sn, u_power);
  float pindex = v * 4;

  vec3 c1 = palette[int(pindex)];
  vec3 c2 = palette[int(pindex) + 1];

  return mix(c1, c2, fract(pindex));

}

void main() {
  // Setup uv
  vec2 uv = gl_FragCoord.xy / u_resolution.xy - vec2(0.5);
  uv *= 2.0 * vec2(u_resolution.x / u_resolution.y, 1.0);

  // Render
  vec3 color = mandelbrot(uv * u_zoom + u_center);
  // if (length(uv) < 0.01) { color = vec3(1.0, 0.0, 0.0); }
  // Output
  FragColor = pow(vec4(color, 1.0), vec4(u_gamma));
}
