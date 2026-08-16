#include <stdint.h>
#include <system/audio.h>

/*
 * Legacy TypeConverter ABI used by old MTK audio blobs.
 *
 * Old audio.primary.mt6737t.so expects:
 *
 * android::TypeConverter<android::FormatTraits>::mTable
 *
 * Android 11 no longer exports that exact ABI symbol.
 */
struct LegacyAudioFormatEntry {
    const char* literal;
    audio_format_t value;
};

#define LEGACY_AUDIO_FORMAT(x) { #x, x }

extern "C" {

    extern const LegacyAudioFormatEntry gLegacyAudioFormatTable[]
    __asm__("_ZN7android13TypeConverterINS_12FormatTraitsEE6mTableE")
    __attribute__((visibility("default"), used)) = {
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_PCM_16_BIT),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_PCM_8_BIT),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_PCM_32_BIT),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_PCM_8_24_BIT),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_PCM_FLOAT),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_PCM_24_BIT_PACKED),

        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_MP3),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AMR_NB),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AMR_WB),

        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_MAIN),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_LC),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_SSR),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_LTP),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_HE_V1),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_SCALABLE),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_ERLC),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_LD),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_HE_V2),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AAC_ELD),

        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_VORBIS),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_HE_AAC_V1),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_HE_AAC_V2),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_OPUS),

        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_AC3),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_E_AC3),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_DTS),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_DTS_HD),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_IEC61937),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_DOLBY_TRUEHD),

        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_FLAC),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_ALAC),
        LEGACY_AUDIO_FORMAT(AUDIO_FORMAT_APE),

        { nullptr, AUDIO_FORMAT_DEFAULT },
    };

} // extern "C"

#undef LEGACY_AUDIO_FORMAT

extern "C" {
    bool _ZN7android11AudioSystem24getVoiceUnlockDLInstanceEv(){
        return 0;
    }

    int _ZN7android11AudioSystem23GetVoiceUnlockDLLatencyEv(){
        return 0;
    }

    int _ZN7android11AudioSystem17SetVoiceUnlockSRCEjj(uint32_t outSR, uint32_t outChannel){
        return 0;
    }

    bool _ZN7android11AudioSystem18startVoiceUnlockDLEv(){
        return 0;
    }

    int _ZN7android11AudioSystem15ReadRefFromRingEPvjS1_(void*buf, uint32_t datasz, void* DLtime){
        return 0;
    }

    int _ZN7android11AudioSystem20GetVoiceUnlockULTimeEPv(void* DLtime) {
        return 0;
    }

    void _ZN7android11AudioSystem25freeVoiceUnlockDLInstanceEv(){}

    bool _ZN7android11AudioSystem17stopVoiceUnlockDLEv(){
        return 0;
    }

    int _ZN7android11AudioSystem12SetAudioDataEijPv(int par1,size_t byte_len,void *ptr) {
        return 0;
    }

    int _ZN7android11AudioSystem15SetAudioCommandEii(int var1,int var2) {
        return 0;
    }

    int _ZN7android11AudioSystem15GetAudioCommandEiPi(int var1) {
        return 0;
    }
}
