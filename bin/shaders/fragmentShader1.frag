#version 330 core
out vec4 FragColour;

in vec3 out_vecCol;

uniform int setContourMode;
uniform float blend;

//* Declaration
float c_argument(vec2 z);
float c_modulus(vec2 z);
vec2 c_mul(vec2 z1, vec2 z2);
vec2 c_div(vec2 z1, vec2 z2);
vec2 c_pow(vec2 z, int k);
vec2 c_pow(float x, vec2 z);
vec2 c_pow(vec2 z1, vec2 z2);
vec2 c_sin(vec2 z);
vec2 c_cos(vec2 z);
vec2 c_tan(vec2 z);
vec2 c_exp(vec2 z);
vec2 c_log(vec2 z);    
vec2 c_eta(vec2 z, int terms);
vec2 c_zeta(vec2 z, int terms);
vec2 c_gamma(vec2 z);


//* Math Functions
#define M_PI 3.1415926535897932384626433832795
float c_argument(vec2 z){//Complex argument
    return atan(z.y, z.x);
}

float c_modulus(vec2 z){
    return sqrt(z.x * z.x + z.y * z.y);
}

vec2 c_mul(vec2 z1, vec2 z2){
    return vec2(z1.x * z2.x - z1.y * z2.y, z1.x * z2.y + z1.y * z2.x);
}

vec2 c_div(vec2 z1, vec2 z2)
{
    if (z2.y == 0)
    {
        return vec2(z1.x / z2.x, z1.y / z2.x);
    }
    if (z2.x == 0)
    {
        return vec2(z1.y / z2.y, -(z1.x / z2.y));
    }

    float r = z2.x * z2.x + z2.y * z2.y;
    return vec2((z1.x * z2.x + z1.y * z2.y) / r, (z1.y * z2.x - z1.x * z2.y) / r);
}

vec2 c_pow(vec2 z, int k)
{
    vec2 res = vec2(1.0, 0);
    if (k >= 0)
    {
        for (; k > 0; k--)
            res = c_mul(res, z);
    }
    else
    {
        for (; k < 0; k++)
            res = c_div(res, z);
    }
    return res;
}
vec2 c_pow(float x, vec2 z){
    return c_exp(log(x) * z);
}

vec2 c_pow(vec2 z1, vec2 z2){
    float r = c_modulus(z1);
    float theta = c_argument(z1);
    return c_exp((log(r) * (z2)) + c_mul(theta * vec2(0,1), z2));
}

vec2 c_sin(vec2 z)
{
    return vec2(sin(z.x) * cosh(z.y), cos(z.x) * sinh(z.y));
}

vec2 c_cos(vec2 z)
{
    return vec2(cos(z.x) * cosh(z.y), sin(z.x) * sinh(z.y));
}

vec2 c_tan(vec2 z)
{
    float cx = cos(z.x);
    float shy = sinh(z.y);
    float temp = cx * cx + shy * shy;
    return vec2((sin(z.x) * cx) / temp, (shy * cosh(z.y)) / temp);
}

vec2 c_exp(vec2 z)
{
    return exp(z.x) * vec2(cos(z.y), sin(z.y));
}

vec2 c_log(vec2 z)
{
    return vec2(log(c_modulus(z)), c_argument(z));
}            

vec2 c_eta(vec2 z, int terms){
    vec2 temp = vec2(0.0, 0.0);
    for(int i = 1; i <= terms; i++){
        temp += (i % 2) == 1 ? c_div(vec2(1.0,0.0),c_pow(vec2(i,0), z)) : -1 * c_div(vec2(1.0,0.0),c_pow(vec2(i,0), z));
    }
    return temp;
}

//! Not to be called directly
vec2 c_zeta_p(vec2 z, int terms){  
    //Dirichlet eta function
    vec2 temp = c_eta(z, terms);
    //Zeta function for Re(z) > 0 by dividing eta fucntion
    //temp = c_div(temp, (vec2(1,0) - c_div(vec2(2.0,0.0),c_pow(vec2(2,0), z))));
    return c_div(temp, (vec2(1,0) - c_pow(vec2(2,0), vec2(1,0) - z)));
}
//! Not to be called directly
vec2 c_zeta_n(vec2 z, int terms){
    vec2 temp = c_zeta_p(vec2(1,0) - z, terms);
    // Below is why we need operator overloading in glsl. Could have easily been:
    // 2^s * M_PI^(s-1) * c_sin(M_PI/2 * s) * c_gamma(1 - s) * c_zeta_p(h-s, terms);
    return c_mul(c_mul(c_mul(c_mul(c_pow(vec2(2,0), z), c_pow(vec2(M_PI, 0), z - vec2(1,0))), c_sin((M_PI / 2) * z)), c_gamma(vec2(1,0) - z)), c_zeta_p(vec2(1,0) - z, terms));
}
//? WHY DOESNT THIS WORK?
vec2 c_zeta(vec2 z, int terms){
    if(z.x > 0) 
        return c_zeta_p(z, terms);
    else return c_zeta_n(z, terms);
}

