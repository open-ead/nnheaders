#pragma once

namespace nn::atk::detail {
enum ElementType {
    ElementType_Category_Tables = 0x100,
    ElementType_Category_Parameters = 0x200,
    ElementType_Category_Codecs = 0x300,

    ElementType_Category_General = 0x1F00,

    ElementType_Category_SoundArchiveFile_Blocks = 0x2000,
    ElementType_Category_SoundArchiveFile_InfoSections = 0x2100,
    ElementType_Category_SoundArchiveFile_ItemInfos = 0x2200,
    ElementType_Category_SoundArchiveFile_Parameters = 0x2300,
    ElementType_Category_SoundArchiveFile_General = 0x2400,

    ElementType_Category_StreamSoundFile_Blocks = 0x4000,
    ElementType_Category_StreamSoundFile_ItemInfos = 0x4100,

    ElementType_Category_WaveSoundFile_Blocks = 0x4800,
    ElementType_Category_WaveSoundFile_ItemInfos = 0x4900,

    ElementType_Category_SequenceSoundFile_Blocks = 0x5000,
    ElementType_Category_SequenceSoundFile_ItemInfos = 0x5100,

    ElementType_Category_BankFile_Blocks = 0x5800,
    ElementType_Category_BankFile_Items = 0x5900,
    ElementType_Category_BankFile_ItemTables = 0x6000,

    ElementType_Category_WaveArchiveFile_Blocks = 0x6800,

    ElementType_Category_WaveFile_Blocks = 0x7000,
    ElementType_Category_WaveFile_ItemInfos = 0x7100,

    ElementType_Category_GroupFile_Blocks = 0x7800,
    ElementType_Category_GroupFile_ItemInfos = 0x7900,

    ElementType_Category_AnimSoundFile_Blocks = 0x8000,
    ElementType_Category_AnimSoundFile_Items = 0x8100,

    ElementType_Table_EmbeddingTable = ElementType_Category_Tables,
    ElementType_Table_ReferenceTable,
    ElementType_Table_ReferenceWithSizeTable,

    ElementType_Parameter_Sound3d = ElementType_Category_Parameters,
    ElementType_Parameter_Sends,
    ElementType_Parameter_Envelope,
    ElementType_Parameter_AdshrEnvelope,

    ElementType_Codec_DspAdpcmInfo = ElementType_Category_Codecs,
    ElementType_Codec_ImaAdpcmInfo,

    ElementType_General_ByteStream = ElementType_Category_General,
    ElementType_General_String,

    ElementType_SoundArchiveFile_StringBlock = ElementType_Category_SoundArchiveFile_Blocks,
    ElementType_SoundArchiveFile_InfoBlock,
    ElementType_SoundArchiveFile_FileBlock,

    ElementType_SoundArchiveFile_SoundInfoSection = ElementType_Category_SoundArchiveFile_InfoSections,
    ElementType_SoundArchiveFile_BankInfoSection,
    ElementType_SoundArchiveFile_PlayerInfoSection,
    ElementType_SoundArchiveFile_WaveArchiveInfoSection,
    ElementType_SoundArchiveFile_SoundGroupInfoSection,
    ElementType_SoundArchiveFile_GroupInfoSection,
    ElementType_SoundArchiveFile_FileInfoSection,

    ElementType_SoundArchiveFile_SoundInfo = ElementType_Category_SoundArchiveFile_ItemInfos,
    ElementType_SoundArchiveFile_StreamSoundInfo,
    ElementType_SoundArchiveFile_WaveSoundInfo,
    ElementType_SoundArchiveFile_SequenceSoundInfo,
    ElementType_SoundArchiveFile_SoundGroupInfo,
    ElementType_SoundArchiveFile_WaveSoundGroupInfo,
    ElementType_SoundArchiveFile_BankInfo,
    ElementType_SoundArchiveFile_WaveArchiveInfo,
    ElementType_SoundArchiveFile_GroupInfo,
    ElementType_SoundArchiveFile_PlayerInfo,
    ElementType_SoundArchiveFile_FileInfo,
    ElementType_SoundArchiveFile_SoundArchivePlayerInfo,
    ElementType_SoundArchiveFile_InternalFileInfo,
    ElementType_SoundArchiveFile_ExternalFileInfo,
    ElementType_SoundArchiveFile_StreamSoundTrackInfo,
    ElementType_SoundArchiveFile_SendInfo,
    ElementType_SoundArchiveFile_StreamSoundExtensionInfo,
    
    ElementType_SoundArchiveFile_StringTable = ElementType_Category_SoundArchiveFile_General,
    ElementType_SoundArchiveFile_PatriciaTree,

    ElementType_StreamSoundFile_InfoBlock = ElementType_Category_StreamSoundFile_Blocks,
    ElementType_StreamSoundFile_SeekBlock,
    ElementType_StreamSoundFile_DataBlock,
    ElementType_StreamSoundFile_RegionBlock,
    ElementType_StreamSoundFile_PrefetchDataBlock,

    ElementType_StreamSoundFile_StreamSoundInfo = ElementType_Category_StreamSoundFile_ItemInfos,
    ElementType_StreamSoundFile_TrackInfo,
    ElementType_StreamSoundFile_ChannelInfo,

    ElementType_WaveSoundFile_WaveSoundMetaData = ElementType_Category_WaveSoundFile_ItemInfos,
    ElementType_WaveSoundFile_WaveSoundInfo,
    ElementType_WaveSoundFile_NoteInfo,
    ElementType_WaveSoundFile_TrackInfo,
    ElementType_WaveSoundFile_NoteEvent,

    ElementType_SequenceSoundFile_DataBlock = ElementType_Category_SequenceSoundFile_Blocks,
    ElementType_SequenceSoundFile_LabelBlock,

    ElementType_SequenceSoundFile_LabelInfo = ElementType_Category_SequenceSoundFile_ItemInfos,

    ElementType_BankFile_InfoBlock = ElementType_Category_BankFile_Blocks,

    ElementType_BankFile_InstrumentInfo = ElementType_Category_BankFile_Items,
    ElementType_BankFile_KeyRegionInfo,
    ElementType_BankFile_VelocityRegionInfo,
    ElementType_BankFile_NullInfo,

    ElementType_BankFile_DirectReferenceTable = ElementType_Category_BankFile_ItemTables,
    ElementType_BankFile_RangeReferenceTable,
    ElementType_BankFile_IndexReferenceTable,

    ElementType_WaveArchiveFile_InfoBlock = ElementType_Category_WaveArchiveFile_Blocks,
    ElementType_WaveArchiveFile_FileBlock,

    ElementType_WaveSoundFile_InfoBlock = ElementType_Category_WaveArchiveFile_Blocks,

    ElementType_WaveFile_InfoBlock = ElementType_Category_WaveFile_Blocks,
    ElementType_WaveFile_DataBlock,

    ElementType_WaveFile_ChannelInfo = ElementType_Category_WaveFile_ItemInfos,

    ElementType_GroupFile_InfoBlock = ElementType_Category_GroupFile_Blocks,
    ElementType_GroupFile_FileBlock,
    ElementType_GroupFile_InfoExBlock,

    ElementType_GroupFile_GroupItemInfo = ElementType_Category_GroupFile_ItemInfos,
    ElementType_GroupFile_GroupItemInfoEx,

    ElementType_AnimSoundFile_DataBlock = ElementType_Category_AnimSoundFile_Blocks,

    ElementType_AnimSoundFile_EventInfo = ElementType_Category_AnimSoundFile_Items
};
} // namespace nn::atk::detail