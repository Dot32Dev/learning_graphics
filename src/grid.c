#include <stdlib.h>
#include <stdio.h>
#include "grid.h"

typedef struct Tile {
	int type;
	int rotation;
} Tile;

const Tile lookup[] = {
	// Empty
	[0]  = {0, 0},
	// Corners
	[1]  = {1, 0},
	[2]  = {1, 1},
	[4]  = {1, 2},
	[8]  = {1, 3},
	// Sides
	[3]  = {2, 0},
	[6]  = {2, 1},
	[12] = {2, 2},
	[9]  = {2, 3},
	// Opposite corners
	[5]  = {3, 0},
	[10] = {3, 1},
	// Everything except one corner
	[7]  = {4, 0},
	[14] = {4, 1},
	[13] = {4, 2},
	[11] = {4, 3},
	// Full
	[15] = {5, 0},
};

const float uvCorners[][2] = {
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
};

GLfloat* generate_mesh(int grid[WIDTH][HEIGHT], int* vertices) {
	int dualgrid[WIDTH + 1][HEIGHT + 1];

	int count = 0;
	for (int x = 0; x < WIDTH + 1; x++) {
		for (int y = 0; y < HEIGHT + 1; y++) {
			int tileState = 0;
			if (x > 0 && y > 0 && grid[x - 1][y - 1] == 1) tileState += 1;
			if (x < WIDTH && y > 0 && grid[x][y - 1] == 1) tileState += 2;
			if (x < WIDTH && y < HEIGHT && grid[x][y] == 1) tileState += 4;
			if (x > 0 && y < HEIGHT && grid[x - 1][y] == 1) tileState += 8;

			if (tileState != 0) count ++;
			dualgrid[x][y] = tileState;
		}
	}
	printf("count %d\n", count);

	GLfloat* mesh = (GLfloat*)malloc(count * sizeof(GLfloat) * 5 * 6);

	count = 0;
	for (int x = 0; x < WIDTH + 1; x++) {
		for (int y = 0; y < HEIGHT + 1; y++) {
			if (dualgrid[x][y] != 0) {
				printf("%d, ", dualgrid[x][y]);

				GLfloat tileX = (GLfloat)(x * TILESIZE - TILESIZE / 2);
				GLfloat tileY = (GLfloat)(y * TILESIZE - TILESIZE / 2);

				Tile tile = lookup[dualgrid[x][y]];

				float* topLeft = uvCorners[(0 + tile.rotation) % 4];
				float* topRight = uvCorners[(1 + tile.rotation) % 4];
				float* lowRight = uvCorners[(2 + tile.rotation) % 4];
				float* lowLeft = uvCorners[(3 + tile.rotation) % 4];

				GLfloat vertices[] = {
					// positions                                                 // texture coords
					(GLfloat)TILESIZE + tileX, -(GLfloat)TILESIZE - tileY, 0.0f, ((GLfloat)topRight[0] + tile.type) / 5.0f, (GLfloat)topRight[1],
					(GLfloat)TILESIZE + tileX,  0.0f              - tileY, 0.0f, ((GLfloat)lowRight[0] + tile.type) / 5.0f, (GLfloat)lowRight[1],
					00.0f             + tileX,  0.0f              - tileY, 0.0f, ((GLfloat)lowLeft[0]  + tile.type) / 5.0f, (GLfloat)lowLeft[1],  

					00.0f             + tileX,  0.0f              - tileY, 0.0f, ((GLfloat)lowLeft[0]  + tile.type) / 5.0f, (GLfloat)lowLeft[1],  
					00.0f             + tileX, -(GLfloat)TILESIZE - tileY, 0.0f, ((GLfloat)topLeft[0]  + tile.type) / 5.0f, (GLfloat)topLeft[1],   
					(GLfloat)TILESIZE + tileX, -(GLfloat)TILESIZE - tileY, 0.0f, ((GLfloat)topRight[0] + tile.type) / 5.0f, (GLfloat)topRight[1],
				};

				for (int i = 0; i < 6 * 5; i++) {
					mesh[count * 5 * 6 + i] = vertices[i];
				}

				count++;
			}
		}
	}

	printf("\n");

	if (vertices != NULL) {
		*vertices = count*6;
	}

	return mesh;
}