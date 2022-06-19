# LearnOpenGL
This is my personal spagetti code while learning OpenGL

This fork is an attempt to implement simple domain colouring. It generates a 2D grid of points, and uses an EBO generated in such a way to tessellate the grid of points with triangles when passing the grid points into the vertexshader. Each point (x,y) are treated as complex inputs. The triangle primitives are then passed into the the fragment shader, where each vertex of the triangle will be coloured using domain colouring techniques. Opengl's colour linear interpolation will then fill the entire triangle with based on the colour of the vertices. The idea is that with more points in the grid, the higher more accurate the resulting colouring will be.
