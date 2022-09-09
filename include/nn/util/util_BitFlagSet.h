#pragma once

#include <nn/types.h>
#include <type_traits>

namespace nn::util {

template <int N, typename Tag>
struct BitFlagSet {
    // todo: figure out the condition based off other types
    typedef typename std::conditional<N == 32, uint32_t, uint64_t>::type StorageT;

    static const int StorageBitCount = N;
    static const int StorageCount = N / (8 * sizeof(StorageT));
    StorageT _storage[StorageCount];

    class Reference {
    public:
        Reference& operator=(bool);
        Reference& operator=(const Reference&);
        Reference& Flip();
        bool operator~() const;
        operator bool() const;

    private:
        BitFlagSet* m_Set;
        int m_Index;

        Reference();
        Reference(BitFlagSet&, int);
    };

    template <int BitIndex>
    struct Flag {
    public:
        static const int Index = BitIndex;
        static const BitFlagSet Mask = 1 << Index;

    private:
        static const int StorageIndex = BitIndex / StorageBitCount;
        static const StorageT StorageMask = 1 << Index % StorageBitCount;
    };

    BitFlagSet operator~() const;
    BitFlagSet operator&(const BitFlagSet&) const;
    BitFlagSet operator^(const BitFlagSet&) const;
    BitFlagSet operator|(const BitFlagSet&) const;
    BitFlagSet& operator&=(const BitFlagSet&);
    BitFlagSet& operator^=(const BitFlagSet&);
    BitFlagSet& operator|=(const BitFlagSet&);
    bool operator==(const BitFlagSet&) const;
    bool operator!=(const BitFlagSet&) const;
    bool operator[](int) const;
    Reference operator[](int);
    bool IsAnyOn() const;
    int CountPopulation() const;
    BitFlagSet& Flip(int);
    BitFlagSet& Flip();
    bool IsAllOn() const;
    bool IsAllOff() const;
    BitFlagSet& Reset();
    BitFlagSet& Reset(int);
    BitFlagSet& Set();
    BitFlagSet& Set(int, bool);
    int GetCount() const;
    bool Test(int) const;

private:
    BitFlagSet& SetImpl(int, StorageT, bool);
    bool TestImpl(int, StorageT) const;
    void Truncate();
    void TruncateIf(std::true_type);
    void TruncateIf(std::false_type);
    static int GetStorageIndex(int);
    static StorageT MakeStorageMask(int);
};

}  // namespace nn::util
