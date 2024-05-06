#pragma once

#include <nn/fs/fs_types.h>

namespace nn::fs {
/*
    Open a directory given the path and open mode.
    handleOut: Pointer to write the handle into.
    path: Path to the directory to open.
    openMode: Mode to open the directory with, see nn::fs::OpenDirectoryMode.
*/
Result OpenDirectory(DirectoryHandle* handleOut, const char* path, s32 openMode);

/*
    Closes directory.
    handle: Handle of directory to close.
*/
void CloseDirectory(DirectoryHandle handle);

/*
    Read entries for a given opened directory into a provided buffer.
    entryCountOut: Pointer to write actual amount of entries read.
    entriesOut: Pointer to buffer containing (entryBufferLength) amount of DirectoryEntry
    handle: Handle of directory to be opened.
    entryBufferLength: How many entries provided in the entriesOut argument.
*/
Result ReadDirectory(s64* entryCountOut, DirectoryEntry* entriesOut, DirectoryHandle handle,
                     s64 entryBufferLength);

/*
    Creates a directory at  given path.
    path: Path to path to create.
*/
Result CreateDirectory(const char* path);

/*
    Get the amount of entries in a given opened directory.
    entryCountOut: Pointer to write the entry count.
    handle: Handle of the directory to count entries.
*/
Result GetDirectoryEntryCount(s64* entryCountOut, DirectoryHandle handle);

/*
    Delete a given directory path and all of it's subdirectories/folders.
    path: Path to the directory to delete.
*/
Result DeleteDirectoryRecursively(const char* path);
Result DeleteDirectory(const char* path);

/*
    path: Path to the directory to be cleaned.
*/
Result CleanDirectoryRecursively(const char* path);

Result RenameDirectory(const char* path, const char* newPath);

Result GetEntryType(DirectoryEntryType*, const char*);

}  // namespace nn::fs
