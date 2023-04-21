#pragma once

#include <nn/types.h>
#include <type_traits>

namespace nn::util {

template <int N, typename Tag>
struct BitFlagSet {
    // todo: figure out the condition based off other types
    typedef typename std::conditional<N == 32, uint32_t, uint64_t>::type StorageT;

    static const int StorageBitCount = 8 * sizeof(StorageT);  // 64 for N == 256
    static const int StorageCount = N / StorageBitCount;      // 4 for N == 256
    StorageT _storage[StorageCount];

    class Reference {
    public:
        Reference& operator=(bool isOn) {
            m_Set->Set(m_Index, isOn);
            return *this;
        }

        Reference& operator=(const Reference& ref) {
            m_Set->Set(m_Index, ref);
            return *this;
        }

        Reference& Flip() {
            m_Set->Flip(m_Index);
            return *this;
        }

        bool operator~() const { return !m_Set->Test(m_Index); }

        operator bool() const { return m_Set->Test(m_Index); }

    private:
        BitFlagSet* m_Set;
        int m_Index;

        Reference() : m_Set(nullptr), m_Index(0) {}
        Reference(BitFlagSet& set, int index) : m_Set(&set), m_Index(index) {}
    };

    template <int BitIndex>
    struct Flag {
    public:
        static const int Index = BitIndex;
        // todo: figure out how to make this not break past 63
        static const BitFlagSet Mask = 1 << Index;

    private:
        static const int StorageIndex = GetStorageIndex(BitIndex);
        static const StorageT StorageMask = MakeStorageMask(BitIndex);
    };

    BitFlagSet operator~() const {
        BitFlagSet tmp = *this;
        temp.Flip();
        return tmp;
    }

    BitFlagSet operator&(const BitFlagSet& other) const {
        BitFlagSet value = *this;
        value &= other;
        return value;
    }

    BitFlagSet operator^(const BitFlagSet& other) const {
        BitFlagSet value = *this;
        value ^= other;
        return value;
    }

    BitFlagSet operator|(const BitFlagSet& other) const {
        BitFlagSet value = *this;
        value |= other;
        return value;
    }

    BitFlagSet& operator&=(const BitFlagSet& other) {
        for (int i = 0; i < StorageCount; ++i) {
            _storage[i] &= other._storage[i];
        }

        return *this;
    }

    BitFlagSet& operator^=(const BitFlagSet& other) {
        for (int i = 0; i < StorageCount; ++i) {
            _storage[i] ^= other._storage[i];
        }

        return *this;
    }

    BitFlagSet& operator|=(const BitFlagSet& other) {
        for (int i = 0; i < StorageCount; ++i) {
            _storage[i] |= other._storage[i];
        }

        return *this;
    }

    bool operator==(const BitFlagSet& other) const {
        for (int i = 0; i < StorageCount; ++i) {
            if (_storage[i] != other._storage[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const BitFlagSet& other) const { return !(*this == other); }

    bool operator[](int index) const { return Test(index); }

    Reference operator[](int index) { return Reference(*this, index); }

    bool IsAnyOn() const;
    int CountPopulation() const;

    BitFlagSet& Flip(int index) { return Set(index, !Test(index)); }

    BitFlagSet& Flip() {
        for (int i = 0; i < StorageCount; ++i) {
            _storage[i] = ~_storage[i];
        }
        return *this;
    }

    bool IsAllOn() const;
    bool IsAllOff() const;

    BitFlagSet& Reset() {
        for (int i = 0; i < StorageCount; ++i) {
            _storage[i] = 0;
        }
        return *this;
    }

    BitFlagSet& Reset(int index) { return Set(index, false); }

    BitFlagSet& Set() {
        for (int i = 0; i < StorageCount; ++i) {
            _storage[i] = ~0;
        }
        return *this;
    }

    BitFlagSet& Set(int index, bool isOn) {
        return SetImpl(GetStorageIndex(index), MakeStorageMask(index), isOn);
    }

    int GetCount() const;

    bool Test(int index) const { return TestImpl(GetStorageIndex(index), MakeStorageMask(index)); }

private:
    BitFlagSet& SetImpl(int storageIndex, StorageT storageMask, bool isOn) {
        if (isOn) {
            _storage[storageIndex] |= storageMask;
        } else {
            _storage[storageIndex] &= ~storageMask;
        }
        return *this;
    }

    bool TestImpl(int storageIndex, StorageT storageMask) const {
        return _storage[storageIndex] & storageMask;
    }

    // todo: figure out the purpose of this
    void Truncate();
    void TruncateIf(std::true_type);
    void TruncateIf(std::false_type);

    static int GetStorageIndex(int index) { return index / StorageBitCount; }

    static StorageT MakeStorageMask(int index) { return StorageT(1) << index % StorageBitCount; }
};

}  // namespace nn::util
