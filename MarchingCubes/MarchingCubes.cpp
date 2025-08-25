// MarchingCubes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <MarchingCubes.h>

std::array<float, 3> operator+(const std::array<float, 3>& f1, const std::array<float, 3>& f2)
{
	return { f1[0] + f2[0],
			f1[1] + f2[1],
			f1[2] + f2[2] };
}

std::array<float, 3> operator/(const std::array<float, 3>& f1, float d)
{
	return { f1[0]/d,
			f1[1]/d,
			f1[2]/d};
}



/*
		6 ________67_________ 7
	   /|				     /|
	  / |                   / |
	 46 |                 57  |
	/   |                 /   |
   /    26               /    |
  /		|				/     37
4 ______|___45________ 5      |
|       |              |      |
|       |              |      |
|       2 ________23___|_____ 3
|      /               |      /
40    /                15    /
|    20                |   13
|   /                  |   /
|  /                   |  /
| /                    | /
 0 ________10_________ 1

 */
/// <summary>
/// This function returns an array of triangle edges included in mesh
/// </summary>
/// <param name="cell">8 edges of cell</param>
/// <param name="innerRange">range of values that belong to an object</param>
/// <returns>mesh as an array of triangles</returns>
mesh getTriangles(const std::vector<std::pair<std::array<float, 3>, unsigned short>>& cell, std::pair<unsigned short, unsigned short>& innerRange, int &cornerSet)
{
	if (cell.size() != 8)
	{
		throw std::exception("cell must consist of 8 points");
	}

	mesh trianglesArray;

	int count = 0;

	for (const auto& c : cell)
	{
		//this is inner point
		if (c.second >= innerRange.first && c.second <= innerRange.second)
		{
			cornerSet |= (1 << count);
		}

		count++;
	}

	std::vector<int> edges;

	switch (cornerSet)
	{
	case 0b00000001: //case 1
		edges = { 10,20,40 };
		break;
	case 0b00000010://case 1
		edges = { 10,13,15 };
		break;
	case 0b00000011: //case 2
		edges = { 20,40,15,15,13,20 };
		break;
	case 0b00000100://case 1
		edges = { 20,23,26 };
		break;
	case 0b00000101:
		edges = {10, 26, 40, 10, 23, 26};
		break;
	case 0b00000110: //case 2
		edges = { 57,45,10,57,10,13 };
		break;
	case 0b00000111:
		edges = {26,40,15,26,15,23,23,15,13};
		break;
	case 0b00001000://case 1
		edges = { 13,37,23 };
		break;
	case 0b00001001:
		edges = {};
		break;
	case 0b00001010:
		edges = {10,15,23,15,37,23};
		break;
	case 0b00001011:
		edges = {20,23,40, 40,23,37,37,40,15};
		break;
	case 0b00001100://case 2
		edges = {26,20,13,13,37,26};
		break;
	case 0b00001101:
		edges = { 37,26,40,40,10,37,10,37,13};
		break;
	case 0b00001110:
		edges = { 10,15,20,20,15,26,26,15,37 };
		break;
	case 0b00001111:
		edges = {40,15,37,40,37,26};
		break;
	case 0b00010000://case 1
		edges = { 40,45,46 };
		break;
	case 0b00010001:
		edges = {10, 45, 46, 10, 46, 20};
		break;
	case 0b00010010:
		edges = {};
		break;
	case 0b00010011:
		edges = {};
		break;
	case 0b00010100:
		edges = {};
		break;
	case 0b00010101:
 		edges = {10,23,45,23,45,26,45,26,46};
		break;
	case 0b00010110:
		edges = {};
		break;
	case 0b00010111:
		edges = {13,15,45,23,26,46,13,45,46,23,13,46};
		break;
	case 0b00011000:
		edges = {13,37,23,46,45,40};
		break;
	case 0b00011001:
		edges = {};
		break;
	case 0b00011010:
		edges = {};
		break;
	case 0b00011011:
		edges = {};
		break;
	case 0b00011100:
		edges = {};
		break;
	case 0b00011101:
		edges = {};
		break;
	case 0b00011110:
		edges = {};
		break;
	case 0b00011111:
		edges = {};
		break;
	case 0b00100000://case 1
		edges = { 15,45,57 };
		break;
	case 0b00100001:
		edges = {};
		break;
	case 0b00100010:
		edges = {10, 13, 57, 10, 57, 45};
		break;
	case 0b00100011:
		edges = {};
		break;
	case 0b00100100:
		edges = {26,20,23,45,57,15};
		break;
	case 0b00100101:
		edges = {};
		break;
	case 0b00100110:
		edges = {};
		break;
	case 0b00100111:
		edges = {};
		break;
	case 0b00101000:
		edges = {};
		break;
	case 0b00101001:
		edges = {};
		break;
	case 0b00101010:
		edges = {45,10,23, 45,23,57,23,57,37};
		break;
	case 0b00101011:
		edges = {40,45,20,23,37,57,20,23,57,20,57,45};
		break;
	case 0b00101100:
		edges = {};
		break;
	case 0b00101101:
		edges = {};
		break;
	case 0b00101110:
		edges = {};
		break;
	case 0b00101111:
		edges = {};
		break;
	case 0b00110000://case 2
		edges = {46,40,15,15,57,46};
		break;
	case 0b00110001:
		edges = {};
		break;
	case 0b00110010:
		edges = {};
		break;
	case 0b00110011:
		edges = {};
		break;
	case 0b00110100:
		edges = {};
		break;
	case 0b00110101:
		edges = {};
		break;
	case 0b00110110:
		edges = {};
		break;
	case 0b00110111:
		edges = {};
		break;
	case 0b00111000:
		edges = {};
		break;
	case 0b00111001:
		edges = {};
		break;
	case 0b00111010:
		edges = {};
		break;
	case 0b00111011:
		edges = {};
		break;
	case 0b00111100:
		edges = {};
		break;
	case 0b00111101:
		edges = {};
		break;
	case 0b00111110:
		edges = {};
		break;
	case 0b00111111:
		edges = {};
		break;
	case 0b01000000://case 1
		edges = { 26,67,46 };
		break;
	case 0b01000001:
		edges = {};
		break;
	case 0b01000010:
		edges = {10,15,13,46,67,26};
		break;
	case 0b01000011:
		edges = {};
		break;
	case 0b01000100:
		edges = {20, 23, 67, 20, 67, 46};
		break;
	case 0b01000101:
		edges = {10,23,67,10,67,46,10,46,40};
		break;
	case 0b01000110:
		edges = {};
		break;
	case 0b01000111:
		edges = {};
		break;
	case 0b01001000:
		edges = {};
		break;
	case 0b01001001:
		edges = {};
		break;
	case 0b01001010:
		edges = {};
		break;
	case 0b01001011:
		edges = {};
		break;
	case 0b01001100:
		edges = {};
		break;
	case 0b01001101:
		edges = {10,13,46,13,46,67,13,67,37,10,46,40};
		break;
	case 0b01001110:
		edges = {};
		break;
	case 0b01001111:
		edges = {};
		break;
	case 0b01010000:
		edges = {40, 45, 26, 45, 67, 26};
		break;
	case 0b01010001:
		edges = {10,67,45,10,67,26,10,26,20};
		break;
	case 0b01010010:
		edges = {};
		break;
	case 0b01010011:
		edges = {};
		break;
	case 0b01010100:
		edges = {23,67,45,23,45,40,23,40,20};
		break;
	case 0b01010101:
		edges = {10,23,67,10,67,45};
		break;
	case 0b01010110:
		edges = {};
		break;
	case 0b01010111:
		edges = {};
		break;
	case 0b01011000:
		edges = {};
		break;
	case 0b01011001:
		edges = {};
		break;
	case 0b01011010:
		edges = {};
		break;
	case 0b01011011:
		edges = {};
		break;
	case 0b01011100:
		edges = {};
		break;
	case 0b01011101:
		edges = {};
		break;
	case 0b01011110:
		edges = {};
		break;
	case 0b01011111:
		edges = {};
		break;
	case 0b01100000: 
		edges = {};
		break;
	case 0b01100001:
		edges = {};
		break;
	case 0b01100010:
		edges = {};
		break;
	case 0b01100011:
		edges = {};
		break;
	case 0b01100100:
		edges = {};
		break;
	case 0b01100101:
		edges = {};
		break;
	case 0b01100110:
		edges = {};
		break;
	case 0b01100111:
		edges = {};
		break;
	case 0b01101000:
		edges = {};
		break;
	case 0b01101001:
		edges = {};
		break;
	case 0b01101010:
		edges = {};
		break;
	case 0b01101011:
		edges = {};
		break;
	case 0b01101100:
		edges = {};
		break;
	case 0b01101101:
		edges = {};
		break;
	case 0b01101110:
		edges = {};
		break;
	case 0b01101111:
		edges = {};
		break;
	case 0b01110000:
		edges = {26,40,15,15,26,67,15,67,57};
		break;
	case 0b01110001:
		edges = {15,57,10,26,67,20,20,57,67,10,57,20};
		break;
	case 0b01110010:
		edges = {};
		break;
	case 0b01110011:
		edges = {};
		break;
	case 0b01110100:
		edges = {};
		break;
	case 0b01110101:
		edges = {};
		break;
	case 0b01110110:
		edges = {};
		break;
	case 0b01110111:
		edges = {};
		break;
	case 0b01111000:
		edges = {};
		break;
	case 0b01111001:
		edges = {};
		break;
	case 0b01111010:
		edges = {};
		break;
	case 0b01111011:
		edges = {};
		break;
	case 0b01111100:
		edges = {};
		break;
	case 0b01111101:
		edges = {};
		break;
	case 0b01111110:
		edges = {};
		break;
	case 0b01111111:
		edges = {};
		break;
	case 0b10000000://case 1
		edges = { 37,57,67 };
		break;
	case 0b10000001:
		edges = {20,10,40,67,57,37};
		break;
	case 0b10000010:
		edges = {};
		break;
	case 0b10000011:
		edges = {};
		break;
	case 0b10000100:
		edges = {};
		break;
	case 0b10000101:
		edges = {};
		break;
	case 0b10000110:
		edges = {};
		break;
	case 0b10000111:
		edges = {};
		break;
	case 0b10001000:
		edges = {13, 57, 67, 13, 67, 23};
		break;
	case 0b10001001:
		edges = {};
		break;
	case 0b10001010:
		edges = {10,23,67,10,67,57,10,57,15};
		break;
	case 0b10001011:
		edges = {};
		break;
	case 0b10001100:
		edges = {};
		break;
	case 0b10001101:
		edges = {};
		break;
	case 0b10001110:
		edges = {10,15,57,20,26,67,10,57,20,20,57,67};
		break;
	case 0b10001111:
		edges = {};
		break;
	case 0b10010000:
		edges = {};
		break;
	case 0b10010001:
		edges = {};
		break;
	case 0b10010010:
		edges = {};
		break;
	case 0b10010011:
		edges = {};
		break;
	case 0b10010100:
		edges = {};
		break;
	case 0b10010101:
		edges = {};
		break;
	case 0b10010110:
		edges = {};
		break;
	case 0b10010111:
		edges = {};
		break;
	case 0b10011000:
		edges = {};
		break;
	case 0b10011001:
		edges = {};
		break;
	case 0b10011010:
		edges = {};
		break;
	case 0b10011011:
		edges = {};
		break;
	case 0b10011100:
		edges = {};
		break;
	case 0b10011101:
		edges = {};
		break;
	case 0b10011110:
		edges = {};
		break;
	case 0b10011111:
		edges = {};
		break;
	case 0b10100000:
		edges = {15, 37, 67, 15, 67, 45};
		break;
	case 0b10100001:
		edges = {};
		break;
	case 0b10100010:
		edges = {67,45,10,10,67,13,67,13,37};
		break;
	case 0b10100011:
		edges = {};
		break;
	case 0b10100100:
		edges = {};
		break;
	case 0b10100101:
		edges = {};
		break;
	case 0b10100110:
		edges = {};
		break;
	case 0b10100111:
		edges = {};
		break;
	case 0b10101000:
		edges = {23,67,45,23,45,15,23,15,13};
		break;
	case 0b10101001:
		edges = {};
		break;
	case 0b10101010:
		edges = { 10,23,67,10,67,45 };
		break;
	case 0b10101011:
		edges = {};
		break;
	case 0b10101100:
		edges = {};
		break;
	case 0b10101101:
		edges = {};
		break;
	case 0b10101110:
		edges = {};
		break;
	case 0b10101111:
		edges = {};
		break;
	case 0b10110000:
		edges = {40,15,37,40,37,67,40,67,46};
		break;
	case 0b10110001:
		edges = {};
		break;
	case 0b10110010:
		edges = {46,40,10,67,37,13,10,46,67,10,13,67};
		break;
	case 0b10110011:
		edges = {};
		break;
	case 0b10110100:
		edges = {};
		break;
	case 0b10110101:
		edges = {};
		break;
	case 0b10110110:
		edges = {};
		break;
	case 0b10110111:
		edges = {};
		break;
	case 0b10111000:
		edges = {};
		break;
	case 0b10111001:
		edges = {};
		break;
	case 0b10111010:
		edges = {};
		break;
	case 0b10111011:
		edges = {};
		break;
	case 0b10111100:
		edges = {};
		break;
	case 0b10111101:
		edges = {};
		break;
	case 0b10111110:
		edges = {};
		break;
	case 0b10111111:
		edges = {};
		break;
	case 0b11000000://case 2
		edges = {26,46,57,57,37,26};
		break;
	case 0b11000001:
		edges = {};
		break;
	case 0b11000010:
		edges = {};
		break;
	case 0b11000011:
		edges = {};
		break;
	case 0b11000100:
		edges = {};
		break;
	case 0b11000101:
		edges = {};
		break;
	case 0b11000110:
		edges = {};
		break;
	case 0b11000111:
		edges = {};
		break;
	case 0b11001000:
		edges = {};
		break;
	case 0b11001001:
		edges = {};
		break;
	case 0b11001010:
		edges = {};
		break;
	case 0b11001011:
		edges = {};
		break;
	case 0b11001100:
		edges = {};
		break;
	case 0b11001101:
		edges = {};
		break;
	case 0b11001110:
		edges = {};
		break;
	case 0b11001111:
		edges = {};
		break;
	case 0b11010000:
		edges = {37,26,40,40,37,57,40,57,45};
		break;
	case 0b11010001:
		edges = {};
		break;
	case 0b11010010:
		edges = {};
		break;
	case 0b11010011:
		edges = {};
		break;
	case 0b11010100:
		edges = {20,40,45,23,37,57,20,45,57,20,57,23};
		break;
	case 0b11010101:
		edges = {};
		break;
	case 0b11010110:
		edges = {};
		break;
	case 0b11010111:
		edges = {};
		break;
	case 0b11011000:
		edges = {};
		break;
	case 0b11011001:
		edges = {};
		break;
	case 0b11011010:
		edges = {};
		break;
	case 0b11011011:
		edges = {};
		break;
	case 0b11011100:
		edges = {};
		break;
	case 0b11011101:
		edges = {};
		break;
	case 0b11011110:
		edges = {};
		break;
	case 0b11011111:
		edges = {};
		break;
	case 0b11100000:
		edges = {15,37,26, 15,26,46, 15,46,45};
		break;
	case 0b11100001:
		edges = {};
		break;
	case 0b11100010:
		edges = {};
		break;
	case 0b11100011:
		edges = {};
		break;
	case 0b11100100:
		edges = {};
		break;
	case 0b11100101:
		edges = {};
		break;
	case 0b11100110:
		edges = {};
		break;
	case 0b11100111:
		edges = {};
		break;
	case 0b11101000:
		edges = {26,46,23,15,13,45,45,13,46,46,13,23};
		break;
	case 0b11101001:
		edges = {};
		break;
	case 0b11101010:
		edges = {};
		break;
	case 0b11101011:
		edges = {};
		break;
	case 0b11101100:
		edges = {};
		break;
	case 0b11101101:
		edges = {};
		break;
	case 0b11101110:
		edges = {};
		break;
	case 0b11101111:
		edges = {};
		break;
	case 0b11110000:
		edges = { 40,15,37,40,37,26 };
		break;
	case 0b11110001:
		edges = {};
		break;
	case 0b11110010:
		edges = {};
		break;
	case 0b11110011:
		edges = {};
		break;
	case 0b11110100:
		edges = {};
		break;
	case 0b11110101:
		edges = {};
		break;
	case 0b11110110:
		edges = {};
		break;
	case 0b11110111:
		edges = {};
		break;
	case 0b11111000:
		edges = {};
		break;
	case 0b11111001:
		edges = {};
		break;
	case 0b11111010:
		edges = {};
		break;
	case 0b11111011:
		edges = {};
		break;
	case 0b11111100:
		edges = {};
		break;
	case 0b11111101:
		edges = {};
		break;
	case 0b11111110:
		edges = {};
		break;
	case 0b11111111:
		edges = {};
		break;
	default: //case 0b00000000: case 0
		edges = {};
		break;
	}

	if (!(edges.size() == 0 || edges.size() % 3 != 0))
	{
		for (int ii = 0; ii < edges.size() / 3; ii++)
		{
			std::array<std::array<float, 3>, 3> triangle;

			for (int jj = 0; jj < 3; jj++)
			{
				int edge = edges[ii * 3 + jj];

				triangle[jj] = (cell[edge%10].first + cell[edge/10].first) / 2;
			}

			trianglesArray.push_back(triangle);
		}
	}

	return trianglesArray;
}


