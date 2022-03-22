//
// Created by Administrator on 2022/3/16.
//

#ifndef AACDECODER_DECODER_H
#define AACDECODER_DECODER_H

#include <cstdint>
#include <fstream>


/*
struct aac_buffer {
    long bytes_into_buffer{0};
    long bytes_consumed{0};
    long file_offset{0};
    unsigned char *buffer{nullptr};
    int at_eof{0};
    FILE *infile = {stderr};
};
*/


class BitStream;

class AdtsHeader;

class Decoder {
    /*要填充多少字节进去*/
    uint32_t fillByteSize{0};

    bool isEof{true};
private:
    std::ifstream file;

    /*uint32_t fileSize{0};*/
    uint32_t readFileSize{0};

    uint8_t *buffer{nullptr};

    const char *fileName{nullptr};

    /*BitStream *bs{nullptr};*/
public:

    explicit Decoder(const char *file);

    ~Decoder();

    int initDecoder();

    int adts_sequence();

    int adts_frame(BitStream &bs, AdtsHeader &adtsHeader);

private:
    int fillBuffer();

    int advanceBuffer(uint16_t frameLength);
};

#endif //AACDECODER_DECODER_H
