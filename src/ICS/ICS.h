//
// Created by Administrator on 2022/3/23.
//

#ifndef AACDECODER_ICS_H
#define AACDECODER_ICS_H


/*mdct的一个变换窗口的分析窗口长度N是语法元素window_sequence的函数，定义如下  */
enum window_sequence {
    ONLY_LONG_SEQUENCE = 0,
    LONG_START_SEQUENCE = 1,
    EIGHT_SHORT_SEQUENCE = 2,
    LONG_STOP_SEQUENCE = 3
};

class BitStream;

class AdtsHeader;

/*独立通道流*/
class ICS {
public:
    /*保留位*/
    uint8_t ics_reserved_bit{0};
    /*表示表44中定义的窗口序列 */
    uint8_t window_sequence{0};
    /*决定分析窗后部的窗口形状*/
    uint8_t window_shape{0};
    /*每组传送的比例因子频段的个数*/
    uint8_t max_sfb{0};
    /*包含关于短光谱数据分组的信息的位域  */
    uint8_t scale_factor_grouping{0};
    uint8_t predictor_reset_group_number{0};
    uint8_t prediction_used[41]{0};

    /*共享一组比例因子的窗口组数*/
    uint8_t num_window_groups{0};
    /*实际窗口序列的窗口数*/
    uint8_t num_windows{0};
    /*每组中Windows的个数*/
    uint8_t window_group_length[8]{0};
    /*在EIGHT_SHORT_SEQUENCE的情况下，用于短窗口的缩放因子窗口频带数，否则用于长窗口的缩放因子窗口频带数。*/
    uint8_t num_swb{0};
    uint8_t sect_sfb_offset[8][52]{0};
    uint8_t swb_offset[52]{0};


    uint8_t ms_used[8][63]{0};

    /*用于g组第i节的Spectrum Huffman码本(见第6.3款，表17)。 */
    uint8_t sect_cb[8][120]{0};
    uint8_t sfb_cb[8][120]{0};
    uint8_t sect_start[8][120]{0};
    uint8_t sect_end[8][120]{0};
public:
    int ics_info(BitStream &bs, AdtsHeader &adtsHeader, bool common_window);

private:
    int window_grouping_info(AdtsHeader &adtsHeader);

    int ltp_data(BitStream &bs, AdtsHeader &adtsHeader);
};

#endif //AACDECODER_ICS_H
