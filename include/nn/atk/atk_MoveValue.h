#pragma once

namespace nn::atk::detail {

template <typename ValueType, typename CountType>
class MoveValue {
public:
    MoveValue() = default;

    void InitValue(ValueType value) {
        m_Origin = value;
        m_Target = value;
        m_Frame = 0;
        m_Counter = 0;
    }

    void SetTarget(ValueType targetValue, CountType frames) {
        m_Origin = GetValue();
        m_Target = targetValue;
        m_Frame = frames;
        m_Counter = 0;
    }

    ValueType GetTarget() const { return m_Target; }

    ValueType GetValue() const {
        if (IsFinished())
            return m_Target;

        return static_cast<ValueType>(m_Origin + (m_Target - m_Origin) * m_Counter / m_Frame);
    }

    void Update() {
        if (m_Counter < m_Frame)
            ++m_Counter;
    }

    bool IsFinished() const { return m_Counter >= m_Frame; }

    CountType GetRemainingCount() const {
        if (IsFinished())
            return 0;

        return m_Frame - m_Counter;
    }

private:
    ValueType m_Origin{0};
    ValueType m_Target{0};
    CountType m_Frame{0};
    CountType m_Counter{0};
};

}  // namespace nn::atk::detail