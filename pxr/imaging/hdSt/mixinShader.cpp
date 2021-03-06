//
// Copyright 2017 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
//

#include "pxr/imaging/hdSt/mixinShader.h"
#include "pxr/imaging/hd/tokens.h"

#include "pxr/base/arch/hash.h"

#include <boost/functional/hash.hpp>

PXR_NAMESPACE_OPEN_SCOPE

HdStMixinShader::HdStMixinShader(
    std::string mixinSource,
    HdStShaderCodeSharedPtr baseShader)
: HdStShaderCode()
, _mixinSource(mixinSource)
, _baseShader(baseShader)
{
}

HdStMixinShader::~HdStMixinShader() = default;

HdStShaderCode::ID
HdStMixinShader::ComputeHash() const 
{
    HdStShaderCode::ID hash = 0;
    boost::hash_combine(hash, ArchHash(
        _mixinSource.c_str(), 
        _mixinSource.size()));
    boost::hash_combine(hash, _baseShader->ComputeHash());
    return hash;
}

HdStShaderCode::ID
HdStMixinShader::ComputeTextureSourceHash() const
{
    return _baseShader->ComputeTextureSourceHash();
}

std::string
HdStMixinShader::GetSource(TfToken const &shaderStageKey) const 
{
    std::string baseSource = _baseShader->GetSource(shaderStageKey);
    if (shaderStageKey == HdShaderTokens->fragmentShader) {
        return _mixinSource + baseSource;
    }
    return baseSource;
}

HdSt_MaterialParamVector const&
HdStMixinShader::GetParams() const 
{
    return _baseShader->GetParams();
}

bool
HdStMixinShader:: IsEnabledPrimvarFiltering() const
{
    return _baseShader->IsEnabledPrimvarFiltering();
}

TfTokenVector const &
HdStMixinShader::GetPrimvarNames() const 
{
    return _baseShader->GetPrimvarNames();
}

HdBufferArrayRangeSharedPtr const&
HdStMixinShader::GetShaderData() const 
{
    return _baseShader->GetShaderData();
}

void
HdStMixinShader::BindResources(const int program,
                               HdSt_ResourceBinder const &binder,
                               HdRenderPassState const &state)
{
    _baseShader->BindResources(program, binder, state);
}

void
HdStMixinShader::UnbindResources(const int program,
                                 HdSt_ResourceBinder const &binder,
                                 HdRenderPassState const &state)
{
    _baseShader->UnbindResources(program, binder, state);
}

void
HdStMixinShader::AddBindings(HdBindingRequestVector *customBindings)
{
    _baseShader->AddBindings(customBindings);
}

TfToken
HdStMixinShader::GetMaterialTag() const
{
    return _baseShader->GetMaterialTag();
}

PXR_NAMESPACE_CLOSE_SCOPE


