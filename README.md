# 1. **Domain Colouring**
Simple Domain Colouring with contour shading is implemented here using OpenGL. Note that due to naive implementation and OpenGL limitations, floating point precision is not very high. Planned features includes a UI with an equation parser for a less clunky rendering experience. The default function rendered is the `gamma` function
___
# 2. Instructions to render your own function
1. Open the `~/bin/shaders/fragmentShader1.frag` shader file with a text editor
2. Navigate to the function `vec2 f(vec2 z)`
3. Edit `f()` with the complex functions implemented in the fragment shader. A list of implemented complex functions can be found below.
 # 3. Complex Functions

 Complex numbers are represented as a `vec2`, with their `x` and `y` component being the real and imaginary parts respectively. Note that since GLSL does not have operator overloading, simple mathematical operations like multiplication, division and exponentiation havs to be done using the complex functions implemented in the fragment shader. A list of complex functions can be found below.

 ___
 **Supported operators:**

 Data Types:
  ```float a,b,x,y```

 - `+` : ```vec2(x,y) + vec2(a,b) = vec2(x+a, y+b)```
 - `+` : ```a + vec2(x,y) = vec2(a+x,a+y)```
 - `-` : ```vec2(x,y) - vec2(a,b) = vec2(x-a, y-b)```
 - `-` : ```-vec2(x,y) = vec2(x,y)```
 ___

**Currently implemented complex functions:**
___
- float c_argument(vec2 z)
- float c_modulus(vec2 z)
- vec2 c_mul(vec2 z1, vec2 z2)
- vec2 c_div(vec2 z1, vec2 z2)
- vec2 c_pow(vec2 z, int k)
- vec2 c_pow(float x, vec2 z)
- vec2 c_pow(vec2 z1, vec2 z2)
- vec2 c_sin(vec2 z)
- vec2 c_cos(vec2 z)
- vec2 c_tan(vec2 z)
- vec2 c_exp(vec2 z)
- vec2 c_log(vec2 z)    
- vec2 c_eta(vec2 z, int terms)
- vec2 c_gamma(vec2 z)

**Complex functions to be implemented:**
___
- vec2 c_zeta(vec2 z, int terms)

