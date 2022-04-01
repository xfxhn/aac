//
// Created by Administrator on 2022/4/1.
//

#ifndef AACDECODER_HUFFMAN_H
#define AACDECODER_HUFFMAN_H

class BitStream;

class Huffman {
public:
    int8_t scale_factor(BitStream &bs);
};

#endif //AACDECODER_HUFFMAN_H
