#include <cstdint>
#include "adtsData.h"
#include "bitStream.h"

enum ID_SYN_ELE {
    /*Single Channel Element单通道元素。单通道元素基本上只由一个ICS组成。一个原始数据块最可能由16个SCE组成。*/
    ID_SCE = 0x0,
    /*Channel Pair Element 双通道元素，由两个可能共享边信息的ICS和一些联合立体声编码信息组成。一个原始数据块最多可能由16个SCE组成。*/
    ID_CPE = 0x1,
    /*Coupling Channel Element 藕合通道元素。代表一个块的多通道联合立体声信息或者多语种程序的对话信息。*/
    ID_CCE = 0x2,
    /* Low Frequency Element 低频元素。包含了一个加强低采样频率的通道。*/
    ID_LFE = 0x3,
    /*Data Stream Element 数据流元素，包含了一些并不属于音频的附加信息。*/
    ID_DSE = 0x4,
    /*Program Config Element 程序配置元素。包含了声道的配置信息。它可能出现在 ADIF 头部信息中。*/
    ID_PCE = 0x5,
    /*Fill Element 填充元素。包含了一些扩展信息。如SBR，动态范围控制信息等*/
    ID_FIL = 0x6,
    ID_END = 0x7
};

int AdtsData::raw_data_block(BitStream &bs) {
    uint8_t id = 0;
    while ((id = bs.readMultiBit(3)) != ID_END) {
        switch (id) {
            case ID_SCE:
//                single_channel_element();
                break;
            case ID_CPE:
//                channel_pair_element();
                break;
            case ID_CCE:
//                coupling_channel_element();
                break;
            case ID_LFE:
//                lfe_channel_element();
                break;
            case ID_DSE:
//                data_stream_element();
                break;
            case ID_PCE:
//                program_config_element();
                break;
            case ID_FIL:
                fill_element();
                break;
            default:
                break;
        }
    }
    return 0;
}

int AdtsData::fill_element(BitStream &bs) {
    uint16_t cnt = bs.readMultiBit(4);
    if (cnt == 15) {
        cnt += bs.readMultiBit(8) - 1;
    }
    while (cnt > 0) {
        cnt -= extension_payload(bs, cnt);
    }
    return 0;
}

int AdtsData::extension_payload(BitStream &bs, uint16_t cnt) {
    return 0;
}

