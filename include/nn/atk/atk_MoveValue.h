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
    ValueType m_Origin{0};
    ValueType m_Target{0};
    CountType m_Frame{0};
    CountType m_Counter{0};
};

}  // namespace nn::atk::detail