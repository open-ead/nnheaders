#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include "types.h"

namespace nn {

namespace applet {
enum ExitReason { Normal = 0, Canceled = 1, Abnormal = 2, Unexpected = 10 };
}

namespace swkbd {
enum Preset { Default, Password, UserName, DownloadCode, Max_Preset };

enum KeyboardMode {
    ModeLanguageSet1,
    ModeNumeric,
    ModeASCII,
    ModeLanguageSet1Latin,
    ModeAlphabet,
    ModeSimplifiedChinese,
    ModeTraditionalChinese,
    ModeKorean,
    ModeLanguageSet2,
    ModeMax_KeyboardMode,
    ModeFull,
    ModeFullLatin
};

enum InvalidChar {
    Space = 1 << 1,
    AtMark = 1 << 2,
    Percent = 1 << 3,
    Slash = 1 << 4,
    BackSlash = 1 << 5,
    Numeric = 1 << 6,
    OutsideOfDownloadCode = 1 << 7,
    OutsideOfMiiNickName = 1 << 8,
    Force32 = 1 << 9
};

enum PasswordMode { Show, Hide, Max_PasswordMode };

enum InputFormMode { OneLine, MultiLine, Separate, Max_InputFormMode };

enum InitialCursorPos { First, Last, Max_InitialCursorPos };

enum TextCheckResult { Success, ShowFailureDialog, ShowConfirmDialog, Max_TextCheckResult };

enum DictionaryLang {
    Japanese,
    AmericanEnglish,
    CanadianFrench,
    LatinAmericanSpanish,
    Reserved1,
    BritishEnglish,
    French,
    German,
    Spanish,
    Italian,
    Dutch,
    Portuguese,
    Russian,
    Reserved2,
    SimplifiedChinesePinyin,
    TraditionalChineseCangjie,
    TraditionalChineseSimplifiedCangjie,
    TraditionalChineseZhuyin,
    Korean,
    Max_DictionaryLang
};

enum CloseResult { Enter, Cancel };

struct DictionaryInfo {
    u32 offset;           // 0x0
    u16 size;             // 0x4
    DictionaryLang lang;  // 0x6
};

struct KeyboardConfig {
    KeyboardMode keyboardMode;
    char okText[0x12];
    char16_t leftOptionalSymbolKey;
    char16_t rightOptionalSymbolKey;
    bool isPredictionEnabled;
    InvalidChar invalidCharFlag;
    InitialCursorPos initialCursorPos;
    char headerText[0x82];
    char subText[0x102];
    char guideText[0x202];
    int textMaxLength;
    int textMinLength;
    PasswordMode passwordMode;
    InputFormMode inputFormMode;
    bool isUseNewLine;
    bool isUseUtf8;
    bool isUseBlurBackground;
    int _initialStringOffset;
    int _initialStringLength;
    int _userDictionaryOffset;
    int _userDictionaryNum;
    bool _isUseTextCheck;
    void* _textCheckCallback;
    int separateTextPos[0x8];
};

struct ShowKeyboardArg {
    KeyboardConfig keyboardConfig;  // 0x0
    const char* workBuf;            // 0x400
    long workBufSize;               // 0x408
    const char* textCheckWorkBuf;   // 0x98
    long textCheckWorkBufSize;      // 0xA0
    const char* _customizeDicBuf;   // 0xA8
    long _customizeDicBufSize;      // 0xB0
};

class String {
public:
    String(int size) {
        bufsize = size;
        strBuf = nullptr;
    }
    String(int size, char* buf) {
        bufsize = size;
        strBuf = buf;
    }

    void allocate() { strBuf = (char*)malloc(bufsize); }

    void setBuf(char* buf) { strBuf = buf; }

    const char* cstr() { return strBuf; }

private:
    char* strBuf;
    int bufsize;
};

struct UserWord;

ulong GetRequiredWorkBufferSize(bool);
ulong GetRequiredStringBufferSize();
nn::applet::ExitReason getExitReason();
void MakePreset(KeyboardConfig*, Preset);
void SetHeaderText(KeyboardConfig*, const char_16t*);
void SetSubText(KeyboardConfig*, const char_16t*);
void SetOkText(KeyboardConfig*, const char_16t*);
void SetOkTextUtf8(KeyboardConfig*, const char*);
void SetLeftOptionalSymbolKey(KeyboardConfig*, char16_t);
void SetLeftOptionalSymbolKeyUtf8(KeyboardConfig*, const char*);
void SetRightOptionalSymbolKey(KeyboardConfig*, char16_t);
void SetRightOptionalSymbolKeyUtf8(KeyboardConfig*, const char*);
void SetHeaderText(KeyboardConfig*, const char_16t*);
void SetHeaderTextUtf8(KeyboardConfig*, const char*);
void SetSubText(KeyboardConfig*, const char_16t*);
void SetSubTextUtf8(KeyboardConfig*, const char*);
void SetGuideText(KeyboardConfig*, const char_16t*);
void SetGuideTextUtf8(KeyboardConfig*, const char*);
void SetInitialText(ShowKeyboardArg*, const char_16t*);
void SetInitialTextUtf8(ShowKeyboardArg*, const char*);
void SetUserWordList(ShowKeyboardArg*, const UserWord*, int);
int ShowKeyboard(String*, const ShowKeyboardArg&);

__attribute__((used)) static nn::applet::ExitReason g_ExitReason;

}  // namespace swkbd
}  // namespace nn
