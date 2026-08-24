#pragma once

namespace nn::atk::detail {
template <typename ValueType, typename CountType>
class MoveValue {
public:
    MoveValue() = default;

    void InitValue(ValueType value) {
        m_Origin = value;
        m_Target = value;
        m_Counter = 0;
        m_Frame = 0;
    }

    void SetTarget(ValueType targetValue, CountType frames);

    ValueType GetTarget() const;
    ValueType GetValue() const;

    void Update();

    bool IsFinished() const;

    CountType GetRemainingCount() const;

private:
    ValueType m_Origin {};
    ValueType m_Target {};
    CountType m_Frame {};
    CountType m_Counter {};
};
} // namespace nn::atk::detail