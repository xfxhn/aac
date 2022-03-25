
#include <cstdint>
#include "adtsHeader.h"
#include "bitStream.h"

int AdtsHeader::adts_sample_rates[] = {
        96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000, 7350, 0, 0, 0
};

int AdtsHeader::adts_fixed_header(BitStream &bs) {
    syncword = bs.readMultiBit(12);
    ID = bs.readBit();
    layer = bs.readMultiBit(2);
    protection_absent = bs.readMultiBit(1);
    profile = bs.readMultiBit(2);
    sampling_frequency_index = bs.readMultiBit(4);
    sample_rate = AdtsHeader::adts_sample_rates[sampling_frequency_index];
    private_bit = bs.readBit();
    channel_configuration = bs.readMultiBit(3);
    original_copy = bs.readBit();
    home = bs.readBit();

    set_sample_rate_index();
    return 0;
}

int AdtsHeader::adts_variable_header(BitStream &bs) {

    copyright_identification_bit = bs.readBit();
    copyright_identification_start = bs.readBit();
    frame_length = bs.readMultiBit(13);
    adts_buffer_fullness = bs.readMultiBit(11);
    number_of_raw_data_blocks_in_frame = bs.readMultiBit(2);
    return 0;
}

uint16_t AdtsHeader::getFrameLength() const {
    return frame_length;
}

int AdtsHeader::adts_header_error_check() {

    /*if (protection_absent == 0) {
        for (int i = 1; i <= number_of_raw_data_blocks_in_frame; i++) {
            raw_data_block_position[i];
        }
        crc_check;
    }*/
    return 0;
}

int AdtsHeader::adts_error_check(BitStream &bs) {
    if (protection_absent == 0) {
        crc_check = bs.getMultiBit(16);
    }
    return 0;
}

/*参考表38*/
/*由于给定的采样频率仅与一个采样频率表相关联，并且由于在可能的采样频率范围内需要最大的灵活性，
 * 因此应使用下表将隐含的采样频率与所需的采样频率相关表相关联。*/



int AdtsHeader::set_sample_rate_index() {
    if (sample_rate >= 92017) {
        sample_rate_index = 0;//96000
    } else if (sample_rate < 92017 && sample_rate >= 75132) {
        sample_rate_index = 1;//88200
    } else if (sample_rate < 75132 && sample_rate >= 55426) {
        sample_rate_index = 2;//64000
    } else if (sample_rate < 55426 && sample_rate >= 46009) {
        sample_rate_index = 3;//48000
    } else if (sample_rate < 46009 && sample_rate >= 37566) {
        sample_rate_index = 4;//44100
    } else if (sample_rate < 37566 && sample_rate >= 27713) {
        sample_rate_index = 5;//32000
    } else if (sample_rate < 27713 && sample_rate >= 23004) {
        sample_rate_index = 6;//24000
    } else if (sample_rate < 23004 && sample_rate >= 18783) {
        sample_rate_index = 7;//22050
    } else if (sample_rate < 18783 && sample_rate >= 13856) {
        sample_rate_index = 8;//16000
    } else if (sample_rate < 13856 && sample_rate >= 11502) {
        sample_rate_index = 9;//12000
    } else if (sample_rate < 11502 && sample_rate >= 9391) {
        sample_rate_index = 10;//11025
    } else if (sample_rate < 9391) {
        sample_rate_index = 11;//8000
    }
    return 0;
}

uint8_t AdtsHeader::get_Pred_SFB_MAX() {
    /*该变量在全局数据区分配内存*/
    /*静态局部变量在程序执行到该对象的声明处时被首次初始化，即以后的函数调用不再进行初始化*/
    /*它始终驻留在全局数据区，直到程序运行结束。但其作用域为局部作用域，当定义它的函数或语句块结束时，其作用域随之结束*/
    /*静态数据（即使是函数内部的静态局部变量）也存放在全局数据区。全局数据区的数据并不会因为函数的退出而释放空间*/
    static constexpr uint8_t pred_sfb_max[] = {
            33, 33, 38, 40, 40, 40, 41, 41, 37, 37, 37, 34
    };
    return pred_sfb_max[sample_rate_index];
}
