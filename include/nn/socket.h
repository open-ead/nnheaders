#pragma once

#include <netdb.h>
#include <nn/types.h>
#include <nn/util.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>

namespace nn {
class TimeSpan;
namespace socket {
struct ResourceStatistics;

#if NN_SDK_VER >= NN_MAKE_VER(7, 0, 0)
struct InAddr {
    u32 addr;
};
#endif

// taken from https://switchbrew.org/wiki/Sockets_services#BsdBufferConfig
struct BsdBufferConfig {
    ulong tcp_tx_buf_size = 0x8000;  ///< Size of the TCP transfer (send) buffer (initial or fixed).
    ulong tcp_rx_buf_size = 0x10000;  ///< Size of the TCP recieve buffer (initial or fixed).
    ulong tcp_tx_buf_max_size =
        0x30000;  ///< Maximum size of the TCP transfer (send) buffer. If it is 0, the size of the
                  ///< buffer is fixed to its initial value.
    ulong tcp_rx_buf_max_size = 0x30000;  ///< Maximum size of the TCP receive buffer. If it is 0,
                                          ///< the size of the buffer is fixed to its initial value.
    ulong udp_tx_buf_size =
        0x2400;  ///< Size of the UDP transfer (send) buffer (typically 0x2400 bytes).
    ulong udp_rx_buf_size = 0xA500;  ///< Size of the UDP receive buffer (typically 0xA500 bytes).
    int sb_efficiency =
        4;  ///< Number of buffers for each socket (standard values range from 1 to 8).
};

struct Config {
    int unkInt1 = 2;  // 0x0 (value is 2 in SMO sdk, 8 in sv. could be BsdBufferConfig's version)
    bool unkBool1 = false;         // 0x4
    bool isUseBsdS = false;        // 0x5
    void* pool;                    // 0x8
    ulong poolSize;                // 0x10
    ulong allocPoolSize;           // 0x18
    BsdBufferConfig bufferConfig;  // 0x20-0x50
    int concurLimit;               // 0x54
    int padding;
};

static_assert(sizeof(Config) == 0x60, "Config Size");

s32 Recv(s32 socket, void* out, ulong outLen, s32 flags);
s32 RecvFrom(int, void*, ulong, int, sockaddr*, u32*);
s32 Send(s32 socket, const void* data, ulong dataLen, s32 flags);
s32 SendTo(int, const void*, ulong, int, const sockaddr*, u32);
s32 Accept(int, sockaddr*, u32*);
s32 Bind(int, const sockaddr*, u32);
nn::Result Connect(s32 socket, const sockaddr* address, u32 addressLen);
s32 GetPeerName(int, sockaddr*, u32*);
s32 GetSockName(int, sockaddr*, u32*);
s32 GetSockOpt(int, int, int, void*, u32*);
s32 Listen(int, int);
s32 SetSockOpt(s32 socket, s32 socketLevel, s32 option, const void*, u32 len);
s32 SockAtMark(int);
s32 Shutdown(int, int);
s32 ShutdownAllSockets(bool);
s32 Socket(s32 domain, s32 type, s32 protocol);
s32 SocketExempt(int, int, int);
s32 Write(int, const void*, ulong);
s32 Read(int, void*, ulong);
Result Close(s32 socket);
s32 Select(int, fd_set*, fd_set*, fd_set*, timeval*);
s32 Poll(pollfd*, ulong, int);
s32 Fcntl(int, int, ...);
s32 InetPton(int, const char*, void*);
const char* InetNtop(int af, const void* src, char* dst, u32 size);

#if NN_SDK_VER >= NN_MAKE_VER(7, 0, 0)
s32 InetAton(const char* addressStr, InAddr* addressOut);
char* InetNtoa(InAddr);
#endif

s32 InetAton(const char* addressStr, in_addr* addressOut);
char* InetNtoa(in_addr);

u16 InetHtons(u16 val);
u32 InetHtonl(u32);
u16 InetNtohs(u16);
u32 InetNtohl(u32);
s32 GetLastErrno();
void SetLastErrno(int);
s32 RecvMsg(int, msghdr*, int);
s32 RecvMMsg(int, mmsghdr*, ulong, int, nn::TimeSpan*);
s32 SendMsg(int, const msghdr*, int);
s32 SendMMsg(int, const mmsghdr*, ulong, int);
s32 Ioctl(int, u32, void*, ulong);
s32 Open(const char*, int);
Result Initialize(void* pool, ulong poolSize, ulong allocPoolSize, int concurLimit);
Result Initialize(nn::socket::Config const&);
s32 Finalize();
s32 GetAddrInfo(const char*, const char*, const addrinfo*, addrinfo**);
s32 GetAddrInfo(const char*, const char*, const addrinfo*, addrinfo**, int);
s32 GetAddrInfoWithoutNsdResolve(const char*, const char*, const addrinfo*, addrinfo**);
s32 GetAddrInfoWithoutNsdResolve(const char*, const char*, const addrinfo*, addrinfo**, int);
s32 FreeAddrInfo(addrinfo*);
s32 GetNameInfo(const sockaddr*, u32, char*, u32, char*, u32, int);
s32 GetNameInfo(const sockaddr*, u32, char*, u32, char*, u32, int, int);
hostent* GetHostByName(const char* name);
hostent* GetHostByName(const char*, int);
hostent* GetHostByNameWithoutNsdResolve(const char*);
hostent* GetHostByNameWithoutNsdResolve(const char*, int);
hostent* GetHostByAddr(const void*, u32, int);
hostent* GetHostByAddr(const void*, u32, int, int);
s32 RequestCancelHandle();
s32 Cancel(int);
s32 GetHErrno();
s32 HStrError(int);
s32 GAIStrError(int);
s32 Sysctl(int*, ulong, void*, ulong*, void*, ulong);
s32 DuplicateSocket(int, ulong);
s32 GetResourceStatistics(ResourceStatistics*, ulong);

}  // namespace socket
}  // namespace nn

