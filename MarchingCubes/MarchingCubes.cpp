// MarchingCubes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifdef _DEBUG
#include <iostream>
#include <bitset>
#endif



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

#ifdef _DEBUG
	std::cout << "set: 0b" << std::bitset<8>(cornerSet) << '\n';
#endif

	switch (cornerSet)
	{
	case 0b00000001: //case 1
		edges = { 10,20,40 };
		break;
	case 0b00000010://case 1
		edges = { 10,15,13 };
		break;
	case 0b00000011: //case 2
		edges = { 20,40,15,15,13,20 };
		break;
	case 0b00000100://case 1
		edges = { 20,23,26 };
		break;
	case 0b00000101:// case 2
		edges = {10, 26, 40, 10, 23, 26};
		break;
	case 0b00000110: //case 3
		edges = { 20,23,26,10,15,13 };
		break;
	case 0b00000111:// case 5
		edges = {26,40,15,26,15,23,23,15,13};
		break;
	case 0b00001000://case 1
		edges = { 13,37,23 };
		break;
	case 0b00001001://case 3
		edges = {20,40,10,13,37,23 };
		break;
	case 0b00001010://case 2
		edges = {10,15,23,15,37,23};
		break;
	case 0b00001011://case 5
		edges = {20,40,23, 40,37,23,37,40,15};
		break;
	case 0b00001100://case 2
		edges = {26,20,13,13,37,26};
		break;
	case 0b00001101:// case 5
		edges = { 37,26,40,40,10,37,10,13,37 };
		break;
	case 0b00001110://case 5
		edges = { 10,15,20,20,15,26,26,15,37 };
		break;
	case 0b00001111://case 8
		edges = {40,15,37,40,37,26};
		break;
	case 0b00010000://case 1
		edges = { 40,46,45 };
		break;
	case 0b00010001://case 2
		edges = {10, 46, 45,10, 20,46 };
		break;
	case 0b00010010://case 3
		edges = {46,45,40,15,13,10 };
		break;
	case 0b00010011://case 5
		edges = {46,13,20, 13,46,45,13,45,15 };
		break;
	case 0b00010100://case 3
		edges = {46,45,40,20,23,26};
		break;
	case 0b00010101://case 5
 		edges = {10,23,45,23,26,45,45,26,46};
		break;
	case 0b00010110://case 7
		edges = {46,45,40,10,15,13,26,20,23};
		break;
	case 0b00010111://case 9
		edges = {13,45,15,23,26,46,13,46,45,23,46,13};
		break;
	case 0b00011000:// case 4
		edges = {13,37,23,46,45,40};
		break;
	case 0b00011001:// case 6
		edges = {46,45,10,46,10,20,23,13,37};
		break;
	case 0b00011010://case 6
		edges = { 10,15,37,10,37,23,46,45,40 };
		break;
	case 0b00011011://case 11 14
		edges = {23,37,15,23,15,46,23,46,20,15,45,46};
		break;
	case 0b00011100://case 6
		edges = {26,13,37,13,26,20,46,45,40};
		break;
	case 0b00011101://case 12
		edges = {13,37,26,13,26,46,13,45,10,13,45,46};
		break;
	case 0b00011110://case 12
		edges = {46,45,40,15,37,26,15,26,20,15,20,10,};
		break;
	case 0b00011111://inverted class 5
		edges = { 15,37,26, 15,26,46, 15,46,45 };
		break;
	case 0b00100000://case 1
		edges = { 15,45,57 };
		break;
	case 0b00100001://case 3
		edges = {10,20,40,45,57,15 };
		break;
	case 0b00100010:// case 2
		edges = {10, 57, 13, 10, 45,57};
		break;
	case 0b00100011://case 5
		edges = {20,57,13,20,45,57,20,40,45 };
		break;
	case 0b00100100:// case 4
		edges = {26,20,23,45,57,15};
		break;
	case 0b00100101:// case6
		edges = {40,10,23,40,23,26,45,57,15};
		break;
	case 0b00100110:// case 6
		edges = {45,57,13,45,13,10,20,23,26};
		break;
	case 0b00100111://case 11 14
		edges = {23,26,40,23,40,57,23,57,13,40,45,57};
		break;
	case 0b00101000://case 3
		edges = { 45,57,15,13,37,23 };
		break;
	case 0b00101001://case 7
		edges = {40,10,20,23,13,37,45,57,15,};
		break;
	case 0b00101010://case 5
		edges = {45,23,10, 45,57,23,23,57,37};
		break;
	case 0b00101011://case 9
		edges = {40,45,20,23,57,37,20,57,23,20,45,57, };
		break;
	case 0b00101100://case 6
		edges = {26,13,37,13,26,20,45,57,15};
		break;
	case 0b00101101://case 12
		edges = {45,57,15,37,26,40,37,40,10,37,10,13,};
		break;
	case 0b00101110://case 11 14
		edges = {20,26,37,20,37,45,20,45,10,37,57,45};
		break;
	case 0b00101111://inverted case 5
		edges = { 37,26,40,40,57,37,40,45,57 };
		break;
	case 0b00110000://case 2
		edges = {46,15,40, 15,46,57 };
		break;
	case 0b00110001://case 5
		edges = {20,46,57,20,57,15,20,15,10,};
		break;
	case 0b00110010:// case 5
		edges = {46,57,13,13,40,46,13,10,40 };
		break;
	case 0b00110011://case 8
		edges = {46,13,20,46,57,13};
		break;
	case 0b00110100://case 6
		edges = {46,15,40,46,57,15,26,20,23};
		break;
	case 0b00110101://case 11 14
		edges = {26,23,10,26,10,57,10,57,15,26,57,46};
		break;
	case 0b00110110://case 12
		edges = {20,23,26,13,57,46,13,46,40,13,40,10,};
		break;
	case 0b00110111://inverted case 5
		edges = { 13,46,57,13,26,46,13,23,26 };
		break;
	case 0b00111000://case 6
		edges = {46,15,40,46,57,15,37,23,13};
		break;
	case 0b00111001://case 12
		edges = {37,13,23,57,46,20,57,20,10,57,10,15};
		break;
	case 0b00111010://case 11 14
 		edges = {37,10,15,37,10,46,15,46,40,37,46,57};
		break;
	case 0b00111011://inverted case 5
		edges = { 57,20,46,57,23,20,57,37,23 };
		break;
	case 0b00111100://case 10
		edges = {46,57,15,46,15,40,26,13,37,26,20,13};
		break;
	case 0b00111101://inverted case 6
		edges = { 46,37,26,46,57,37,10,13,15 };
		break;
	case 0b00111110://inverted case 6
		edges = { 46,37,26,46,57,37,20,10,40 };
		break;
	case 0b00111111://inverted case 2
		edges = { 26,46,57,57,37,26 };
		break;
	case 0b01000000://case 1
		edges = { 26,67,46 };
		break;
	case 0b01000001://case 3
		edges = {46,26,67,40,10,20 };
		break;
	case 0b01000010://case 4
		edges = {10,15,13,46,26,67 };
		break;
	case 0b01000011://case 6
		edges = { 40,15,13,13,20,40,46,26,67 };
		break;
	case 0b01000100://case 2
		edges = {20, 23, 67, 20, 67, 46};
		break;
	case 0b01000101://case 5
		edges = {10,23,67,10,67,46,10,46,40};
		break;
	case 0b01000110://case 6
		edges = {46,23,67,46,20,23,10,15,13};
		break;
	case 0b01000111://case 11 14
		edges = {13,15,40,13,40,67,13,67,23,40,46,67};
		break;
	case 0b01001000://case 3
		edges = {46,26,67,37,23,13};
		break;
	case 0b01001001://case 7
		edges = {40,10,20,37,23,13,46,26,67 };
		break;
	case 0b01001010://case 6
		edges = { 10,15,37,10,37,23,46,26,67 };
		break;
	case 0b01001011://case 12
		edges = {46,26,67,40,15,37,40,37,23,40,23,20};
		break;
	case 0b01001100://case 5
		edges = {46,20,13,46,13,37,46,37,67,};
		break;
	case 0b01001101://case 9
		edges = {10,13,46,13,67,46,13,37,67,10,46,40};
		break;
	case 0b01001110://case 11 14
		edges = {10,15,37,37,46,67,46,20,10,10,37,46};
		break;
	case 0b01001111://inverted case 5
		edges = {40,15,37,40,37,67,40,67,46 };
		break;
	case 0b01010000://case 2
		edges = {40, 26, 45, 45, 26, 67};
		break;
	case 0b01010001://case 5
		edges = {10,67,45,10,26,67, 10,20,26 };
		break;
	case 0b01010010://case 6
		edges = {40,67,45,40,26,67,10,15,13};
		break;
	case 0b01010011://case 11 14
		edges = {15,13,20,15,20,67,20,67,26,15,67,45};
		break;
	case 0b01010100://case 5
		edges = {23,67,45,23,45,40,23,40,20};
		break;
	case 0b01010101://case 8
		edges = {10,23,67,10,67,45};
		break;
	case 0b01010110://case 12
		edges = {10,15,13,45,67,23,45,23,20,45,20,40,};
		break;
	case 0b01010111://inverted case 5
		edges = { 23,67,45,23,45,15,23,15,13 };
		break;
	case 0b01011000://case 6
		edges = {40,67,45,40,26,67,13,37,23};
		break;
	case 0b01011001://case 12
		edges = {37,13,23,10,45,67,10,67,26,10,26,20};
		break;
	case 0b01011010://case 10
		edges = {45,26,67,45,40,26,10,37,23,10,15,37, };
		break;
	case 0b01011011://inverted case 6
		edges = { 45,15,37,45,37,67,26,23,20 };
		break;
	case 0b01011100://case 11 14
		edges = {37,20,13,37,20,45,20,45,40, 37,45,67};
		break;
	case 0b01011101://inverted case 5
		edges = { 67,45,10,10,13,67,67,13,37 };
		break;
	case 0b01011110://inverted case 6
		edges = { 45,15,37,45,37,67,40,20,10 };
		break;
	case 0b01011111://inverted case 2
		edges = { 15, 37, 67, 15, 67, 45 };
		break;
	case 0b01100000: //case 3
		edges = {46,26,67, 45,57,15};
		break;
	case 0b01100001://case 7
		edges = {67,46,26,57,15,45,40,10,20 };
		break;
	case 0b01100010://case 6
		edges = {45,57,13,45,13,10,46,26,67};
		break;
	case 0b01100011://case 12
		edges = {46,67,26,20,13,57,20,57,45,20,45,40,};
		break;
	case 0b01100100://case 6
		edges = {46,23,67,46,20,23,45,57,15};
		break;
	case 0b01100101://case 12
		edges = {45,57,15,67,23,10,67,10,40,67,40,46,};
		break;
	case 0b01100110://case 10
		edges = {46,23,67,46,20,23,45,57,13,45,13,10};
		break;
	case 0b01100111://inverted case 6
		edges = { 67,57,13,67,13,23,46,40,45 };
		break;
	case 0b01101000://case 7
		edges = {46,26,67,45,57,15,37,23,13 };
		break;
	case 0b01101001://case 13
		edges = {20,40,10,23,13,37,46,26,67,45,57,15};
		break;
	case 0b01101010://case 12
		edges = { 46,67,26,45,10,23,45,23,37,45,37,57, };
		break;
	case 0b01101011://inverted case 7
		edges = { 46,40,45,67,57,37,26,23,20 };
		break;
	case 0b01101100://case 12
		edges = {45,57,15,13,20,46,13,46,67,13,67,37,};
		break;
	case 0b01101101://inverted case 7
		edges = { 67,57,37,46,40,45,15,10,13 };
		break;
	case 0b01101110://inverted case 6
		edges = { 46,10,45,46,20,10,67,57,37 };
		break;
	case 0b01101111://inverted case 3
		edges = { 46,40,45,67,57,37 };
		break;
	case 0b01110000://case 5
		edges = {26,15,40,15,26,67,15,67,57};
		break;
	case 0b01110001://case 9
		edges = {15,10,57,26,67,20,20,67,57,10,20,57};
		break;
	case 0b01110010://case 11 14
		edges = {67,40,26,67,40,13,67,13,57,40,10,13};
		break;
	case 0b01110011://inverted case 5
		edges = { 20,57,13,20,67,57,20,26,67 };
		break;
	case 0b01110100://case 11 14
		edges = {57,15,40,57,40,23,40,20,23,57,23,67};
		break;
	case 0b01110101://inverted case 5
		edges = { 10,23,67,10,67,57,10,57,15 };
		break;
	case 0b01110110://inverted case 6
		edges = { 67,57,13,67,13,23,40,20,10 };
		break;
	case 0b01110111://inverted case 2
		edges = { 13, 67,57, 13, 23,67 };
		break;
	case 0b01111000://case 12
		edges = {23,13,37,26,40,15,26,15,57,26,57,67,};
		break;
	case 0b01111001://inverted case 7
		edges = { 67,57,37,26,23,20,15,10,13 };
		break;
	case 0b01111010://inverted case 6
		edges = { 40,23,10,40,26,23,67,57,37 };
		break;
	case 0b01111011://inverted case 3
		edges = { 67,57,37,26,23,20 };
		break;
	case 0b01111100://inverted case 6
		edges = { 40,13,15,13,40,20,67,57,37 };
		break;
	case 0b01111101://inverted case 3
		edges = { 67,57,37,15,10,13 };
		break;
	case 0b01111110://inverted case 4
		edges = { 20,10,40,67,57,37 };
		break;
	case 0b01111111://inverted class 1
		edges = {67,57,37};
		break;
	case 0b10000000://case 1
		edges = { 37,57,67 };
		break;
	case 0b10000001://case 4
		edges = {20,40,10,67,37,57 };
		break;
	case 0b10000010://case 3
		edges = {67,37,57,15,13,10};
		break;
	case 0b10000011://case 6
		edges = {40,15,13,13,20,40,67,37,57};
		break;
	case 0b10000100://case 3
		edges = {67,37,57, 26,20,23};
		break;
	case 0b10000101://case 6
		edges = {40,10,23,40,23,26,67,37,57};
		break;
	case 0b10000110://case 7
		edges = {67,37,57,26,20,23,15,13,10};
		break;
	case 0b10000111://case 12
		edges = {67,57,37,26,40,15,26,15,13,26,13,23,};
		break;
	case 0b10001000://case 2
		edges = {13, 57, 67, 13, 67, 23};
		break;
	case 0b10001001:// case 6
		edges = {67,13,57,67,23,13,40,10,20};
		break;
	case 0b10001010://case 5
		edges = {10,67,23, 10,57,67,10,15,57 };
		break;
	case 0b10001011://case 11 14
		edges = {20,40,15,23,15,57,23,57,67,20,23,15};
		break;
	case 0b10001100://case 5
		edges = {20,13,57,20,57,67,20,67,26,};
		break;
	case 0b10001101://case 11 14
		edges = {10,40,26,10,26,57,10,57,13,26,67,57};
		break;
	case 0b10001110://case 9
		edges = {10,15,57,20,67,26,10,57,20,20,57,67};
		break;
	case 0b10001111://inverted case 5
		edges = { 26,40,15,15,67,26,15,57,67 };
		break;
	case 0b10010000:// case 3
		edges = {46,45,40,67,37,57 };
		break;
	case 0b10010001://case 6
		edges = {46,45,10,46,10,20,67,37,57};
		break;
	case 0b10010010://case 7
		edges = {67,37,57,46,45,40,15,13,10 };
		break;
	case 0b10010011://case 12
		edges = {67,57,37,46,20,13,46,13,15,46,15,45};
		break;
	case 0b10010100://case 7
		edges = {46,45,40,67,37,57,26,20,23 };
		break;
	case 0b10010101://case 12
		edges = {67,57,37,23,10,45,23,45,46,23,46,26,};
		break;
	case 0b10010110://case 13
		edges = {46,45,40,67,37,57,26,20,23,10,15,13};
		break;
	case 0b10010111://inverted case 7
		edges = { 46,67,26,45,15,57,37,13,23 };
		break;
	case 0b10011000://case 6
		edges = {67,13,57,67,23,13,46,45,40};
		break;
	case 0b10011001://case 10
		edges = {46,45,10,46,10,20,67,13,57,67,23,13, };
		break;
	case 0b10011010://case 12
		edges = {46,45,40,10,23,67,10,67,57,10,57,15};
		break;
	case 0b10011011://inverted case 6
		edges = { 46,67,23,46,23,20,45,15,57 };
		break;
	case 0b10011100://case 12
		edges = {46,45,40,20,13,57,20,57,67,20,67,26,};
		break;
	case 0b10011101://inverted case 6
		edges = { 45,13,57,45,10,13,46,67,26 };
		break;
	case 0b10011110://inverted case 7
		edges = { 67,26,46,57,45,15,40,20,10 };
		break;
	case 0b10011111://inverted case 3
		edges = { 46,67,26,45,15,57 };
		break;
	case 0b10100000://case 2
		edges = {15, 67, 37, 15, 45, 67};
		break;
	case 0b10100001://case 6
		edges = {45,37,15,45,67,37,40,10,20};
		break;
	case 0b10100010:// case 5
		edges = {67,10,45,10,67,13,67,37,13 };
		break;
	case 0b10100011://case 11 14
		edges = {40,20,13,40,13,67,13,67,37,40,67,45};
		break;
	case 0b10100100://case 6
		edges = {45,37,15,45,67,37,26,20,23};
		break;
	case 0b10100101://case 10
		edges = {45,67,37,45,37,15,40,23,26,40,10,23};
		break;
	case 0b10100110://case 12
		edges = {20,23,26,67,45,10,67,10,13,67,13,37};
		break;
	case 0b10100111://inverted case 6
		edges = { 40,45,67,40,67,26,13,23,37 };
		break;
	case 0b10101000:// case 5
		edges = {23,45,67,23,15,45,23,13,15 }; 
		break;
	case 0b10101001://case 12
		edges = {40,20,10,23,67,45,23,45,15,23,15,13,};
		break;
	case 0b10101010://case 8
		edges = { 10,67,23,10,45,67  };
		break;
	case 0b10101011://inverted case 5
		edges = { 23,45,67,23,40,45,23,20,40 };
		break;
	case 0b10101100://case 11 14
		edges = {15,67,45,15,67,20,67,20,26,15,20,13};
		break;
	case 0b10101101://inverted case 6
		edges = { 40,45,67,40,67,26,10,13,15 };
		break;
	case 0b10101110://inverted case 5
		edges = { 10,45,67,10,67,26,10,26,20 };
		break;
	case 0b10101111://inverted case 2
		edges = { 40, 45, 26, 45, 67, 26 };
		break;
	case 0b10110000://case 5
		edges = {40,37,15,40,67,37,40,46,67 };
		break;
	case 0b10110001://case 11 14
		edges = {67,37,15,67,15,20,15,20,10,67,20,46};
		break;
	case 0b10110010://case 9
		edges = {46,10,40,67,37,13,10,46,67,10,67,13};
		break;
	case 0b10110011://inverted case 5
		edges = { 46,13,20,46,37,13,46,67,37 };
		break;
	case 0b10110100://case 12
		edges = {26,20,23,40,15,37,40,37,67,40,67,46};
		break;
	case 0b10110101://inverted case 6
		edges = { 10,37,15,10,23,37,46,67,26 };
		break;
	case 0b10110110://inverted case 7
		edges = { 40,20,10,37,13,23,46,67,26 };
		break;
	case 0b10110111://inverted case 3
		edges = { 46,67,26,37,13,23 };
		break;
	case 0b10111000://case 11 14
		edges = {46,40,15,46,15,23,15,23,13,46,67,23};
		break;
	case 0b10111001://inverted case 6
		edges = { 46,67,23,46,23,20,10,13,15 };
		break;
	case 0b10111010://inverted case 5
		edges = { 10,67,23,10,46,67,10,40,46 };
		break;
	case 0b10111011://inverted case 2
		edges = { 20, 67, 23, 20, 46,67};
		break;
	case 0b10111100://inverted case 6
		edges = { 40,13,15,13,40,20,46,67,26 };
		break;
	case 0b10111101://inverted case 4
		edges = { 10,13,15, 46,67,26 };
		break;
	case 0b10111110://inverted case 3
		edges = { 46,67,26, 40,20,10 };
		break;
	case 0b10111111://inverted class 1
		edges = {46,67,26};
		break;
	case 0b11000000://case 2
		edges = {26,57,46, 57,26,37};
		break;
	case 0b11000001://case 6
		edges = {46,26,37,46,37,57,20,40,10};
		break;
	case 0b11000010://case 6
		edges = {46,26,37,46,37,57,10,15,13};
		break;
	case 0b11000011://case 10
		edges = {46,37,57,46,26,37,40,15,13,40,13,20};
		break;
	case 0b11000100://case 5
		edges = {57,46,20,57,20,23,57,23,37,};
		break;
	case 0b11000101://case 11 14
		edges = {40,10,23,40,23,57,23,57,37,40,57,46};
		break;
	case 0b11000110://case 12
		edges = {10,15,13,20,46,57,20,57,37,20,37,23,};
		break;
	case 0b11000111://inverted case 6
		edges = { 46,40,15,46,15,57,37,13,23 };
		break;
	case 0b11001000://case 5
		edges = {13,57,46,13,46,26,13,26,23,};
		break;
	case 0b11001001://case 12
		edges = {20,10,40,13,57,46,13,46,26,13,26,23,};
		break;
	case 0b11001010://case 11 14
		edges = {15,10,23,15,23,46,15,46,57,23,46,26};
		break;
	case 0b11001011://inverted case 6
		edges = { 46,40,15,46,15,57,26,23,20 };
		break;
	case 0b11001100://case 8
		edges = {20,13,57,20,57,46,};
		break;
	case 0b11001101://inverted case 5
		edges = { 46,13,57,13,46,40,13,40,10 };
		break;
	case 0b11001110://inverted case 5
		edges = { 20,57,46,20,15,57,20,10,15 };
		break;
	case 0b11001111://inverted case 2
		edges = { 46,40,15,15,57,46 };
		break;
	case 0b11010000://case 5
		edges = {37,40,26,40,37,57,40,57,45};
		break;
	case 0b11010001://case 11 14
		edges = {57,37,26,57,26,10,57,45,10,26,20,10};
		break;
	case 0b11010010://case 12
		edges = {10,13,15,37,26,40,37,40,45,37,45,57,};
		break;
	case 0b11010011://inverted case 6
		edges = { 26,37,13,13,20,26,45,15,57 };
		break;
	case 0b11010100://case 9
		edges = {20,45,40,23,37,57,20,57,45,20,23,57};
		break;
	case 0b11010101://inverted case 5
		edges = { 45,10,23, 45,23,57,23,37,57 };
		break;
	case 0b11010110://inverted case 7
		edges = { 40,20,10,23,37,13,45,15,57 };
		break;
	case 0b11010111://inverted case 3
		edges = { 45,15,57, 13,23,37 };
		break;
	case 0b11011000://case 11 14
		edges = {45,40,26,45,26,13,45,57,13,26,23,13};
		break;
	case 0b11011001://inverted case 6
		edges = { 45,13,57,45,10,13,20,26,23};
		break;
	case 0b11011010://inverted case 6
		edges = { 40,23,10,40,26,23,45,15,57 };
		break;
	case 0b11011011://inverted case 4
		edges = { 26,23,20,45,15,57 };
		break;
	case 0b11011100://inverted case 5
		edges = { 20,13,57,20,57,45,20,45,40, };
		break;
	case 0b11011101://inverted case 2
		edges = { 10, 13, 57, 10, 57, 45 };
		break;
	case 0b11011110://inverted case 3
		edges = { 10,40,20,45,15,57 };
		break;
	case 0b11011111://inverted class 1
		edges = {45,15,57};
		break;
	case 0b11100000://case 5
		edges = {15,26,37, 15,46,26, 15,45,46 };
		break;
	case 0b11100001://case 12
		edges = {40,20,10,15,37,26,15,26,46,15,46,26,};
		break;
	case 0b11100010://case 11 14
		edges = {46,26,37,46,37,10,37,13,10,46,45,10};
		break;
	case 0b11100011://inverted case 6
		edges = { 26,37,13,13,20,26,46,40,45 };
		break;
	case 0b11100100://case 11 14
		edges = {45,15,37,45,37,20,45,20,46,37,20,23};
		break;
	case 0b11100101://inverted case 6
		edges = { 10,37,15,10,23,37,46,40,45 };
		break;
	case 0b11100110://inverted case 6
		edges = { 46,10,45,46,20,10,23,37,13 };
		break;
	case 0b11100111://inverted case 4
		edges = { 13,23,37,46,40,45 };
		break;
	case 0b11101000://case 9
		edges = {26,23,46,15,45,13,45,46,13,46,23,13};
		break;
	case 0b11101001://inverted case 7
		edges = { 46,40,45,10,13,15,26,23,20 };
		break;
	case 0b11101010://inverted case 5
		edges = { 10,45,23,23,45,26,45,46,26, };
		break;
	case 0b11101011://inverted case 3
		edges = { 46,40,45,20, 26,23 };
		break;
	case 0b11101100://inverted case 5
		edges = { 46,20,13,13,45,46,13,15,45 };
		break;
	case 0b11101101://inverted case 3
		edges = { 46,40,45, 15,10,13 };
		break;
	case 0b11101110://inverted case 2
		edges = { 10, 45, 46, 10, 46, 20 };
		break;
	case 0b11101111://inverted class 1
		edges = {46,40,45};
		break;
	case 0b11110000://case 8
		edges = { 40,37,15,40,26,37 };
		break;
	case 0b11110001://inverted case 5
		edges = { 10,20,15,20,26,15,26,37,15 };
		break;
	case 0b11110010://inverted case 5
		edges = { 37,40,26,40,37,10,10,37,13 };
		break;
	case 0b11110011://inverted case 2
		edges = { 26,13,20,13,26,37 };
		break;
	case 0b11110100://inverted case 5
		edges = { 20,23,40, 40,23,37,37,15,40 };
		break;
	case 0b11110101://inverted case 2
		edges = { 10,23,15, 15,23,37 };
		break;
	case 0b11110110://inverted case 3
		edges = { 20,10,40,13,23,37 };
		break;
	case 0b11110111://inverted class 1
		edges = {13,23,37};
		break;
	case 0b11111000://inverted case 5
		edges = { 26,15,40,26,23,15,23,13,15 };
		break;
	case 0b11111001://inverted case 3
		edges = { 20,26,23,10,13,15 };
		break;
	case 0b11111010://inverted case 2
		edges = { 10, 40, 26, 10, 26,23 };
		break;
	case 0b11111011://inverted case 1
		edges = {20,26,23 };
		break;
	case 0b11111100://inverted case 2
		edges = { 20,15,40,15,20,13 };
		break;
	case 0b11111101://inverted case 1
		edges = {10,13,15};
		break;
	case 0b11111110://inverted case 1
		edges = {40,20,10};
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


