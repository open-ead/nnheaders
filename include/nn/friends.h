/**
 * @file friends.h
 * @brief Friend implementation.
 */

#pragma once

#include <nn/account.h>
#include <nn/os.h>

namespace nn {
namespace friends {
struct Url {
    char m_Buffer[0xA0];
};
struct ImageSize {
    s32 m_Size;
};

class AsyncContext;
class Profile;

void Initialize();
Result GetProfileList(nn::friends::AsyncContext* context, nn::friends::Profile* profiles,
                      nn::account::Uid const& userID,
                      nn::account::NetworkServiceAccountId const* accountIDs, s32 numAccounts);

class Profile {
public:
    Profile();

    nn::account::NetworkServiceAccountId GetAccountId() const;
    nn::account::Nickname& GetNickname() const;
    bool IsValid() const;
    Result GetProfileImageUrl(nn::friends::Url*, ImageSize) const;
};

class AsyncContext {
public:
    AsyncContext();
    ~AsyncContext();

    Result GetSystemEvent(nn::os::SystemEvent*);
    Result GetResult() const;
};
}  // namespace friends
}  // namespace nn
