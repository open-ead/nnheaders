#pragma once

#include <iterator>
#include "nn/types.h"
#include "nn/util/AccessorBase.h"
#include "nn/util/BinaryTypes.h"

namespace nn::util {

struct ResDicData {
    struct Entry {
        int32_t refBit;
        uint16_t children[2];
        BinPtrToString pKey;
    };

    BinBlockSignature signature;
    int32_t count;
    Entry entries[1];
};

// TODO: clean up ore::ResDic based on this implementation
// (this one is likely to be more accurate since it is based on debug symbols/info)
class ResDic : public AccessorBase<ResDicData> {
public:
    static constexpr int Npos = -1;

    ResDic(const ResDic&) = delete;
    auto operator=(const ResDic&) = delete;

    int GetCount() const { return ToData().count; }

    string_view GetKey(int index) const { return ToData().entries[1 + index].pKey.Get()->Get(); }

    int FindIndex(const string_view& key) const {
        const Entry* entry = FindImpl(key);
        return *entry->pKey.Get() == key ?
                   static_cast<int>(std::distance(&ToData().entries[1], entry)) :
                   Npos;
    }

    bool Build();

    static size_t CalculateSize(int numEntries) {
        size_t size = 0;
        size += sizeof(ResDicData);
        size += sizeof(ResDicData::Entry) * numEntries;
        return size;
    }

private:
    static int ExtractRefBit(const string_view& key, int refBit) {
        int index = refBit >> 3;
        if (u32(index) < key.length()) {
            int bitIndex = refBit & 7;
            return (key[key.length() - index - 1] >> bitIndex) & 1;
        }
        return 0;
    }

    static int FindRefBit(const string_view& lhs, const string_view& rhs);

    static bool Older(const ResDicData::Entry* parent, const ResDicData::Entry* child) {
        return parent->refBit < child->refBit;
    }

    ResDicData::Entry* GetChild(const ResDicData::Entry* parent, const string_view& key) {
        int bit = ExtractRefBit(key, parent->refBit);
        int index = parent->children[bit];
        return &ToData().entries[index];
    }

    const ResDicData::Entry* GetChild(const ResDicData::Entry* parent,
                                      const string_view& key) const {
        int bit = ExtractRefBit(key, parent->refBit);
        int index = parent->children[bit];
        return &ToData().entries[index];
    }

    void SetChild(ResDicData::Entry* parent, ResDicData::Entry* child) {
        int bit = ExtractRefBit(*child->pKey.Get(), parent->refBit);
        auto index = std::distance(ToData().entries, child);
        parent->children[bit] = static_cast<uint16_t>(index);
    }

    const ResDicData::Entry* FindImpl(const string_view& key) const {
        const Entry* parent = &ToData().entries[0];
        const Entry* child = &ToData().entries[parent->children[0]];

        while (Older(parent, child)) {
            parent = child;
            child = GetChild(child, key);
        }
        return child;
    }
};

}  // namespace nn::util
