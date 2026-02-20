#include <algorithm>
#include <format>
#include <fstream>
#include <sstream>
#include <vector>

#include <ioData.h>

VolumeData::VolumeData(const std::string filePath)
{
    std::ifstream file(filePath.c_str(), std::ios::binary);

    if (file.is_open())
    {
        // Go to end to find file size
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read into buffer
        size_t headerSize = 1024;
        file.read(reinterpret_cast<char*>(&header), headerSize);

        size -= headerSize;
        data.resize(size / 2);

        file.read(reinterpret_cast<char*>(data.data()), size);
    }
}

const uint16_csv_volume* VolumeData::getHeader() const
{
    return &header;
}

void VolumeData::saveHeaderToFile(std::string fileName)
{
    std::ofstream toFile(fileName, std::ios::trunc);
    toFile << getHeaderString();
}

std::string VolumeData::getHeaderString() const
{
    std::ostringstream oss;

    oss << std::format("headSize: {}\n", header.headSize);
    //unsigned int mirrorZ;// : uint32(bytes 4 - 7) : UNKNOWN what this is or represents
    oss << std::format("mirrorZ: {}\n", header.mirrorZ);
    // : uint32(bytes 8 - 11) : number of bits per voxel
    oss << std::format("numBitsVox: {}\n", header.numBitsVox);
    // : uint32(bytes 12 - 15) : size of volume in x - dimension
    oss << std::format("recoX: {}\n", header.recoX);
    // : uint32(bytes 16 - 19) : size of volume in y - dimension
    oss << std::format("recoY: {}\n", header.recoY);
    // : uint32(bytes 20 - 23) : size of volume in z - dimension
    oss << std::format("recoZ: {}\n", header.recoZ);
    //: float64(bytes 24 - 31) : size of voxel in x
    oss << std::format("voxSizeX: {}\n", header.voxSizeX);
    // : float64(bytes 32 - 39) : size of voxel in y
    oss << std::format("voxSizeY: {}\n", header.voxSizeY);
    // : float64(bytes 40 - 47) : size of voxel in z
    oss << std::format("voxSizeZ: {}\n", header.voxSizeZ);
    // : float64(bytes 48 - 55) : minimum gray - scale value as determined by CERA
    oss << std::format("minValue: {}\n", header.minValue);
    // : float64(bytes 56 - 63) : maximum gray - scale value as determined by CERA
    oss << std::format("maxValue: {}\n", header.maxValue);
    // : float64(bytes 64 - 71) : beam center in x - dimension
    oss << std::format("tubeX: {}\n", header.tubeX);
    // : float64(bytes 72 - 79) : beam center in y - dimension
    oss << std::format("tubeY: {}\n", header.tubeY);
    // : float64(bytes 80 - 87) : beam center in z - dimension
    oss << std::format("tubeZ: {}\n", header.tubeZ);
    // : int32(bytes 88 - 91) : x - ray tube current
    oss << std::format("tubeCurrent: {}\n", header.tubeCurrent);
    //: int32(bytes 92 - 95) : x - ray tube voltage
    oss << std::format("tubeVoltage: {}\n", header.tubeVoltage);
    // : float64(bytes 96 - 103) : rotary table X - position
    oss << std::format("rtDataX: {}\n", header.rtDataX);
    // : float64(bytes 104 - 111) : rotary table Y - position
    oss << std::format("rtDataY: {}\n", header.rtDataY);
    // : float64(bytes 112 - 119) : rotary table Z - position
    oss << std::format("rtDataZ: {}\n", header.rtDataZ);
    // : float64(bytes 120 - 127) : detector integration time
    oss << std::format("detIntTime: {}\n", header.detIntTime);
    // : float64(bytes 128 - 135) : detector gain
    oss << std::format("detGain: {}\n", header.detGain);
    // : float64(bytes 136 - 143) : detector X - position
    oss << std::format("detXPos: {}\n", header.detXPos);
    // : float64(bytes 144 - 151) : detector Y - position
    oss << std::format("detYPos: {}\n", header.detYPos);
    // : float64(bytes 152 - 159) : detector Z - position
    oss << std::format("detZPos: {}\n", header.detZPos);
    // : float32(bytes 160 - 163) : the size of the detector pixel(width)
    oss << std::format("detPixWidth: {}\n", header.detPixWidth);
    // : float32(bytes 164 - 167) : the size of the detector pixel(height)
    oss << std::format("detPixHeight: {}\n", header.detPixHeight);
    // : int32(bytes 168 - 171) : bit depth of each pixel in the image
    oss << std::format("detImgBitDepth: {}\n", header.detImgBitDepth);
    // : int32(bytes 172 - 175) : number of pixels the detector contains(width)
    oss << std::format("detSizeWidth: {}\n", header.detSizeWidth);
    // : int32(bytes 176 - 179) : number of pixels the detector contains(height)
    oss << std::format("detSizeHeight: {}\n", header.detSizeHeight);
    // : int32(bytes 180 - 183) : image width
    oss << std::format("detImgSizeWidth: {}\n", header.detImgSizeWidth);
    // : int32(bytes 184 - 187) : image height
    oss << std::format("detImgSizeHeight: {}\n", header.detImgSizeHeight);
    // : uint32(bytes 188 - 191) : number of projections
    oss << std::format("numProj: {}\n", header.numProj);
    // : int32(bytes 192 - 195) : region of interest x, upper left corner
    oss << std::format("roiXulc: {}\n", header.roiXulc);
    // : int32(bytes 196 - 199) : region of interest y, upper left corner
    oss << std::format("roiYulc: {}\n", header.roiYulc);
    // : int32(bytes 200 - 203) : region of interest(width)
    oss << std::format("roiWidth: {}\n", header.roiWidth);
    // : int32(bytes 204 - 207) : region of interest(height)
    oss << std::format("roiHeight: {}\n", header.roiHeight);
    // : float64(bytes 208 - 215) : filter for noise reduction
    oss << std::format("filtNoiseReduct: {}\n", header.filtNoiseReduct);
    // : float64(bytes 216 - 223) : voxel reduction factor
    oss << std::format("voxReductFact: {}\n", header.voxReductFact);
    // : float32(bytes 224 - 227) : gain, according to GUI(1x, 2x, 8x, 16x, ...)
    oss << std::format("imgAvg: {}\n", header.imgAvg);
    // : float32(bytes 228 - 231) : detector pixel binning mode
    oss << std::format("binMode: {}\n", header.binMode);
    //char preFilter[128];// : UTF - 8   (bytes 232 - 359) : pre - filter used, 32 unicode characters
    //oss << std::format(": {}\n", header.);
    // : float64(bytes 360 - 367) : volume start position in x - dimension
    oss << std::format("volStartX: {}\n", header.volStartX);
    // : float64(bytes 368 - 375) : volume start position in y - dimension
    oss << std::format("volStartY: {}\n", header.volStartY);
    // : float64(bytes 376 - 383) : volume start position in z - dimension
    oss << std::format("volStartZ: {}\n", header.volStartZ);
    // : float32(bytes 384 - 387) : minimum pixel value recorded in the data
    oss << std::format("minValueData: {}\n", header.minValueData);
    // : float32(bytes 388 - 391) : maximum pixel value recorded in the data
    oss << std::format("maxValueData: {}\n", header.maxValueData);
    // : float32(bytes 392 - 395) : Angle of first projection of dataset
    oss << std::format("volDefAngle: {}\n", header.volDefAngle);
    // : 8bit(bytes 396) : 8 - bit boolean
    oss << std::format("volumeTogether: {}\n", header.volumeTogether);
    //char padding[627];// : UNKNOWN(627 bytes) : garbage bytes padding out to headSize, which is the size of the header

    std::string output = oss.str();

    return output;
}

