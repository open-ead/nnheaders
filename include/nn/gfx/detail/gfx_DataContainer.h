#pragma once

namespace nn::gfx::detail {

template <class T>
class CasterBase {
protected:
    T* m_Value;

public:
    CasterBase(T* value) : m_Value(value) {}
    T* operator->();
    operator bool();
    operator bool() const;
    bool IsNull();

    template <class U>
    operator U*() {
        return static_cast<U*>(m_Value);
    }
};

template <class T>
class Caster : public CasterBase<T> {
public:
    Caster(T* value) : CasterBase<T>(value) {}

    template <class U>
    operator U&() {
        return static_cast<U&>(*CasterBase<T>::m_Value);
    }
};

template <class T>
class Caster<const T> : public CasterBase<const T> {
public:
    Caster(const T* value) : CasterBase<const T>(value) {}

    template <class U>
    operator const U&() {
        return static_cast<const U&>(*CasterBase<const T>::m_Value);
    }
};

template <class T>
class Caster<volatile T> : public CasterBase<volatile T> {
public:
    Caster(volatile T* value) : CasterBase<volatile T>(value) {}
};

template <class T>
class Caster<const volatile T> : public CasterBase<const volatile T> {
public:
    Caster(const volatile T* value) : CasterBase<const volatile T>(value) {}
};

template <class TData>
class DataContainer : protected TData {
public:
    typedef TData DataType;

    DataContainer() : TData() {}

    Caster<TData> ToData() { return Caster<TData>(static_cast<DataType*>(this)); }

    Caster<const TData> ToData() const {
        return Caster<const TData>(static_cast<const DataType*>(this));
    }

    Caster<volatile TData> ToData() volatile;
    Caster<const volatile TData> ToData() const volatile;

    static Caster<DataContainer> DataToAccessor(TData* pData) {
        return Caster<DataContainer>(static_cast<DataContainer*>(pData));
    }

    static Caster<const DataContainer> DataToAccessor(const TData* pData) {
        return Caster<const DataContainer>(static_cast<const DataContainer*>(pData));
    }

    static Caster<volatile DataContainer> DataToAccessor(volatile TData*);
    static Caster<const volatile DataContainer> DataToAccessor(const volatile TData*);

    static Caster<DataContainer> DataToAccessor(TData& data) { return DataToAccessor(&data); }

    static Caster<const DataContainer> DataToAccessor(const TData& data) {
        return DataToAccessor(&data);
    }

    static Caster<volatile DataContainer> DataToAccessor(volatile TData&);
    static Caster<const volatile DataContainer> DataToAccessor(const volatile TData&);
};

}  // namespace nn::gfx::detail