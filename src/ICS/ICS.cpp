#include <cstdint>
#include "ICS.h"
#include "bitStream.h"
#include "adtsHeader.h"

#define min(a, b) ( (a) < (b) ? (a) : (b) )
/*返回A的第B位的值*/
#define bit_set(A, B) ((A) & (1<<(B)))
/*标度因子带的最大数量由一个常数限制：MAX_LTP_LONG_SFB=40(用于长帧) */
#define MAX_LTP_LONG_SFB 40




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
/*短窗口*/
static constexpr uint8_t num_swb_short_window[] = {
        12, 12, 12, 14, 14, 14, 15, 15, 15, 15, 15, 15
};

/*长窗口*/
static constexpr uint8_t num_swb_long_window[] = {
        41, 41, 47, 49, 49, 51, 47, 47, 43, 43, 43, 40
};

static constexpr uint16_t swb_offset_long_88_96[42] = {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56,
        64, 72, 80, 88, 96, 108, 120, 132, 144, 156, 172, 188, 212, 240,
        276, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1024
};
static constexpr uint16_t swb_offset_long_64[48] = {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56,
        64, 72, 80, 88, 100, 112, 124, 140, 156, 172, 192, 216, 240, 268,
        304, 344, 384, 424, 464, 504, 544, 584, 624, 664, 704, 744, 784, 824,
        864, 904, 944, 984, 1024
};
static constexpr uint16_t swb_offset_long_41_48[50] = {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 48, 56, 64, 72,
        80, 88, 96, 108, 120, 132, 144, 160, 176, 196, 216, 240, 264, 292,
        320, 352, 384, 416, 448, 480, 512, 544, 576, 608, 640, 672, 704, 736,
        768, 800, 832, 864, 896, 928, 1024
};

static constexpr uint16_t swb_offset_long_32[52] = {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 48, 56, 64, 72,
        80, 88, 96, 108, 120, 132, 144, 160, 176, 196, 216, 240, 264, 292,
        320, 352, 384, 416, 448, 480, 512, 544, 576, 608, 640, 672, 704, 736,
        768, 800, 832, 864, 896, 928, 960, 992, 1024
};
static constexpr uint16_t swb_offset_long_22_24[48] = {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 52, 60, 68,
        76, 84, 92, 100, 108, 116, 124, 136, 148, 160, 172, 188, 204, 220,
        240, 260, 284, 308, 336, 364, 396, 432, 468, 508, 552, 600, 652, 704,
        768, 832, 896, 960, 1024
};
static constexpr uint16_t swb_offset_long_12_16_11[44] = {
        0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 100, 112, 124,
        136, 148, 160, 172, 184, 196, 212, 228, 244, 260, 280, 300, 320, 344,
        368, 396, 424, 456, 492, 532, 572, 616, 664, 716, 772, 832, 896, 960, 1024
};
static constexpr uint16_t swb_offset_long_8[41] = {
        0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 172,
        188, 204, 220, 236, 252, 268, 288, 308, 328, 348, 372, 396, 420, 448,
        476, 508, 544, 580, 620, 664, 712, 764, 820, 880, 944, 1024
};
/*指针数组*/
static const uint16_t *swb_offset_long_window[12] = {
        swb_offset_long_88_96,/* 96000 */
        swb_offset_long_88_96,/* 88200 */
        swb_offset_long_64,   /*64 kHz */
        swb_offset_long_41_48,/* 48000 */
        swb_offset_long_41_48,/* 44100 */
        swb_offset_long_32,   /*32 kHz */
        swb_offset_long_22_24,/*24000*/
        swb_offset_long_22_24,/*22000*/
        swb_offset_long_12_16_11,/*16000*/
        swb_offset_long_12_16_11,/*12000*/
        swb_offset_long_12_16_11,/*11025*/
        swb_offset_long_8/*8000*/
};
static constexpr uint16_t swb_offset_short_88_96[13] = {
        0, 4, 8, 12, 16, 20, 24, 32, 40, 48, 64, 92, 128
};
static constexpr uint16_t swb_offset_short_64[13] = {
        0, 4, 8, 12, 16, 20, 24, 32, 40, 48, 64, 92, 128
};
static constexpr uint16_t swb_offset_short_32_41_48[15] = {
        0, 4, 8, 12, 16, 20, 28, 36, 44, 56, 68, 80, 96, 112, 128
};
static constexpr uint16_t swb_offset_short_22_24[16] = {
        0, 4, 8, 12, 16, 20, 24, 28, 36, 44, 52, 64, 76, 92, 108, 128
};
static constexpr uint16_t swb_offset_short_11_12_16[16] = {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 40, 48, 60, 72, 88, 108, 128
};
static constexpr uint16_t swb_offset_short_8[16] = {
        0, 4, 8, 12, 16, 20, 24, 28, 36, 44, 52, 60, 72, 88, 108, 128
};
static const uint16_t *swb_offset_short_window[] = {
        swb_offset_short_88_96,       /* 96000 */
        swb_offset_short_88_96,       /* 88200 */
        swb_offset_short_64,       /* 64000 */
        swb_offset_short_32_41_48,       /* 48000 */
        swb_offset_short_32_41_48,       /* 44100 */
        swb_offset_short_32_41_48,       /* 32000 */
        swb_offset_short_22_24,       /* 24000 */
        swb_offset_short_22_24,       /* 22050 */
        swb_offset_short_11_12_16,       /* 16000 */
        swb_offset_short_11_12_16,       /* 12000 */
        swb_offset_short_11_12_16,       /* 11025 */
        swb_offset_short_8         /* 8000  */
};

