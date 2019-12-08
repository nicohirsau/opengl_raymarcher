#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform float u_time;
uniform vec2 u_mPos;
uniform vec3 u_pPos;
uniform vec2 u_pRot;
uniform sampler2D u_texture0;

#define M_PI 3.1415926535897932384626433832795

#define sphere_pos vec3(0.0, 0.0, 0)

mat3 rotAxis(float a, vec3 axis) {
    float s=sin(a);
    float c=cos(a);
    float oc=1.0-c;
    vec3 as=axis*s;
    mat3 p=mat3(axis.x*axis,axis.y*axis,axis.z*axis);
    mat3 q=mat3(c,-as.z,as.y,as.z,c,-as.x,-as.y,as.x,c);
    return p*oc+q;
}

float sdSphere(vec3 p, vec3 sp, float r) {
    return length(p - sp) - r;
}

float sdTorus( vec3 p, vec3 tp, vec2 t ) {
  vec2 q = vec2(length(p.xz - tp.xz)-t.x,p.y - tp.y);
  return length(q)-t.y;
}

float sdBox( vec3 p, vec3 bp, vec3 b ) {
  vec3 q = abs(p - bp) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdCappedCylinder( vec3 p, vec3 cp, float h, float r )
{
  vec2 d = abs(vec2(length(p.xz - cp.xz),p.y - cp.y)) - vec2(h,r);
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float dot2( vec3 v ) { return dot(v,v); }
float udQuad( vec3 p, vec3 a, vec3 b, vec3 c, vec3 d )
{
  vec3 ba = b - a; vec3 pa = p - a;
  vec3 cb = c - b; vec3 pb = p - b;
  vec3 dc = d - c; vec3 pc = p - c;
  vec3 ad = a - d; vec3 pd = p - d;
  vec3 nor = cross( ba, ad );

  return sqrt(
    (sign(dot(cross(ba,nor),pa)) +
     sign(dot(cross(cb,nor),pb)) +
     sign(dot(cross(dc,nor),pc)) +
     sign(dot(cross(ad,nor),pd))<3.0)
     ?
     min( min( min(
     dot2(ba*clamp(dot(ba,pa)/dot2(ba),0.0,1.0)-pa),
     dot2(cb*clamp(dot(cb,pb)/dot2(cb),0.0,1.0)-pb) ),
     dot2(dc*clamp(dot(dc,pc)/dot2(dc),0.0,1.0)-pc) ),
     dot2(ad*clamp(dot(ad,pd)/dot2(ad),0.0,1.0)-pd) )
     :
     dot(nor,pa)*dot(nor,pa)/dot2(nor) );
}

float intersectSDF(float distA, float distB) {
    return max(distA, distB);
}

float unionSDF(float distA, float distB) {
    return min(distA, distB);
}

float differenceSDF(float distA, float distB) {
    return max(distA, -distB);
}

float displacement(vec3 p) {
    return sin(20*p.x)*sin(20*p.y)*sin(20*p.z);
}

float opDisplace( float dst, vec3 p )
{
    float d1 = dst;
    float d2 = displacement(p);
    return d1+d2;
}

float getSceneDistance(vec3 pos) {
    pos = vec3(pos.x, pos.y, pos.z);
    float dst = unionSDF(
        differenceSDF(sdBox(pos, sphere_pos, vec3(2.3)), sdSphere(pos, sphere_pos, 2.75)), 
            differenceSDF(
                differenceSDF(
                    differenceSDF(
                        differenceSDF(
                        sdTorus(pos, sphere_pos, vec2(0.5, 0.3)), 
                        sdSphere(pos, sphere_pos + vec3(-0.75, 0.0, -0.75), 0.5)
                    ), sdSphere(pos, sphere_pos + vec3(0.75, 0.0, 0.75), 0.5)
                ), sdSphere(pos, sphere_pos + vec3(0.75, 0.0, -0.75), 0.5)
            ), sdSphere(pos, sphere_pos + vec3(-0.75, 0.0, 0.75), 0.5)
        )
    );
    //float dst = differenceSDF(
    //    intersectSDF(
    //        sdBox(pos, sphere_pos, vec3(1)),
    //        sdSphere(pos, sphere_pos, 1.25)
    //    ),
    //    sdCappedCylinder(pos, sphere_pos, 0.5, 2)
    //);
    return dst;
}

void main() {
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    vec3 pos = u_pPos;
    vec3 dir = normalize(vec3(
        TexCoord.x / 2.0,
        TexCoord.y / 2.0,
        1
    ));

    mat3 rotX = rotAxis(u_pRot.x, vec3(1, 0, 0));
    dir = dir * rotX;
    mat3 rotY = rotAxis(u_pRot.y, vec3(0, 1, 0));
    dir = dir * rotY;

    bool hit = false;
    float steps = 0;
    float dstToOrig = 0.0;
    float max_steps = 10.0;

    while(dstToOrig < 999) {
        float c = 10.0;
        //pos = mod(pos+0.5*c,c)-0.5*c;
        pos = vec3(pos.x, pos.y, pos.z);
        
        float dst = getSceneDistance(pos);
        
        if (dst <= 0.001) {
            vec3 d = normalize(sphere_pos - pos);
            float u = 0.5 + atan(d.z, d.x) / M_PI + u_time * 0.2;
            float v = 1.0 - (0.5 - asin(d.y) / M_PI);
            color = texture(u_texture0, vec2(u, v)) * vec4(1.0);
            color *= (1 - steps / 40.0);// * vec4(sin(pos.x), cos(pos.y), cos(pos.z), 1.0);
            color.w = 1.0;
            hit = true;
            break;
        }

        pos += dir * dst; 
        dstToOrig += dst;   
        steps++;
    }

    if (!hit) {
        float background_shading = 1 * (steps / max_steps);
        color = vec4(
            background_shading * (3 / 255.0), 
            background_shading * (33 / 255.0), 
            background_shading * (74 / 255.0), 
            1.0
        );
    }

    FragColor = color;
}
