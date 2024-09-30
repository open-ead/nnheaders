#pragma once

#include <nn/types.h>

namespace nn {
typedef u64 ApplicationId;

namespace fs {
using namespace ams::fs;  // for errors

typedef u64 UserId;

/* Handle representing an opened file. */
struct FileHandle {
    u64 _internal;
};

/* Handle representing an opened directory. */
struct DirectoryHandle {
    u64 _internal;
};

/* Kinds of entries within a directory. */
enum DirectoryEntryType {
    DirectoryEntryType_Directory,
    DirectoryEntryType_File,
};

/* Bitfield to define the kinds of entries to open from a directory. */
enum OpenDirectoryMode {
    OpenDirectoryMode_Directory = 1 << 0,
    OpenDirectoryMode_File = 1 << 1,
    OpenDirectoryMode_All = OpenDirectoryMode_Directory | OpenDirectoryMode_File,
};

/* Maximum length a directory name can be. */
constexpr s32 PathLengthMax = 0x300;

/* Information about an entry within a directory. */
struct DirectoryEntry {
    char mName[PathLengthMax + 1];
    char _x302[3];
    union {
        DirectoryEntryType mType;
        u8 mTypeByte;
    };
    s64 mFileSize;
};

/* Mode for opening files. */
enum OpenMode {
    OpenMode_Read = 1 << 0,
    OpenMode_Write = 1 << 1,
    OpenMode_Append = 1 << 2,

    OpenMode_ReadWrite = OpenMode_Read | OpenMode_Write
};

/* Options for reading. */
struct ReadOption {
    u32 value;

    static inline ReadOption MakeOption(u32 value) { return {value}; }
    static const ReadOption None;
};
inline constexpr const ReadOption ReadOption::None = {0};

enum WriteOptionFlag {
    WriteOptionFlag_Flush = 1 << 0,
};

/* Options for writing. */
struct WriteOption {
    int flags;
    static WriteOption CreateOption(int flags) {
        WriteOption op;
        op.flags = flags;
        return op;
    }
};
}  // namespace fs
}  // namespace nn
