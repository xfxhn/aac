
#include <iostream>
#include <fstream>
#include <cstring>
#include "decoder.h"
#include "bitStream.h"
#include "adtsHeader.h"

#define MIN_STREAMSIZE 768
#define MAX_CHANNELS 6


Decoder::Decoder(const char *file) : fileName(file), fileSize(0), buffer(nullptr) {}


int Decoder::initDecoder() {
    std::ifstream in(fileName, std::ios::binary);
    if (!in) {
        std::cout << "读取文件失败" << std::endl;
        return -1;
    }
    in.seekg(0, std::ios::end);
    fileSize = in.tellg();
    in.seekg(0, std::ios::beg);

    buffer = new uint8_t[MIN_STREAMSIZE * MAX_CHANNELS]();
    in.read(reinterpret_cast<char *>(buffer), MIN_STREAMSIZE * MAX_CHANNELS);

    uint32_t size = in.gcount();

    if (memcmp(buffer, "ID3", 3) == 0) {
        std::cout << "不支持解析ID3" << std::endl;
        return -1;
    }

//    BitStream bs(buffer, size);
    bs = new BitStream(buffer, size);
    uint32_t val = bs->getMultiBit(12);

    /*此处的判断筛选是根据ADTS帧头部的同步字，以此来判断每一帧的开始
     * 判断前遇到0xfff则表明则是新的一帧的开始。*/
    if (val == 0xFFF) {
        std::cout << "ADTS" << std::endl;
        adts_sequence();
    } else if (memcmp(buffer, "ADIF", 4) == 0) {
        std::cout << "不支持解析ADIF格式" << std::endl;
        return -1;
    } else {
        std::cout << "格式错误" << std::endl;
        return -1;
    }
    return 0;
}

int Decoder::adts_sequence() {


    /*while (nextbits() == syncword) {
         adts_frame();
      }*/
    return adts_frame();
}

int Decoder::adts_frame() {

    AdtsHeader adtsHeader;
    adtsHeader.adts_fixed_header(bs);
    adtsHeader.adts_variable_header(bs);


    /*adts_fixed_header();*/


    return 0;
}


Decoder::~Decoder() {
    if (bs) {
        delete bs;
        bs = nullptr;
    }
}




