
#include <iostream>
#include <cstring>
#include "decoder.h"
#include "bitStream.h"
#include "adtsHeader.h"
#include "adtsData.h"

#define MIN_STREAMSIZE 768
#define MAX_CHANNELS 6
#define MAX_DATA 8191

#define min(a, b) ( (a) < (b) ? (a) : (b) )

Decoder::Decoder(const char *file) : fileName(file) {}


int Decoder::initDecoder() {
//    std::ifstream in(fileName, std::ios::binary);

    file.open(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "读取文件失败" << std::endl;
        return -1;
    }

    /*file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);*/

    //最大ADTS头是9，没有差错校验就是7
    buffer = new uint8_t[MAX_DATA]();
    file.read(reinterpret_cast<char *>(buffer), MAX_DATA);

    readFileSize = file.gcount();

    if (readFileSize < 7) {
        std::cout << "数据不完整" << std::endl;
        return -1;
    }

    if (memcmp(buffer, "ID3", 3) == 0) {
        std::cout << "不支持解析ID3" << std::endl;
        return -1;
    }
    BitStream bs(buffer, readFileSize);
    uint32_t val = bs.getMultiBit(12);

    /*此处的判断筛选是根据ADTS帧头部的同步字，以此来判断每一帧的开始
     * 判断前遇到0xfff则表明则是新的一帧的开始。*/
    if (val == 0xFFF) {
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
    while (true) {
        fillBuffer();
        if (readFileSize > 7) {
            BitStream bs(buffer, MAX_DATA);
            if (bs.getMultiBit(12) != 0xFFF) {
                break;
            }
            /*读取每一帧的ADTS头*/
            AdtsHeader adtsHeader;
            adtsHeader.adts_fixed_header(bs);
            adtsHeader.adts_variable_header(bs);

            adts_frame(bs, adtsHeader);

            uint16_t frameLength = adtsHeader.frame_length;
            /*如果这一帧的长度等于0或者大于filesize的话就退出，数据不对*/
            if (frameLength == 0 || frameLength > readFileSize) {
                break;
            }
            advanceBuffer(frameLength);
        } else {
            fillByteSize = 0;
            break;
        }
    }

    return 0;
}

int Decoder::adts_frame(BitStream &bs, AdtsHeader &adtsHeader) {
    AdtsData adtsData;
    if (adtsHeader.number_of_raw_data_blocks_in_frame == 0) {
        adtsHeader.adts_error_check(bs);
        adtsData.raw_data_block(bs);
//        raw_data_block();
    } else {
        /* adts_header_error_check();
         for (i = 0; i <= adtsHeader.number_of_raw_data_blocks_in_frame; i++) {
             raw_data_block();
             adts_raw_data_block_error_check();
         }*/
    }
    return 0;
}


int Decoder::fillBuffer() {

    if (fillByteSize > 0) {
        if (readFileSize) {
            //void *memmove(void *dest, const void *src, unsigned int count);
            //由src所指内存区域复制count个字节到dest所指内存区域。
            memmove(buffer, (buffer + fillByteSize), readFileSize);
        }

        if (isEof) {
            file.read(reinterpret_cast<char *>(buffer + readFileSize), fillByteSize);
            uint32_t size = file.gcount();

            /*如果读不满 fillByteSize 大小代表读到尾部了size是实际读取了多大*/
            if (size != fillByteSize) {
                isEof = false;
            }
            readFileSize += size;
        }
        fillByteSize = 0;
    }
    return 0;
}

int Decoder::advanceBuffer(uint16_t frameLength) {
    if (frameLength > 0 && readFileSize > 0) {
        uint32_t size = min(frameLength, readFileSize);
        fillByteSize += size;
        readFileSize -= size;
    }

    return 0;
}

Decoder::~Decoder() {
    if (buffer) {
        delete[] buffer;
        buffer = nullptr;
    }


    file.close();
}



