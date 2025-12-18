#pragma once
#include <vector>
#include <xaudio2.h>

struct SoundData
{
    std::vector<BYTE> buffer;
    WAVEFORMATEX format;
};
