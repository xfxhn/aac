//
// Created by Administrator on 2022/3/16.
//

#ifndef AACDECODER_DECODER_H
#define AACDECODER_DECODER_H

#include <cstdint>


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

class Decoder {
private:
    uint32_t fileSize{0};
    uint8_t *buffer{nullptr};

    const char *fileName{nullptr};

    BitStream *bs{nullptr};
public:

    explicit Decoder(const char *file);

    ~Decoder();

    int initDecoder();

    int adts_sequence();

    int adts_frame();
};

#endif //AACDECODER_DECODER_H
