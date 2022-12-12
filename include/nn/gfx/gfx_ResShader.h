#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_ResShaderData.h>
#include <nn/util/AccessorBase.h>
#include <nn/util/util_BinaryFormat.h>

namespace nn::gfx {

class ShaderInfo;

class ResShaderProgram : public nn::util::AccessorBase<ResShaderProgramData> {
    NN_NO_COPY(ResShaderProgram);

public:
    static ResShaderProgram* ToAccessor(value_type*);
    static ResShaderProgram& ToAccessor(value_type&);
    static const ResShaderProgram* ToAccessor(const value_type*);
    static const ResShaderProgram& ToAccessor(const value_type&);
    ShaderInfo* GetShaderInfo();
    const ShaderInfo* GetShaderInfo() const;
    detail::Caster<void> GetShader();
    detail::Caster<const void> GetShader() const;
    const nngfxToolShaderCompilerShaderReflection* GetShaderCompilerReflection() const;
};

class ResShaderVariation : public nn::util::AccessorBase<ResShaderVariationData> {
    NN_NO_COPY(ResShaderVariation);

public:
    static ResShaderVariation* ToAccessor(value_type*);
    static ResShaderVariation& ToAccessor(value_type&);
    static const ResShaderVariation* ToAccessor(const value_type*);
    static const ResShaderVariation& ToAccessor(const value_type&);
    ResShaderProgram* GetResShaderProgram(ShaderCodeType);
    const ResShaderProgram* GetResShaderProgram(ShaderCodeType) const;
};

class ResShaderContainer : public nn::util::AccessorBase<ResShaderContainerData> {
    NN_NO_COPY(ResShaderContainer);

public:
    static const int Signature = 0x63737267;  // ??

    static ResShaderContainer* ToAccessor(value_type*);
    static ResShaderContainer& ToAccessor(value_type&);
    static const ResShaderContainer* ToAccessor(const value_type*);
    static const ResShaderContainer& ToAccessor(const value_type&);
    ResShaderProgram* GetResShaderProgram(int);
    const ResShaderProgram* GetResShaderProgram(int) const;
    ResShaderVariation* GetResShaderVariation(int);
    const ResShaderVariation* GetResShaderVariation(int) const;
    int GetShaderVariationCount() const;
};

class ResShaderFile : public nn::util::AccessorBase<ResShaderFileData> {
    NN_NO_COPY(ResShaderFile);

public:
    static const int64_t Signature = 0x48534E42;  // ??
    static const int MajorVersion = 2;
    static const int MinorVersion = 1;
    static const int MicroVersion = 5;

    static size_t GetMaxFileAlignment();

    static ResShaderFile* ToAccessor(value_type* pData) {
        return static_cast<ResShaderFile*>(pData);
    }

    static ResShaderFile& ToAccessor(value_type& data) { return static_cast<ResShaderFile&>(data); }

    static const ResShaderFile* ToAccessor(const value_type*);
    static const ResShaderFile& ToAccessor(const value_type&);
    static bool IsValid(const void*);
    static ResShaderFile* ResCast(void*);

    ResShaderContainer* GetShaderContainer();
    const ResShaderContainer* GetShaderContainer() const {
        return ResShaderContainer::ToAccessor(
            reinterpret_cast<const ResShaderContainerData*>(fileHeader.GetFirstBlock()));
    }

    nn::util::BinaryFileHeader* GetBinaryFileHeader() { return &fileHeader; }
    const nn::util::BinaryFileHeader* GetBinaryFileHeader() const { return &fileHeader; }
};
}  // namespace nn::gfx