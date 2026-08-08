#pragma once

#include <nn/atk/atk_NoteOnCallback.h>
#include <nn/atk/atk_WaveArchiveFileReader.h>
#include <nn/atk/atk_BankFileReader.h>

namespace nn::atk::detail::driver {
class Bank {
public:
    Bank();
    ~Bank();

    Channel* NoteOn(const BankFileReader& bankReader, 
                    const WaveArchiveFileReader& warcReader, 
                    const NoteOnInfo& noteOnInfo) const;
    
    static f32 CalcChannelVelocityVolume(u8 velocity);
};
} // namespace nn::atk::detail::driver