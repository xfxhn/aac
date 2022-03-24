#include <cstdint>
#include "ICS.h"
#include "bitStream.h"
#include "adtsHeader.h"

#define min(a, b) ( (a) < (b) ? (a) : (b) )
enum window_sequence {
    ONLY_LONG_SEQUENCE = 0,
    LONG_START_SEQUENCE = 1,
    EIGHT_SHORT_SEQUENCE = 2,
    LONG_STOP_SEQUENCE = 3
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
        }
    }

    return 0;
}
