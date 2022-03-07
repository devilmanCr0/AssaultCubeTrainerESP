#include "geom.h"
#include <windows.h>

bool WorldToScreen(Vector3L pos, Vector3L& screen, float matrix[16], int windowWidth, int windowHeight)
{
	Vec4 clipCoords; // start off with a 4 Vector clip coord from the matrix
	/*
	Imagine this __									   __
				|	x1			x2			x3		x4	 |
				|										 |	
				|	y1			y2			y3		y4	 |
				|										 |
				|	z1			z2			z3		z4	 |
				|__	transform1	transform2	tran3	t4 __|
				
	This is a matrix that would be expressed by opengl's graphic api
	they are three column-structured vertexes that are used to then transform clip coordinates to 
	screen coordinates, allowing us to draw shapes relative to what is positioned on the screen

	aquiring the PVM (projection viewmodel) requires multiplying the matrixices using standard matrix conventions
	*/


	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12]; //first vertex row
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13]; //second vertex row
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14]; //third vertex row
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15]; //fourth vertex row


	if (clipCoords.w < 0.1f)
	{
		return false;
	}


	//We are now dividing the clip coordinates with the transform numbers (Perspective division) to acquire the perspective im  guessing lol
	//this might be viewport transformation
	Vector3L NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	//adjusts with the windowscreen
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	return true;

}