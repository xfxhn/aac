//
// Created by Administrator on 2022/3/21.
//

#ifndef AACDECODER_ADTSDATA_H
#define AACDECODER_ADTSDATA_H

class BitStream;

class AdtsData {
public:
    int raw_data_block(BitStream &bs);

private:
    int fill_element(BitStream &bs);

    int extension_payload(BitStream &bs, uint16_t cnt);
};

#endif //AACDECODER_ADTSDATA_H
