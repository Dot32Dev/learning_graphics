#ifndef GRID_H
#define GRID_H

#include <glad/glad.h>

#define WIDTH 20
#define HEIGHT 15
#define TILESIZE 40

/*
* Will allocate space for the mesh, the address returned by this function must 
* be freed later. Returns a mesh where each vertex has a vec3 position and a 
* vec2 UV coordinate. The grid must be in the format of grid[WIDTH][HEIGHT]
* The grid must be larger than 0x0
*/
GLfloat* generate_mesh(int grid[WIDTH][HEIGHT], int* vertices);

#endif // GRID_H