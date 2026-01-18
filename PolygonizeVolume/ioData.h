#pragma once

#include <string>
#include <vector>

#pragma pack(push, 1)
struct uint16_csv_volume
{
    unsigned int headSize;//: uint32(bytes 0 - 3) : size of the header
    unsigned int mirrorZ;// : uint32(bytes 4 - 7) : UNKNOWN what this is or represents
    unsigned int numBitsVox;// : uint32(bytes 8 - 11) : number of bits per voxel
    unsigned int recoX;// : uint32(bytes 12 - 15) : size of volume in x - dimension
    unsigned int recoY;// : uint32(bytes 16 - 19) : size of volume in y - dimension
    unsigned int recoZ;// : uint32(bytes 20 - 23) : size of volume in z - dimension
    double voxSizeX;//: float64(bytes 24 - 31) : size of voxel in x
    double voxSizeY;// : float64(bytes 32 - 39) : size of voxel in y
    double voxSizeZ;// : float64(bytes 40 - 47) : size of voxel in z
    double minValue;// : float64(bytes 48 - 55) : minimum gray - scale value as determined by CERA
    double maxValue;// : float64(bytes 56 - 63) : maximum gray - scale value as determined by CERA
    double tubeX;// : float64(bytes 64 - 71) : beam center in x - dimension
    double tubeY;// : float64(bytes 72 - 79) : beam center in y - dimension
    double tubeZ;// : float64(bytes 80 - 87) : beam center in z - dimension
    int tubeCurrent;// : int32(bytes 88 - 91) : x - ray tube current
    int tubeVoltage;//: int32(bytes 92 - 95) : x - ray tube voltage
    double rtDataX;// : float64(bytes 96 - 103) : rotary table X - position
    double rtDataY;// : float64(bytes 104 - 111) : rotary table Y - position
    double rtDataZ;// : float64(bytes 112 - 119) : rotary table Z - position
    double detIntTime;// : float64(bytes 120 - 127) : detector integration time
    double detGain;// : float64(bytes 128 - 135) : detector gain
    double detXPos;// : float64(bytes 136 - 143) : detector X - position
    double detYPos;// : float64(bytes 144 - 151) : detector Y - position
    double detZPos;// : float64(bytes 152 - 159) : detector Z - position
    float detPixWidth;// : float32(bytes 160 - 163) : the size of the detector pixel(width)
    float detPixHeight;// : float32(bytes 164 - 167) : the size of the detector pixel(height)
    int detImgBitDepth;// : int32(bytes 168 - 171) : bit depth of each pixel in the image
    int detSizeWidth;// : int32(bytes 172 - 175) : number of pixels the detector contains(width)
    int detSizeHeight;// : int32(bytes 176 - 179) : number of pixels the detector contains(height)
    int detImgSizeWidth;// : int32(bytes 180 - 183) : image width
    int detImgSizeHeight;// : int32(bytes 184 - 187) : image height
    unsigned int numProj;// : uint32(bytes 188 - 191) : number of projections
    int roiXulc;// : int32(bytes 192 - 195) : region of interest x, upper left corner
    int roiYulc;// : int32(bytes 196 - 199) : region of interest y, upper left corner
    int roiWidth;// : int32(bytes 200 - 203) : region of interest(width)
    int roiHeight;// : int32(bytes 204 - 207) : region of interest(height)
    double filtNoiseReduct;// : float64(bytes 208 - 215) : filter for noise reduction
    double voxReductFact;// : float64(bytes 216 - 223) : voxel reduction factor
    float imgAvg;// : float32(bytes 224 - 227) : gain, according to GUI(1x, 2x, 8x, 16x, ...)
    float binMode;// : float32(bytes 228 - 231) : detector pixel binning mode
    char preFilter[128];// : UTF - 8   (bytes 232 - 359) : pre - filter used, 32 unicode characters
    double volStartX;// : float64(bytes 360 - 367) : volume start position in x - dimension
    double volStartY;// : float64(bytes 368 - 375) : volume start position in y - dimension
    double volStartZ;// : float64(bytes 376 - 383) : volume start position in z - dimension
    float minValueData;// : float32(bytes 384 - 387) : minimum pixel value recorded in the data
    float maxValueData;// : float32(bytes 388 - 391) : maximum pixel value recorded in the data
    float volDefAngle;// : float32(bytes 392 - 395) : Angle of first projection of dataset
    bool volumeTogether;// : 8bit(bytes 396) : 8 - bit boolean
    char padding[627];// : UNKNOWN(627 bytes) : garbage bytes padding out to headSize, which is the size of the header
};
#pragma pack(pop)

//for now support only for uint16 
class VolumeData
{
private:
    uint16_csv_volume   header;
    std::vector<unsigned short> data;
    std::vector<unsigned char> volumeDataTex;


public:
    VolumeData(const std::string filePath);
    const uint16_csv_volume* getHeader() const;
    std::string getHeaderString() const;
    void saveHeaderToFile(std::string fileName);
    void fillBuffer();
    const std::vector<unsigned char>& getVolumeDataTex() const;
};

