#pragma once

#include <nn/font/font_Util.h>
#include <nn/types.h>

namespace nn {
namespace font {

template <class CharType>
class TextWriterBase;

class Rectangle;

template <class CharType>
class PrintContext {
public:
    TextWriterBase<CharType>* writer;
    const CharType* str;
    const CharType* strEnd;
    const float xOrigin;
    const float yOrigin;
    const float hScale;
    const float vScale;
    uint32_t flags;
    uint32_t prevCode;

    PrintContext(TextWriterBase<CharType>*, const CharType*, const CharType*, const float,
                 const float, const float, const float, uint32_t);

private:
    const PrintContext& operator=(const PrintContext<CharType>&);
};

template <class CharType>
class TagProcessorBase {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    typedef PrintContext<CharType> ContextType;

    enum Operation {
        Operation_Default,
        Operation_NoCharSpace,
        Operation_CharSpace,
        Operation_NextLine,
        Operation_EndDraw,
        Operation_MaxOperation
    };

    TagProcessorBase();
    virtual ~TagProcessorBase();
    virtual Operation Process(uint32_t, ContextType*);
    virtual Operation CalculateRect(Rectangle*, ContextType*, uint32_t);
    virtual void BeginPrint(ContextType*);
    virtual void EndPrint(ContextType*);
    virtual void BeginCalculateRect(ContextType*);
    virtual void EndCalculateRect(ContextType*);
    virtual void BeginCalculateRectWhole(const TextWriterBase<CharType>*, const CharType*,
                                         const CharType*);
    virtual void EndCalculateRectWhole(const TextWriterBase<CharType>*, const CharType*,
                                       const CharType*);
    virtual const CharType* AcquireNextPrintableChar(bool*, const CharType*);

private:
    void ProcessLinefeed(ContextType*) const;
    void ProcessTab(ContextType*) const;
};

}  // namespace font
}  // namespace nn
