#pragma once

#include <cstdlib>
#include <nn/applet.h>
#include <nn/types.h>

namespace nn::swkbd {

enum class Preset {
    Default,
    Password,
    UserName,
    DownloadCode,
    Max_Preset,
};

enum class KeyboardMode {
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
    ModeFullLatin,
};

enum class InvalidChar {
    Space = 1 << 1,
    AtMark = 1 << 2,
    Percent = 1 << 3,
    Slash = 1 << 4,
    BackSlash = 1 << 5,
    Numeric = 1 << 6,
    OutsideOfDownloadCode = 1 << 7,
    OutsideOfMiiNickName = 1 << 8,
    Force32 = 1 << 9,
};

enum class PasswordMode {
    Show,
    Hide,
    Max_PasswordMode,
};

enum class InputFormMode {
    OneLine,
    MultiLine,
    Separate,
    Max_InputFormMode,
};

enum class InitialCursorPos {
    First,
    Last,
    Max_InitialCursorPos,
};

enum class TextCheckResult {
    Success,
    ShowFailureDialog,
    ShowConfirmDialog,
    Max_TextCheckResult,
};

enum class DictionaryLang {
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
    Max_DictionaryLang,
};

enum class CloseResult { Enter, Cancel };

enum class Trigger : u8 { Default };

struct DictionaryInfo {
    u32 offset;
    u16 size;
    DictionaryLang lang;
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
    int initialStringOffset;
    int initialStringLength;
    int userDictionaryOffset;
    int userDictionaryNum;
    bool isUseTextCheck;
    void* textCheckCallback;

#if NN_SDK_VER >= NN_MAKE_VER(3, 0, 0)
    int separateTextPos[0x8];
#endif

#if NN_SDK_VER >= NN_MAKE_VER(6, 0, 0)
    u64 customDictionaryEntries[0x18];
    u8 customDictionaryNum;
#endif

#if NN_SDK_VER >= NN_MAKE_VER(8, 0, 0)
    u8 _3c1;
    u8 filler[0xd];
    u8 trigger;
#endif
};

struct ShowKeyboardArg {
    KeyboardConfig keyboardConfig;
    const char* workBuf;
    long workBufSize;
    const char* textCheckWorkBuf;
    long textCheckWorkBufSize;
    const char* _customizeDicBuf;
    long _customizeDicBufSize;
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

struct UserWord;   // TODO contents missing
struct KbdConfig;  // TODO contents missing

struct CustomizedDictionarySet {
    void* buffer;    // 0x1000-byte aligned buffer.
    int bufferSize;  // 0x1000-byte aligned buffer size.
    u64 entries[0x18];
    u16 totalEntries;
};

typedef TextCheckResult (*TextCheckCb)(void*, ulong*, String*);

int ConvertUtf8ToUtf16(void*, int, const char*, int);
int ConvertUtf8ToUtf16(void*, int, const char*);
int GetLengthOfConvertedStringUtf8ToUtf16(const char*);
void MakePresetDefault(KeyboardConfig*);
void MakePresetPassword(KeyboardConfig*);
void MakePresetUsername(KeyboardConfig*);
void MakePresetDownloadCode(KeyboardConfig*);
Result GetInteractiveOutStorageCallback(nn::applet::LibraryAppletHandle, String*,
                                        const ShowKeyboardArg&);
ulong GetRequiredTextCheckWorkBufferSize();
void ReadCloseResultAndString(nn::applet::LibraryAppletHandle, CloseResult*, String*);
void CopyInitialStringInfo(ShowKeyboardArg*, int);
void CopyUserDictionaryInfo(ShowKeyboardArg*, int);

#if NN_SDK_VER >= NN_MAKE_VER(3, 0, 0)
ulong GetRequiredWorkBufferSize(int);
#endif

#if NN_SDK_VER >= NN_MAKE_VER(6, 0, 0)
void keyboardConfig2kbdConfig(const KeyboardConfig&, KbdConfig*);
#endif

Result ShowKeyboard(String*, const ShowKeyboardArg&);

#if NN_SDK_VER >= NN_MAKE_VER(8, 0, 0)
Result ShowKeyboard(String*, const ShowKeyboardArg&, Trigger);
#endif

void InitializeKeyboardConfig(KeyboardConfig*);
void MakePreset(KeyboardConfig*, Preset);

ulong GetRequiredWorkBufferSize(bool);

#if NN_SDK_VER >= NN_MAKE_VER(1, 0, 0) && NN_SDK_VER < NN_MAKE_VER(2, 0, 0)
ulong GetRequiredWorkBufferSize(int);
#endif

#if NN_SDK_VER >= NN_MAKE_VER(3, 0, 0)
ulong GetRequiredWorkBufferSize();
#endif

#if NN_SDK_VER < NN_MAKE_VER(2, 0, 0)
ulong GetRequiredStringBufferSize();
#endif

void SetOkText(KeyboardConfig*, const char16_t*);
void SetOkTextUtf8(KeyboardConfig*, const char*);
void SetLeftOptionalSymbolKey(KeyboardConfig*, char16_t);
void SetLeftOptionalSymbolKeyUtf8(KeyboardConfig*, const char*);
void SetRightOptionalSymbolKey(KeyboardConfig*, char16_t);
void SetRightOptionalSymbolKeyUtf8(KeyboardConfig*, const char*);
void SetHeaderText(KeyboardConfig*, const char16_t*);
void SetHeaderTextUtf8(KeyboardConfig*, const char*);
void SetSubText(KeyboardConfig*, const char16_t*);
void SetSubTextUtf8(KeyboardConfig*, const char*);
void SetGuideText(KeyboardConfig*, const char16_t*);
void SetGuideTextUtf8(KeyboardConfig*, const char*);
void SetInitialText(ShowKeyboardArg*, const char16_t*);
void SetInitialTextUtf8(ShowKeyboardArg*, const char*);
void SetUserWordList(ShowKeyboardArg*, const UserWord*, int);

#if NN_SDK_VER >= NN_MAKE_VER(6, 0, 0)
void SetCustomizedDictionaries(ShowKeyboardArg*, const CustomizedDictionarySet&);
#endif

void SetTextCheckCallback(ShowKeyboardArg*, TextCheckCb);

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
nn::applet::ExitReason GetExitReason();
#endif

extern nn::applet::ExitReason g_ExitReason;
}  // namespace nn::swkbd
