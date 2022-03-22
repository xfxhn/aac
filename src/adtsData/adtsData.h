
#ifndef AACDECODER_ADTSDATA_H
#define AACDECODER_ADTSDATA_H

class BitStream;

class AdtsData {
public:
    uint8_t pce_tag_present;
    uint8_t pce_instance_tag;
    uint8_t drc_tag_reserved_bits;
    uint8_t excluded_chns_present;
    uint8_t drc_bands_present;
    uint8_t drc_band_incr;
    uint8_t drc_bands_reserved_bits;
    uint8_t drc_band_top[17];
    uint8_t prog_ref_level_present;
    uint8_t prog_ref_level;
    uint8_t prog_ref_level_reserved_bits;
    uint8_t dyn_rng_sgn[17];
    uint8_t dyn_rng_ctl[17];
    uint8_t exclude_mask[7];
    uint8_t additional_excluded_chns[64];

public:
    int raw_data_block(BitStream &bs);

private:
    int fill_element(BitStream &bs);

    int extension_payload(BitStream &bs, uint16_t cnt);

    uint8_t dynamic_range_info(BitStream &bs);

    uint8_t excluded_channels(BitStream &bs);
};

#endif //AACDECODER_ADTSDATA_H