void VolumeData::fillBuffer()
{
    const uint64_t width = header.recoX;
    const uint64_t height = header.recoY;
    const uint64_t depth = header.recoZ;

    size_t totalSize = (width * height * depth);

    volumeDataTex.resize(totalSize, 0);

    if (data.empty())
    {
        throw std::runtime_error("VolumeData::fillBuffer: source data is empty.");
    }

    auto [minIt, maxIt] = std::minmax_element(data.begin(), data.end());
    const uint16_t minVal = *minIt;
    const uint16_t maxVal = *maxIt;
    const float range = static_cast<float>(std::max(1, maxVal - minVal));

    for (uint64_t z = 0; z < depth; ++z)
    {
        for (uint64_t y = 0; y < height; ++y)
        {
            for (uint64_t x = 0; x < width; ++x)
            {
                size_t dstIdx = x + width * (y + height * z);

                if (x < width && y < height && z < depth)
                {
                    size_t srcIdx = x + width * (y + height * z); // Fortran-order
                    //size_t srcIdx = z + volumeDepth * (y + volumeHeight * x);
                    float norm = (static_cast<float>(data[srcIdx]) - minVal) / range;
                    volumeDataTex[dstIdx] = static_cast<unsigned char>(std::clamp(norm, 0.0f, 1.0f) * 255.0f);
                }
                else {
                    volumeDataTex[dstIdx] = 0;
                }
            }
        }
    }
}

const std::vector<unsigned char>& VolumeData::getVolumeDataTex() const
{
    return volumeDataTex;
}

