#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;
 
layout(location = 0) out vec2 coord;
layout(location = 1) out vec3 color;
layout(location = 2) out vec2 resolution;
 
layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
 
    vec2 resolution;
    float time;

    float angle;
    float amp;
    float seed;
    float freqX;
    float freqY;
    float noiseSpeed;

    vec4 color1;
    vec4 color2;
    vec4 color3;
    vec4 color4;
    vec4 color5;
} ubufVert;
 
out gl_PerVertex { vec4 gl_Position; };   

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 permute(vec4 x) { return mod289(((x*34.0)+1.0)*x); }
vec4 taylorInvSqrt(vec4 r) { return 1.79284291400159 - 0.85373472095314 * r; }

float snoise(vec3 v)
{
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy;
  vec3 x3 = x0 - D.yyy;
  i = mod289(i);
  vec4 p = permute( permute( permute(i.z + vec4(0.0, i1.z, i2.z, 1.0 )) + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
  float n_ = 0.142857142857;
  vec3  ns = n_ * D.wyz - D.xzx;
  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);
  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );
  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);
  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));
  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );
}

struct VertDeform
{
    float incline;
    float offsetTop;
    float offsetBottom;

    vec2 noiseFreq;
    float noiseAmp;
    float noiseSpeed;
    float noiseFlow;
    float noiseSeed;
};

struct WaveLayer
{
    vec3  color;

    vec2 noiseFreq;
    float noiseSpeed;
    float noiseFlow;
    float noiseSeed;
    float noiseFloor;
    float noiseCeil;
};

void main() {
    coord = texcoord;

    vec3 colors[5];
    colors[0] = ubufVert.color1.rgb;
    colors[1] = ubufVert.color2.rgb;
    colors[2] = ubufVert.color3.rgb;
    colors[3] = ubufVert.color4.rgb;
    colors[4] = ubufVert.color5.rgb;

    const int wavesCount = 5;

    WaveLayer waveLayers[wavesCount];
    for(int i = 0; i < wavesCount; ++i)
    {
        waveLayers[i] = WaveLayer(
            colors[i],
            vec2(2.0 + float(i) / float(wavesCount), 3.0 + float(i) / float(wavesCount)),
            11.0 + 0.3 * float(i),
            6.5 + 0.3 * float(i),
            5.0 + 10.0 * float(i),
            0.1,
            0.63 + 0.07 * float(i)
        );
    }

    VertDeform vertDeform = VertDeform(
        sin(ubufVert.angle) / cos(ubufVert.angle),
        -0.5,
        -0.5,
        vec2(4.0, 4.0),
        ubufVert.amp,
        10.0,
        3.0,
        ubufVert.seed
    );

    vec2 uv = position.xy / ubufVert.resolution.xy;
    vec2 uvNorm = -1.0 + 2.0 * uv;
    vec2 st = 1.0 - uvNorm.xy;

    vec2 noiseFreq = vec2(ubufVert.freqX, ubufVert.freqY);
    vec2 noiseCoord = ubufVert.resolution * uvNorm * noiseFreq;

    float tilt = ubufVert.resolution.y / 2.0 * uvNorm.y;
    float incline = ubufVert.resolution.x * uvNorm.x / 2.0 * vertDeform.incline;
    float offset = ubufVert.resolution.x / 2.0 * vertDeform.incline * mix(vertDeform.offsetBottom, vertDeform.offsetTop, uv.y);

    float noise = snoise(vec3(
        noiseCoord.x * vertDeform.noiseFreq.x + ubufVert.time * vertDeform.noiseFlow,
        noiseCoord.y * vertDeform.noiseFreq.y,
        ubufVert.time * vertDeform.noiseSpeed + vertDeform.noiseSeed
    )) * vertDeform.noiseAmp;

    noise *= 1.0 - pow(abs(uvNorm.y), 2.0);
    //noise = max(0.0, noise);

    vec3 pos = vec3(
        position.x,
        position.y + tilt + incline + noise - offset,
        position.z
    );

    color = colors[0];

    for (int i = 0; i < wavesCount; ++i)
    {
        WaveLayer layer = waveLayers[i];
        vec3 noiseSource = vec3(
            noiseCoord.x * layer.noiseFreq.x + ubufVert.time * layer.noiseFlow,
            noiseCoord.y * layer.noiseFreq.y,
            ubufVert.time * layer.noiseSpeed + layer.noiseSeed
        );
        float noise = smoothstep(layer.noiseFloor, layer.noiseCeil, snoise(noiseSource) / 2.0 + 0.5);

        color = layer.color * pow(noise, 4.0) + color * (1.0 - pow(noise, 4.0));
    }

    resolution = ubufVert.resolution;

    gl_Position = ubufVert.qt_Matrix * vec4(pos, 1.0);
}
