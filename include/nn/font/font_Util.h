#pragma once

#include <type_traits>

namespace nn::font {

namespace detail {

class RuntimeTypeInfo {
public:
    const RuntimeTypeInfo* m_ParentTypeInfo;

    explicit RuntimeTypeInfo(const RuntimeTypeInfo* parent) : m_ParentTypeInfo(parent) {}
    bool IsDerivedFrom(const RuntimeTypeInfo* s_TypeInfo) const {
        const RuntimeTypeInfo* self = this;

        while (self) {
            if (self == s_TypeInfo) {
                return true;
            }

            self = self->m_ParentTypeInfo;
        }

        return false;
    }
};

}  // namespace detail

template <typename TToPtr, typename TFrom>
TToPtr DynamicCast(TFrom* obj) {
    const detail::RuntimeTypeInfo* typeInfoU =
        std::remove_pointer<TToPtr>::type::GetRuntimeTypeInfoStatic();
    if (obj && obj->GetRuntimeTypeInfo()->IsDerivedFrom(typeInfoU)) {
        return static_cast<TToPtr>(obj);
    }
    return nullptr;
}

template <typename To, typename From>
bool IsDerivedFrom(const From* instance) {
    const detail::RuntimeTypeInfo* typeInfoU = To::GetRuntimeTypeInfoStatic();
    return (instance && instance->GetRuntimeTypeInfo()->IsDerivedFrom(typeInfoU));
}

}  // namespace nn::font

// todo: figure out where to put this
#define NN_RUNTIME_TYPEINFO_BASE()                                                                 \
    static const nn::font::detail::RuntimeTypeInfo* GetRuntimeTypeInfoStatic() {                   \
        static const nn::font::detail::RuntimeTypeInfo s_TypeInfo(nullptr);                        \
        return &s_TypeInfo;                                                                        \
    }                                                                                              \
                                                                                                   \
    virtual const nn::font::detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const {                  \
        return GetRuntimeTypeInfoStatic();                                                         \
    }

#define NN_RUNTIME_TYPEINFO(BASE)                                                                  \
    static const nn::font::detail::RuntimeTypeInfo* GetRuntimeTypeInfoStatic() {                   \
        static const nn::font::detail::RuntimeTypeInfo s_TypeInfo(                                 \
            BASE::GetRuntimeTypeInfoStatic());                                                     \
        return &s_TypeInfo;                                                                        \
    }                                                                                              \
                                                                                                   \
    virtual const nn::font::detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const {                  \
        return GetRuntimeTypeInfoStatic();                                                         \
    }
