#include <cstdint>
#include "adtsData.h"
#include "bitStream.h"
#include "ICS.h"
#include "adtsHeader.h"

#define ANC_DATA 0
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

/*Table 41 */
enum extension_type {
    /*比特流填充*/
    EXT_FILL = 0,
    /*比特流数据作为填充*/
    EXT_FILL_DATA = 1,

    /*data element*/
    EXT_DATA_ELEMENT = 2,

    /*动态范围控制*/
    EXT_DYNAMIC_RANGE = 11,
    /*MPEG环绕*/
    EXT_SAC_DATA = 12,
    /*SBR增强*/
    EXT_SBR_DATA = 13,
    /*用CRC增强SBR*/
    EXT_SBR_DATA_CRC = 14


};

int AdtsData::raw_data_block(BitStream &bs, AdtsHeader &adtsHeader) {
    uint8_t id = 0;
    while ((id = bs.readMultiBit(3)) != ID_END) {
        switch (id) {
            case ID_SCE:
//                single_channel_element();
                break;
            case ID_CPE:
                channel_pair_element(bs, adtsHeader);
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
                fill_element(bs);
                break;
            default:
                break;
        }
    }
    return 0;
}

/*缩写CPE。 包含一对通道的数据的位流的语法元素。 一个
Channel_pair_element()包含两个元素
和附加的联合信道编码信息。 这两个通道可以共享共同的侧信息。
 channel_pair_element()在element_instance_tag和出现次数方面具有与单个channel元素相同的限制(表14)。*/
int AdtsData::channel_pair_element(BitStream &bs, AdtsHeader &adtsHeader) {

    ICS ics;
    element_instance_tag = bs.readMultiBit(4);
    /*指示两个 individual_channel_stream() 是否共 享一个公共 ics_info()。
     * 在共享的情况下，ics_info() 是 channel_pair_element() 的一部分，并且必须用于两个通道。
     * 否则，ics_info() 是每个 individual_channel_stream() 的一部分*/
    bool common_window = bs.readBit();
    if (common_window) {
        ics.ics_info(bs, adtsHeader);
        bool ms_mask_present = bs.readMultiBit(2);
        if (ms_mask_present == 1) {
            for (int g = 0; g < ics.num_window_groups; ++g) {
                for (int sfb = 0; sfb < ics.max_sfb; ++sfb) {
                    ics.ms_used[g][sfb] = bs.readBit();
                }
            }
        }
    }
    return 0;
}

int AdtsData::fill_element(BitStream &bs) {
    /*这里按标准看是否解析SBR功能和PS功能，现在我暂时没找到标准文档对这块功能的描述*/


/*语法单元count给出填充数据的初始长度。与数据单元一样，如果count值等于15,就加上esc_count的值，结果就是要读的fill_byte的个数*/
    /*count 填充数据长度初始值*/
    uint16_t cnt = bs.readMultiBit(4);
    if (cnt == 15) {
        /*esc_count(8bit) 填充数据长度的增量值 */
        cnt += bs.readMultiBit(8) - 1;
    }
    while (cnt > 0) {
        cnt -= extension_payload(bs, cnt);
    }
    return 0;
}

uint16_t AdtsData::extension_payload(BitStream &bs, uint16_t cnt) {
    /*fill_nibble 通常被定义为“0000”，而fill_byte 通常被定义为“10100101”（以确保自时 钟数据流，例如无线电调制解调器，可以执行可靠的时钟恢复*/
    /*4比特，表明填充单元内容的类型*/
    uint8_t extension_type = bs.readMultiBit(4);
    uint8_t align = 4;
    switch (extension_type) {
        case EXT_DYNAMIC_RANGE:
            return dynamic_range_info(bs);
        case EXT_SAC_DATA:
            /*MPEG环绕*/
            /* sac_extension_data(cnt)*/
            return -1;
        case EXT_SBR_DATA:
            /*SBR增强*/
            /* sbr_extension_data(id_aac, 0)*/
            return -1;
        case EXT_SBR_DATA_CRC:
            /*用CRC增强SBR*/
            /*return sbr_extension_data(id_aac, 1)*/
            return -1;
        case EXT_FILL_DATA: {
            /* must be ‘0000’ */
            uint8_t fill_nibble = bs.readMultiBit(4);
            for (int i = 0; i < cnt - 1; ++i) {
                /*fill_byte[i] must be ‘10100101’ */
                uint8_t fill_byte = bs.readMultiBit(8);
            }
            return cnt;
        }
        case EXT_DATA_ELEMENT: {
            /*表示元素的版本*/
            uint8_t data_element_version = bs.readMultiBit(4);
            switch (data_element_version) {
                case ANC_DATA: {
                    uint8_t loopCounter = 0;
                    uint8_t dataElementLength = 0;
                    /*表示扩展有效载荷 "数据元素 "的长度
                     * 值255被用作转义值，表示后面至少还有一个dataElementLengthPart值
                     * 传输的 "数据元素 "的总长度是通过部分数值相加来计算的
                     * */
                    uint8_t dataElementLengthPart = 0;
                    do {
                        dataElementLengthPart = bs.readMultiBit(8);
                        dataElementLength += dataElementLengthPart;
                        loopCounter++;
                    } while (dataElementLengthPart == 255);
                    for (int i = 0; i < dataElementLength; ++i) {
                        /*a variable indicating the partial values of the extension payload "data element" with type ‘ANC_DATA‘ in bytes */
                        uint8_t data_element_byte = bs.readMultiBit(8);
                    }
                    return (dataElementLength + loopCounter + 1);
                }
                default:
                    align = 0;
            }
        }
        case EXT_FILL:
        default:
            for (int j = 0; j < 8 * (cnt - 1) + align; ++j) {
                /*other_bits[i] 被解码器丢弃的位。*/
                uint8_t other_bits = bs.readBit();
            }
            return cnt;
    }
}