int ICS::ics_info(BitStream &bs, AdtsHeader &adtsHeader, bool common_window) {

    ics_reserved_bit = bs.readBit();
    window_sequence = bs.readMultiBit(2);
    window_shape = bs.readBit();


    if (window_sequence == EIGHT_SHORT_SEQUENCE) {
        max_sfb = bs.readMultiBit(4);
        scale_factor_grouping = bs.readMultiBit(7);
    } else {
        max_sfb = bs.readMultiBit(6);
        /*是否在当前帧中使用预测*/
        bool predictor_data_present = bs.readBit();
        if (predictor_data_present) {
            if (adtsHeader.objectType == AudioObjectType::Main) {
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
            } else {
                bool ltp_data_present = bs.readBit();
                if (ltp_data_present) {
                    ltp_data(bs, adtsHeader);
                }
                if (common_window) {
                    ltp_data_present = bs.readBit();
                    if (ltp_data_present) {
                        ltp_data(bs, adtsHeader);
                    }
                }
            }

        }
    }
    window_grouping_info(adtsHeader);
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
            for (int i = 0; i < max_sfb + 1; i++) {
                sect_sfb_offset[0][i] = swb_offset_long_window[fs_index][i];
                swb_offset[i] = swb_offset_long_window[fs_index][i];
            }
            break;
        }
        case EIGHT_SHORT_SEQUENCE: {
            num_windows = 8;
            num_window_groups = 1;
            window_group_length[num_window_groups - 1] = 1;
            num_swb = num_swb_short_window[fs_index];
            for (int i = 0; i < num_swb_short_window[fs_index] + 1; i++) {
                swb_offset[i] = swb_offset_short_window[fs_index][i];
            }

            for (int i = 0; i < num_windows - 1; i++) {
                if ((bit_set(scale_factor_grouping, 6 - i)) == 0) {
                    num_window_groups += 1;
                    window_group_length[num_window_groups - 1] = 1;
                } else {
                    window_group_length[num_window_groups - 1] += 1;
                }
            }

            /* preparation of sect_sfb_offset for short blocks */
            for (int g = 0; g < num_window_groups; g++) {
                uint16_t width;
                uint8_t sect_sfb = 0;
                uint16_t offset = 0;
                for (int i = 0; i < max_sfb; i++) {
                    width = swb_offset_short_window[fs_index][i + 1] - swb_offset_short_window[fs_index][i];
                    width *= window_group_length[g];
                    sect_sfb_offset[g][sect_sfb++] = offset;
                    offset += width;
                }
                sect_sfb_offset[g][sect_sfb] = offset;
            }
            break;
        }
        default:
            return -1;
    }

    return 0;
}

int ICS::ltp_data(BitStream &bs, AdtsHeader &adtsHeader) {
    uint8_t long_used[51]{0};
    if (adtsHeader.objectType == AudioObjectType::LD) {
        bool ltp_lag_update = bs.readBit();
        if (ltp_lag_update) {
            uint8_t ltp_lag = bs.readMultiBit(10);
        }

        uint8_t ltp_coef = bs.readMultiBit(3);
        for (int sfb = 0; sfb < min(max_sfb, MAX_LTP_LONG_SFB); ++sfb) {
            long_used[sfb] = bs.readBit();
        }
    } else {
        uint16_t ltp_lag = bs.readMultiBit(11);
        uint8_t ltp_coef = bs.readMultiBit(3);
        if (window_sequence == EIGHT_SHORT_SEQUENCE) {
            for (int sfb = 0; sfb < min(max_sfb, MAX_LTP_LONG_SFB); ++sfb) {
                long_used[sfb] = bs.readBit();
            }
        }
    }
    return 0;
}
