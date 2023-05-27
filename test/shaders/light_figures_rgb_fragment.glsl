#version 300 es
precision mediump float;

in vec2 TexCoord;
out vec4 fragColor;

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define samples 8

#define pi 3.141592564

const vec3 materials[] = vec3[](
    vec3(1.000, 1.000, 1.000), // white
    vec3(1.000, 0.067, 0.157), // red
    vec3(0.027, 0.945, 0.259), // green
    vec3(0.318, 0.553, 0.992));// blue

const vec2 points[] = vec2[](
    vec2(.1, -.25),
    vec2(.3, -.25),
    vec2(.1, -.05),
    vec2(.3, -.05),
    vec2(-.9, -.4),
    vec2(.8, -.4),
    vec2(-.9, -1.),
    vec2(.8, 1.),
    vec2(-.4, -.3),
    vec2(-.2, -.3),
    vec2(-.4, -.1),
    vec2(-.2, -.1),
    vec2(-.05, -.05),
    vec2(-.05, -.15),
    vec2(0, -.1),
    vec2(-.1, -.1));

const int segmentCount = 15;
const ivec3 segments[15] = ivec3[](
    ivec3(0, 1, 1), // ivec3(a,b,c)
    ivec3(0, 2, 1), // a = endpoint a index
    ivec3(1, 3, 1), // b = endpoint b index
    ivec3(2, 3, 1), // c = material index
    ivec3(4, 5, 0),
    ivec3(4, 6, 0),
    ivec3(5, 7, 0),
    ivec3(8, 9, 3),
    ivec3(8, 10, 3),
    ivec3(9, 11, 3),
    ivec3(10, 11, 3),
    ivec3(12, 14, 2),
    ivec3(14, 13, 2),
    ivec3(13, 15, 2),
    ivec3(15, 12, 2));

// https://www.shadertoy.com/view/4djSRW
vec2 hash21(float p) {
    vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xx + p3.yz) * p3.zy);

}

// Interleaved gradient noise
// https://blog.demofox.org/2022/01/01/interleaved-gradient-noise-a-different-kind-of-low-discrepancy-sequence/
float IGN(ivec2 p) {
    return mod(52.9829189f * mod(.06711056f * float(p.x) + .00583715f * float(p.y), 1.), 1.);
}

// Ray intersection with line segment
float segmentIntersect(vec2 ro, vec2 rd, vec2 a, vec2 b) {
    vec2 v1 = ro - a;
    vec2 v2 = b - a;
    vec2 v3 = vec2(-rd.y, rd.x);

    float d = dot(v2, v3);
    float t1 = cross(vec3(v2, 0), vec3(v1, 0)).z / d;
    float t2 = dot(v1, v3) / d;

    if (t1 >= 0.0 && (t2 >= 0.0 && t2 <= 1.0)) {
        return t1;
    }
    return 1000.;
}

//ray intersection with scene
//sceneIntersect.w is the distance, sceneIntersect.xyz is the color
vec4 sceneIntersect(vec2 ro, vec2 rd) {
    float v0 = 1000.;
    vec3 col;

    for (int i = 0; i < segmentCount; i++) {

        vec2 a = points[segments[i].x];
        vec2 b = points[segments[i].y];

        float v1 = segmentIntersect(ro, rd, a, b);
        if (v1 < v0) {
            col = materials[segments[i].z];
            v0 = v1;
        }
    }
    return vec4(col, v0);
}

//line segment SDF
float line(vec2 p, vec2 a, vec2 b) {
    p -= a, b -= a;
float h = clamp(dot(p, b) / dot(b, b), 0., 1.);
return length(p - b * h);
}

//scene SDF
//sceneDist.w is the distance, sceneDist.xyz is the color
vec4 sceneDist(vec2 p) {
    float v0 = 1000.;
    vec3 col;

    for (int i = 0; i < segmentCount; i++) {

        vec2 a = points[segments[i].x];
        vec2 b = points[segments[i].y];

        float v1 = line(p, a, b);
        if (v1 < v0) {
            col = materials[segments[i].z];
            v0 = v1;
        }
    }
    return vec4(col, v0);
}

vec2 sceneNormal(vec2 p) {
    vec2 epsilon = vec2(.001, -.001);
    return normalize(vec2(sceneDist(p + epsilon.xx).w) - vec2(sceneDist(p - epsilon.xy).w, sceneDist(p - epsilon.yx).w));
}

// ACES Tonemapping
vec3 ACESFilm(vec3 x) {

    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0f, 1.0f);
}

#define lightFalloff 2.
void main() {
    vec2 fragCoord = gl_FragCoord.xy;
    vec3 color;
    vec2 uv = (2. * fragCoord - iResolution.xy - .5) / iResolution.x;
    float randomValue = IGN(ivec2(fragCoord.xy));

    vec3 spotlight;
    vec3 globalIllumination;

    vec2 lightPosition = vec2(sin(iTime * .5) * .75, cos(iTime * .25) * .25 + .25);

    vec2 lightDirection = normalize(vec2(sin(iTime * 1.5), -1));
    if (iMouse.z > 0.) {
        lightPosition = vec2(2, -2) * iMouse.zw / iResolution.x - vec2(1., .56);
        lightDirection = normalize(2. * iMouse.xy / iResolution.x - vec2(1., .561) - lightPosition);
    }
    const float lightRadius = .005;

    if (sceneIntersect(uv, normalize(lightPosition - uv)).w > distance(uv, lightPosition)) {
        spotlight = vec3(max((.5 * float(dot(normalize(uv - lightPosition), lightDirection)) - .5) / lightRadius + 1., 0.));
    }

    vec2 intersectionPoint;
    for (int i = 0; i < samples; i++) {
        vec2 rayOrigin = lightPosition;
        float rotation = .08 * pi * ((float(i) + randomValue) / float(samples) - .5) + atan(lightDirection.y, lightDirection.x);
        vec2 rayDirection = vec2(cos(rotation), sin(rotation));
        vec2 lightDirectionSampled = rayDirection;

        float distanceToIntersection = sceneIntersect(rayOrigin, rayDirection).w;
        intersectionPoint = rayOrigin + rayDirection * distanceToIntersection;
        vec2 normal = sceneNormal(intersectionPoint - rayDirection * .01);

        rayOrigin = uv;
        rayDirection = normalize(intersectionPoint - uv);

        float intersectionDistance = min(distance(uv, intersectionPoint) / lightFalloff, 1.);

        vec4 lightRay = sceneIntersect(rayOrigin, rayDirection);
        distanceToIntersection = lightRay.w;

        if (distanceToIntersection + .01 > distance(uv, intersectionPoint)) {
            globalIllumination += 1. / float(samples) * lightRay.rgb * clamp(dot(-rayDirection, normal), 0., 1.) * (1. - sqrt(2. * intersectionDistance - intersectionDistance * intersectionDistance))
            * (sceneDist(uv).w > .005 ? 1. : dot(sceneNormal(uv), lightDirectionSampled) * .5 + .5);
        }
    }

    vec4 scene = sceneDist(uv);
    color = spotlight * .5 + globalIllumination * 1.;
    color *= scene.w > .005 ? vec3(.25) : 3. * scene.rgb;

    // Tonemapping
    color = ACESFilm(color);
    color = pow(color, vec3(1. / 2.2));

    fragColor = vec4(color, 1);
}

