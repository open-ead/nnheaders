#pragma once

#include <nn/types.h>

namespace nn::nfp {

using DeviceHandle = u64;

const s32 AmiiboNameLength = 10;

enum State : u32 {
    State_NonInitialized = 0,
    State_Initialized = 1,
};

enum DeviceState : u32 {
    DeviceState_Initialized = 0,
    DeviceState_SearchingForTag = 1,
    DeviceState_TagFound = 2,
    DeviceState_TagRemoved = 3,
    DeviceState_TagMounted = 4,
    DeviceState_Unavailable = 5,
    DeviceState_Finalized = 6,
};

enum ModelType : u32 {
    ModelType_Amiibo = 0,
};

enum MountTarget : u32 {
    MountTarget_None = 0,
    MountTarget_Rom = 1,
    MountTarget_Ram = 2,
    MountTarget_All = 3,
};

enum TagType : u32 {
    TagType_None = 0,
    TagType_Type1 = 1U << 0,
    TagType_Type2 = 1U << 1,
    TagType_Type3 = 1U << 2,
    TagType_Type4A = 1U << 3,
    TagType_Type4B = 1U << 4,
    TagType_Type5 = 1U << 5,
    TagType_Mifare = 1U << 6,
    TagType_All = 0xFFFFFFFF,
};

enum Protocol : u32 {
    Protocol_None = 0,
    Protocol_TypeA = 1,
    Protocol_TypeB = 1 << 1,
    Protocol_TypeF = 1 << 2,
    Protocol_All = 0xFFFFFFFF,
};

struct AmiiboDate {
    s16 year;
    s8 month;
    s8 day;
};

struct TagId {
    u8 uuid[10];
    u8 uuidLength;
    char reserved[0x15];
};

struct TagInfo {
    TagId id;
    Protocol protocol;
    TagType type;
    char reserved2[0x30];
};

struct CommonInfo {
    AmiiboDate lastWriteDate;
    u16 writeCounter;
    u16 version;
    u32 applicationAreaSize;
    char reserved[0x34];
};

struct ModelInfo {
    u8 gameId;
    u8 characterId;
    u8 characterVariant;
    u8 amiiboType;
    u16 modelNumber;
    u8 series;
    char reserved[0x39];
};

struct RegisterInfo {
    char miiStoreData[0x44];  // nn::mii::StoreData
    AmiiboDate registerDate;
    char nickname[AmiiboNameLength * 4 + 1];  // utf-8, null-terminated
    u8 fontRegion;
    char reserved[0x7a];
};

}  // namespace nn::nfp
