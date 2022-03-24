//
// Created by Administrator on 2022/3/23.
//

#ifndef AACDECODER_ICS_H
#define AACDECODER_ICS_H

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
    uint8_t window_group_length[8]{0};

public:
    int ics_info(BitStream &bs, AdtsHeader &adtsHeader);

private:
    int window_grouping_info(AdtsHeader &adtsHeader);
};

#endif //AACDECODER_ICS_H