extern "C" {

int nnsocketRecv(int socket, void* out, ulong outLen, int flags);
int nnsocketRecvFrom(int, void*, ulong, int, sockaddr*, u32*);
int nnsocketSend(int socket, const void* data, ulong dataLen, int flags);
int nnsocketSendTo(int, const void*, ulong, int, const sockaddr*, u32);
int nnsocketAccept(int, sockaddr*, u32*);
int nnsocketBind(int, const sockaddr*, u32);
u32 nnsocketConnect();  // returns nn::Result
int nnsocketGetPeerName(int, sockaddr*, u32*);
int nnsocketGetSockName(int, sockaddr*, u32*);
int nnsocketGetSockOpt(int, int, int, void*, u32*);
int nnsocketListen(int, int);
int nnsocketSetSockOpt(int socket, int socketLevel, int option, const void*, u32 len);
int nnsocketSockAtMark(int);
int nnsocketShutdown(int, int);
int nnsocketSocket(bool);
int nnsocketWrite(int domain, int type, int protocol);
int nnsocketRead(int, int, int);
u32 nnsocketClose();  // returns nn::Result
void nnsocketSelect(int, fd_set*, fd_set*, fd_set*, timeval*);
void nnsocketPoll(pollfd*, ulong, int);
void nnsocketFcntl(int, int, ...);
void nnsocketInetPton(int, const char*, void*);
const char* nnsocketInetNtop(int af, const void* src, char* dst, u32 size);
s32 nnsocketInetAton(const char* addressStr, in_addr* addressOut);
char* nnsocketInetNtoa(in_addr);
u16 nnsocketInetHtons(u16 val);
u32 nnsocketInetHtonl(u32);
u16 nnsocketInetNtohs(u16);
u32 nnsocketInetNtohl(u32);
s32 nnsocketGetLastErrno();
void nnsocketSetLastErrno(int);
s32 nnsocketRecvMsg(int, msghdr*, int);
s32 nnsocketSendMsg(int, const msghdr*, int);
s32 nnsocketIoctl(int, u32, void*, ulong);
s32 nnsocketOpen(const char*, int);
u32 nnsocketInitialize(void* pool, ulong poolSize, ulong allocPoolSize, int concurLimit);
s32 nnsocketFinalize();
s32 nnsocketGetAddrInfo(const char*, const char*, const addrinfo*, addrinfo**);
s32 nnsocketGetAddrInfoCancel(const char*, const char*, const addrinfo*, addrinfo**, int);
s32 nnsocketGetAddrInfoWithoutNsdResolve(const char*, const char*, const addrinfo*, addrinfo**);
s32 nnsocketGetAddrInfoWithoutNsdResolveCancel(const char*, const char*, const addrinfo*,
                                               addrinfo**, int);
s32 nnsocketFreeAddrInfo(addrinfo*);
s32 nnsocketGetNameInfo(const sockaddr*, u32, char*, u32, char*, u32, int);
s32 nnsocketGetNameInfoCancel(const sockaddr*, u32, char*, u32, char*, u32, int, int);
hostent* nnsocketGetHostByName(const char* name);
hostent* nnsocketGetHostByNameCancel(const char*, int);
hostent* nnsocketGetHostByNameWithoutNsdResolve(const char*);
hostent* nnsocketGetHostByNameWithoutNsdResolveCancel(const char*, int);
hostent* nnsocketGetHostByAddr(const void*, u32, int);
hostent* nnsocketGetHostByAddrCancel(const void*, u32, int, int);
s32 nnsocketRequestCancelHandle();
s32 nnsocketCancel(int);
s32 nnsocketGetHErrno();
s32 nnsocketHStrError(int);
s32 nnsocketGAIStrError(int);
s32 nnsocketSysctl(int*, ulong, void*, ulong*, void*, ulong);
s32 nnsocketDuplicateSocket(int, ulong);
}
