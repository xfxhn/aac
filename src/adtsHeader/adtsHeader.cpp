
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
