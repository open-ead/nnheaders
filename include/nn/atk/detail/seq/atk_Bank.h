#pragma once

#include <nn/atk/detail/atk_NoteOnCallback.h>
#include <nn/atk/detail/atk_WaveArchiveFileReader.h>
#include <nn/atk/detail/seq/atk_BankFileReader.h>

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