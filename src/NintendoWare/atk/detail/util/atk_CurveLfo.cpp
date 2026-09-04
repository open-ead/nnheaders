#include <nn/atk/atk_CurveLfo.h>

#include <cstring>

#include <nn/util/util_Arithmetic.h>

#include <nn/atk/atk_Util.h>

namespace nn::atk::detail {

namespace {

float CurveSine(float arg) {
    return util::SinTable(static_cast<util::AngleIndex>(arg * UINT_MAX));
}

float CurveSquare(float arg) {
    return arg < 0.5f ? 0.0f : 1.0f;
}

float CurveSaw(float arg) {
    return arg;
}

float CurveTriangle(float arg) {
    static const float gradients[5]{4.0, -4.0, -4.0, 4.0, 4.0};
    static const float intercepts[5]{0.0, 2.0, 2.0, -4.0, 0.0};

    u32 tmp{static_cast<u32>(arg * 4)};

    return gradients[tmp] * arg + intercepts[tmp];
}

float CurveRandom([[maybe_unused]] float arg) {
    u16 rand{Util::CalcRandom()};
    float ret{rand / 65535.0f};

    return ret;
}

CurveLfo::CurveFunc g_CurveFuncTable[128];

}  // anonymous namespace

void CurveLfo::InitializeCurveTable() {
    std::memset(static_cast<void*>(g_CurveFuncTable), 0, sizeof(g_CurveFuncTable));
    g_CurveFuncTable[0] = CurveSine;
    g_CurveFuncTable[1] = CurveTriangle;
    g_CurveFuncTable[2] = CurveRandom;
    g_CurveFuncTable[3] = CurveSaw;
    g_CurveFuncTable[4] = CurveSquare;
}

CurveLfo::CurveFunc CurveLfo::RegisterUserCurve(CurveFunc func, u32 index) {
    s32 tableIndex = {static_cast<s32>(index + CurveLfoParam::CurveType_UserMin)};
    
    CurveFunc tmp{g_CurveFuncTable[tableIndex]};
    g_CurveFuncTable[tableIndex] = func;

    return tmp;
}

void CurveLfo::Reset() {
    m_Counter = 0.0f;
    m_RandomValue = 1.0f;
    m_DelayCounter = 0;
    m_IsStart = false;
    m_IsNext = false;
}

void CurveLfo::Update(int msec) {
    if (m_DelayCounter < m_Param.delay && m_DelayCounter + msec <= m_Param.delay) {
        m_DelayCounter += msec;
        return;
    }

    if (m_DelayCounter < m_Param.delay) {
        msec -= m_Param.delay - m_DelayCounter;
        m_DelayCounter = m_Param.delay;
    }

    if (m_Param.speed > 0.0f) {
        if (!m_IsStart) {
            m_Counter = m_Param.phase / 127.0f;
            m_IsStart = true;
        }

        m_Counter += (m_Param.speed * msec) / 1000.0f;
        m_IsNext = m_Counter >= 1.0f;
        m_Counter -= static_cast<s32>(m_Counter);
    }
}

float CurveLfo::GetValue() const {
    if (m_Param.depth == 0.0f)
        return 0.0f;

    if (m_DelayCounter < m_Param.delay)
        return 0.0f;

    CurveFunc func{g_CurveFuncTable[m_Param.curve]};

    float value{0};

    if (func != nullptr) {
        if (m_Param.curve == CurveLfoParam::CurveType_Random) {
            if (m_IsNext)
                m_RandomValue = func(m_Counter);

            value = m_RandomValue;
        } else {
            value = func(m_Counter);
        }
    } else {
        value = 1.0f;
    }

    value *= m_Param.depth;
    value *= static_cast<float>(m_Param.range);

    return value;
}

}  // namespace nn::atk::detail