/**************************************************
IMPLEMENTATION BASED ON PYTHON EXAMPLE IN:
https://en.wikipedia.org/wiki/Lanczos_approximation
FROST WAS HERE
***************************************************/
vec2 c_gamma(vec2 z){//Using Lanczos approximation

    float p[8] = float[](
    676.5203681218851
    ,-1259.1392167224028
    ,771.32342877765313
    ,-176.61502916214059
    ,12.507343278686905
    ,-0.13857109526572012
    ,9.9843695780195716e-6
    ,1.5056327351493116e-7
    );

    vec2 y;
    if(z.x < 0.5){
        vec2 _z = vec2(1,0) - z;
        vec2 _y;

        _z -= vec2(1,0);
        vec2 x = vec2(0.99999999999980993, 0);
        for(int i = 0; i < 8; i++){
            x += c_div(vec2(p[i], 0), _z + vec2(i + 1, 0));
        }
        vec2 t = _z + vec2(8 /*len(p)*/ - 0.5, 0);
        _y = sqrt(2 * M_PI) * c_mul(c_mul(c_pow(t, _z + vec2(0.5,0)), c_exp(-1 * t)), x);


        y = c_div(vec2(M_PI,0), c_mul(c_sin(M_PI * z), _y));
    }
    else{
        z -= vec2(1,0);
        vec2 x = vec2(0.99999999999980993, 0);
        for(int i = 0; i < 8; i++){
            x += c_div(vec2(p[i], 0), z + vec2(i + 1, 0));
        }
        vec2 t = z + vec2(8 /*len(p)*/ - 0.5, 0);
        y = sqrt(2 * M_PI) * c_mul(c_mul(c_pow(t, z + vec2(0.5,0)), c_exp(-1 * t)), x);
    }

    return y;
}

//*HSL to RGB Conversion Functions
//Ref: https://www.baeldung.com/cs/convert-color-hsl-rgb
vec3 hsl_to_rgb(float h, float s, float l){
    
    h = degrees(h);//h is passed in as radians
    float c = (1.0f - abs(2 * l - 1.0f)) * s; //Chroma
    float x = c * (1.0f - abs(mod((h / 60), 2) - 1.0f));
    float m = l - (c/2.0f); //Adding lightness back into RGB

    vec3 temp = vec3(0.0f, 0.0f, 0.0f);

    if(h >= 0 && h < 60){
        temp = vec3(c, x, 0);
    }
    else if(h < 120){
        temp = vec3(x, c, 0);
    }
    else if(h < 180){
        temp = vec3(0, c, x);
    }
    else if(h < 240){
        temp = vec3(0, x, c);
    }
    else if(h < 300){
        temp = vec3(x, 0, c);
    }
    else if(h < 360){
        temp = vec3(c, 0, x);
    }
    else{
        temp = vec3(c, x, 0);
    }

    vec3 rgb = vec3((temp.x + m), (temp.y + m), (temp.z + m));

    //return temp;
    return rgb;
}

//HSL Colour Functions
float lightness(vec2 z){
    return (2 / M_PI) * atan(c_modulus(z)); //Range: [0,1)
    //return 1 - pow(0.5f, c_modulus(z));
}

float hue(vec2 z){
    return (c_argument(z) + 2 * M_PI / 3);
}

vec4 colour_point(vec2 z, int setContourMode){

    float r = c_modulus(z);
    float phi = c_argument(z);
    if(phi < 0){
        phi += 2 * M_PI;
    }
    float modulus_temp = fract(blend * log2(r));
    float contour_shading_modulus = modulus_temp > 0.5 ? modulus_temp : modulus_temp + 0.5;
    float phase_temp = fract((2 * (blend + 3) * phi/M_PI)) + 0.5;
    float contour_shading_phase = phase_temp;

    float h = phi;
    float s = 1.0f;
    float l = 0.5f;
    switch (setContourMode){
        case 0:
            l = 0.5f;
            break;
        case 1:
            l = (2 / M_PI) * atan(contour_shading_modulus); //Modulus Contour
            break;
        case 2:
            l = (2 / M_PI) * atan(contour_shading_phase); //Phase Contour
            break;
    }
    return vec4(hsl_to_rgb(h, s, l), 1.0f);
}

vec2 f(vec2 z){ //Edit this
    //return c_mul((z - vec2(1.0f, 0.0f)), (z - vec2(0.0f,1.0f)));
    //return z;
    //return (c_pow(z,5) - c_pow(z,3));
    //return (c_pow(z,3) - vec2(0.5,-0.5));
    //return c_pow(c_log(z), z);
    //return c_pow(z - vec2(1.0, 0),c_log(z - vec2(0, -1.0)));
    return c_gamma(z);
}

void main(){
    vec2 z = f(out_vecCol.xy);
    //z = c_mul(c_pow(0.1,2 * z),2 * z);
    //z = c_pow(z*2, 3) - vec2(1,0);
    FragColour = colour_point(z, setContourMode);
}