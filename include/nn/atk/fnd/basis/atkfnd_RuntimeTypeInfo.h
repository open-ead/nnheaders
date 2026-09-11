#pragma once

namespace nn::atk::detail::fnd {

class RuntimeTypeInfo {
public:
    explicit RuntimeTypeInfo(const RuntimeTypeInfo* parent) : m_ParentTypeInfo{parent} {}

    // UNCHECKED
    bool IsDerivedFrom(const RuntimeTypeInfo* s_TypeInfo) const {
        const RuntimeTypeInfo* self{this};

        if (self == s_TypeInfo)
            return true;

        if (m_ParentTypeInfo == nullptr)
            return false;

        return m_ParentTypeInfo->IsDerivedFrom(s_TypeInfo);
    }

private:
    const RuntimeTypeInfo* m_ParentTypeInfo;
};
static_assert(sizeof(RuntimeTypeInfo) == 0x8);

#define NN_ATK_RTTI_BASE(CLASS)                                                                    \
public:                                                                                            \
    static const nn::atk::detail::fnd::RuntimeTypeInfo* GetRuntimeTypeInfoStatic() {               \
        static const nn::atk::detail::fnd::RuntimeTypeInfo s_TypeInfo{nullptr};                    \
        return &s_TypeInfo;                                                                         \
    }                                                                                              \
                                                                                                   \
    virtual const nn::atk::detail::fnd::RuntimeTypeInfo* GetRuntimeTypeInfo() const {              \
        return CLASS::GetRuntimeTypeInfoStatic();                                                  \
    }

#define NN_ATK_RTTI_OVERRIDE(CLASS, BASE)                                                          \
public:                                                                                            \
    static const nn::atk::detail::fnd::RuntimeTypeInfo* GetRuntimeTypeInfoStatic() {               \
        static const nn::atk::detail::fnd::RuntimeTypeInfo s_TypeInfo{                             \
            BASE::GetRuntimeTypeInfoStatic()};                                                     \
        return &s_TypeInfo;                                                                         \
    }                                                                                              \
                                                                                                   \
    const nn::atk::detail::fnd::RuntimeTypeInfo* GetRuntimeTypeInfo() const override {             \
        return CLASS::GetRuntimeTypeInfoStatic();                                                  \
    }

template <typename TToPtr, typename TFrom>
inline TToPtr* DynamicCast(TFrom* obj) {
    RuntimeTypeInfo* typeInfoU{TToPtr::GetRuntimeTypeInfoStatic()};

    if (typeInfoU->IsDerivedFrom(obj->GetRuntimeTypeInfo()))
        return static_cast<TToPtr*>(obj);

    return nullptr;
}

}  // namespace nn::atk::detail::fnd
