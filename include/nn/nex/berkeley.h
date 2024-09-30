#pragma once

#include <nn/nex/socket.h>
#include <nn/types.h>

namespace nn::nex {

class BerkeleySocketDriver
    : SocketDriver {  // inherits SocketDriver and RootObject but not documented
public:
    class BerkeleySocket : public SocketDriver::Socket {
    public:
        BerkeleySocket();
        BerkeleySocket(const BerkeleySocket*, int);
        ~BerkeleySocket();

        bool Open(TransportProtocol::Type);
        bool SetAsync(bool);
        bool SetBroadcastMode(bool);
        bool Bind(u16&);
        bool LastSocketErrorToResult(const char*, long);
        s32 GetLastSocketError(long);
        s32 RecvFrom(u8*, ulong, SocketDriver::InetAddress*, u64*, SocketDriver::_SocketFlag);
    };

    ~BerkeleySocketDriver() override;
};

}  // namespace nn::nex
