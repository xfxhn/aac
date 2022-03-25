#include <cstdint>
#include "ICS.h"
#include "bitStream.h"
#include "adtsHeader.h"

#define min(a, b) ( (a) < (b) ? (a) : (b) )
/*mdct的一个变换窗口的分析窗口长度N是语法元素window_sequence的函数，定义如下  */
enum window_sequence {
    ONLY_LONG_SEQUENCE = 0,
    LONG_START_SEQUENCE = 1,
    EIGHT_SHORT_SEQUENCE = 2,
    LONG_STOP_SEQUENCE = 3
};

/*
 index    sample_rate     num_swb_long_window
   0      88.2 and 96             41
   1      88.2 and 96             41
   2      64 kHz                  47
   3      44.1 and 48             49
   4      44.1 and 48             49
   5      32 kHz                  51
   6      22.05 and 24            47
   7      22.05 and 24            47
   8      12 and 16 and 11.025    43
   9      12 and 16 and 11.025    43
   10     12 and 16 and 11.025    43
   11     8 kHz                   40
 */

/*num_swb：在EIGHT_SHORT_SEQUENCE的情况下，用于短窗口的缩放因子窗口频带数，否则用于长窗口的缩放因子窗口频带数。*/
/*存储每个对应采样率索引对应的num_swb*/
/*长窗口*/
static constexpr uint8_t num_swb_long_window[] = {
        41, 41, 47, 49, 49, 51, 47, 47, 43, 43, 43, 40
};

static const uint16_t swb_offset_88_96[41] = {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56,
        64, 72, 80, 88, 96, 108, 120, 132, 144, 156, 172, 188, 212, 240,
        276, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960
};
uint8_t aa[1][2] = {
        {12, 3}
};
/*指针数组*/
static const uint16_t *swb_offset_long_window[12] = {
        swb_offset_88_96,/* 96000 */
        swb_offset_88_96,/* 88200 */
};

int ICS::ics_info(BitStream &bs, AdtsHeader &adtsHeader) {

    ics_reserved_bit = bs.readBit();
    window_sequence = bs.readMultiBit(2);
    window_shape = bs.readBit();

    window_grouping_info(adtsHeader);

    if (window_sequence == EIGHT_SHORT_SEQUENCE) {
        max_sfb = bs.readMultiBit(4);
        scale_factor_grouping = bs.readMultiBit(7);
    } else {
        max_sfb = bs.readMultiBit(6);
        /*是否在当前帧中使用预测*/
        bool predictor_data_present = bs.readBit();
        if (predictor_data_present) {
            /*是否在当前帧中应用了预测器重置*/
            bool predictor_reset = bs.readBit();
            if (predictor_reset) {
                /*如果启用了预测器重置，则5bit指定要在当前帧中重置的重置组*/
                predictor_reset_group_number = bs.readMultiBit(5);
            }
            uint8_t PRED_SFB_MAX = adtsHeader.get_Pred_SFB_MAX();
            for (int sfb = 0; sfb < min(max_sfb, PRED_SFB_MAX); ++sfb) {
                prediction_used[sfb] = bs.readBit();
            }
        }
    }
    return 0;
}

int ICS::window_grouping_info(AdtsHeader &adtsHeader) {
    uint8_t fs_index = adtsHeader.sample_rate_index;
    switch (window_sequence) {
        case ONLY_LONG_SEQUENCE:
        case LONG_START_SEQUENCE:
        case LONG_STOP_SEQUENCE: {
            num_windows = 1;
            num_window_groups = 1;
            window_group_length[num_window_groups - 1] = 1;
            num_swb = num_swb_long_window[fs_index];
            /* preparation of sect_sfb_offset for long blocks */
            /* also copy the last value! */
            for (i = 0; i < max_sfb + 1; i++) {
                sect_sfb_offset[0][i] = swb_offset_long_window[fs_index][i];
                swb_offset[i] = swb_offset_long_window[fs_index][i];
            }
            break;
        }
    }

    return 0;
}