/*
 * 当输出的音频信号不是很大的时候，系统会按照原来的设定输出，
 * 但是当输出的音频信号过大的时候，为了保护喇叭DRC会将输出信号的幅度进行压缩将其限制在一个范围内。
 * 因为输出的音频信号过大会引起削峰，从而引起音频失真，并且损坏喇叭，所以需要有DRC的作用来将输出限制在一定的范围内。
 * 在信号很小的时候DRC是不会起作用的，只有当输出信号的功率超过了你设定的DRC门限的时候DRC才会工作。
 * */
/*Dynamic Range Control(DRC)动态范围控制提供压缩和放大能力，可以使声音听起来更柔和或者更大声，即一种信号幅度调节方式。*/
uint8_t AdtsData::dynamic_range_info(BitStream &bs) {
    uint8_t n = 1;
    uint8_t drc_num_bands = 1;

    pce_tag_present = bs.readBit();
    if (pce_tag_present == 1) {
        /*表示动态范围信息与哪个程序相关联的标记字段  */
        pce_instance_tag = bs.readMultiBit(4);
        /*保留*/
        drc_tag_reserved_bits = bs.readMultiBit(4);
        n++;
    }

    /*表明隔离声道存在*/
    excluded_chns_present = bs.readBit();
    if (excluded_chns_present == 1) {
        n += excluded_channels(bs);
    }

    /*表明DRC多频带信息存在*/
    drc_bands_present = bs.readBit();
    if (drc_bands_present == 1) {
        /*包含DRC信息的大于1的DRC频带数  */
        drc_band_incr = bs.readMultiBit(4);
        /*保留*/
        drc_bands_reserved_bits = bs.readMultiBit(4);
        n++;
        drc_num_bands = drc_num_bands + drc_band_incr;
        /*drc_num_bands 最大只有17，因为drc_band_incr 4bit最大=15+1=16*/
        for (int i = 0; i < drc_num_bands; ++i) {
            /*表明第i个DRC频带的最髙频谱，以4根谱线为单位。*/
            /*如果drc_band_top[i] = k，则第i个DRC波段的最高谱系数(w.r.t 0)的指数(w.r.t 0) = k*4-1+3。
             * 如果发生  window_sequence索引被解释为指向8*128(unde-interleaved)频率点对应的8个短变换的连接数组。  */
            drc_band_top[i] = bs.readMultiBit(8);
            n++;
        }
    }

    /*表明参考电平存在*/
    prog_ref_level_present = bs.readBit();
    if (prog_ref_level_present == 1) {
        /*对所有频道的长期节目音频水平的衡量标准  */
        prog_ref_level = bs.readMultiBit(7);
        /*保留*/
        prog_ref_level_reserved_bits = bs.readBit();
        n++;
    }

    for (int i = 0; i < drc_num_bands; ++i) {

        /*一位表示dyn_rng_ctl的符号(0为正，1为负)  */
        dyn_rng_sgn[i] = bs.readMultiBit(1);

        /*动态范围控制幅度信息*/
        /*应用于当前帧的解码音频输出样本的增益值*/
        /*dyn_rng_ctl使用7位无符号整数在0.25 dB步长中量化，因此，与dyn_rng_sgn关联，其范围为+/-31.75 dB。*/
        dyn_rng_ctl[i] = bs.readMultiBit(7);
        n++;
    }
    return n;
}

uint8_t AdtsData::excluded_channels(BitStream &bs) {
    uint8_t n = 0;
    uint8_t num_excl_chan = 7;
    for (int i = 0; i < 7; ++i) {
        /*布尔数组，指示SCE、CPE或LFE不在动态范围控制中。 在传输新的掩码之前，掩码信息一直有效。 */
        /*布尔数组，表明一个排除在DRC处理过程之外的音频声道节目 使用此DRC信息*/
        exclude_mask[i] = bs.readBit();
    }
    n++;

    /*表明存在另外的隔离声道*/
    while ((additional_excluded_chns[n - 1] = bs.readBit()) == 1) {
        for (int i = 0; i < num_excl_chan + 7; ++i) {
            exclude_mask[i] = bs.readBit();
        }
        n++;
        num_excl_chan += 7;
    }
    return n;
}



