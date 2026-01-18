// PolygonizeVolume.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

#include <ioData.h>
#include <MarchingCubes.h>

struct Vec3 { float x, y, z; };
struct Triangle { Vec3 normal; Vec3 v1, v2, v3; uint16_t attr = 0; };

void writeMeshToStl(const std::string& stlFileName, mesh& meshToStore);

int main()
{
	std::string volumeFile(R"(D:\Files\CTLab\SaveVolumeToFile\volumeHeader.uint16_scv)");

	VolumeData vd(volumeFile);
	vd.fillBuffer();

	std::cout << vd.getHeaderString() << std::endl;

	std::vector<unsigned char> buffer = vd.getVolumeDataTex();

	auto header = vd.getHeader();

	const int width = header->recoX;
	const int height = header->recoY;
	const int depth = header->recoZ;

	const double voxSizeX = header->voxSizeX;
	const double voxSizeY = header->voxSizeY;
	const double voxSizeZ = header->voxSizeZ;

	mesh fullMesh;

    for (int z = 0; z < depth; ++z)
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                size_t index0 = x + width * (y + height * z);
				size_t index1 = x + width * (y + height * z);
				size_t index2 = x + width * (y + height * z);
				size_t index3 = x + width * (y + height * z);
				size_t index4 = x + width * (y + height * z);
				size_t index5 = x + width * (y + height * z);
				size_t index6 = x + width * (y + height * z);
				size_t index7 = x + width * (y + height * z);

				//volumeDataTex[dstIdx] = 0;

				mesh localMesh;

				fullMesh.insert(fullMesh.end(), localMesh.begin(), localMesh.end());
            }
        }
    }

	return 0;
}

Vec3 computeNormal(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
	Vec3 u = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
	Vec3 v = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };
	Vec3 n = { u.y * v.z - u.z * v.y,
			   u.z * v.x - u.x * v.z,
			   u.x * v.y - u.y * v.x };
	float len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
	if (len > 0) { n.x /= len; n.y /= len; n.z /= len; }
	return n;
}

void writeMeshToStl(const std::string& stlFileName, mesh& meshToStore)
{
	std::ofstream stl(stlFileName.c_str(), std::ios::binary);
	char header[80] = {};
	stl.write(header, 80);

	size_t count = meshToStore.size();
	stl.write(reinterpret_cast<char*>(&count), sizeof(count));

	for (auto& t : meshToStore)
	{
		Triangle triangle;

		triangle.v1.x = t[0][0];
		triangle.v1.y = t[0][1];
		triangle.v1.z = t[0][2];

		triangle.v2.x = t[1][0];
		triangle.v2.y = t[1][1];
		triangle.v2.z = t[1][2];

		triangle.v3.x = t[2][0];
		triangle.v3.y = t[2][1];
		triangle.v3.z = t[2][2];

		triangle.normal = computeNormal(triangle.v1, triangle.v2, triangle.v3);

		stl.write(reinterpret_cast<char*>(&triangle), sizeof(Triangle));
	}

	stl.close();
}


