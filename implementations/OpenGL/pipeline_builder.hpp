#ifndef AGPU_PIPELINE_BUILDER_HPP_
#define AGPU_PIPELINE_BUILDER_HPP_

#include "device.hpp"
#include "shader.hpp"
#include <utility>
#include <string>
#include <set>

void processTextureWithSamplerCombinations(const std::set<TextureWithSamplerCombination> &rawTextureSamplerCombinations, agpu_shader_signature *shaderSignature, TextureWithSamplerCombinationMap &map, std::vector<MappedTextureWithSamplerCombination> &usedCombinations);

struct _agpu_pipeline_builder: public Object<_agpu_pipeline_builder>
{
public:
    _agpu_pipeline_builder();

    static agpu_pipeline_builder *createBuilder(agpu_device *device);

    void lostReferences();

    agpu_pipeline_state* build ();

    agpu_error setShaderSignature(agpu_shader_signature* signature);

    agpu_error attachShader ( agpu_shader* shader );
    agpu_error attachShaderWithEntryPoint ( agpu_shader* shader, agpu_cstring entry_point );

    agpu_size getBuildingLogLength (  );
    agpu_error getBuildingLog ( agpu_size buffer_size, agpu_string_buffer buffer );

    agpu_error setBlendState(agpu_int renderTargetMask, agpu_bool enabled);
    agpu_error setBlendFunction(agpu_int renderTargetMask, agpu_blending_factor sourceFactor, agpu_blending_factor destFactor, agpu_blending_operation colorOperation, agpu_blending_factor sourceAlphaFactor, agpu_blending_factor destAlphaFactor, agpu_blending_operation alphaOperation);
    agpu_error setColorMask(agpu_int renderTargetMask, agpu_bool redEnabled, agpu_bool greenEnabled, agpu_bool blueEnabled, agpu_bool alphaEnabled);
    agpu_error setFrontFace ( agpu_face_winding winding );
    agpu_error setCullMode ( agpu_cull_mode mode );
    agpu_error setDepthState ( agpu_bool enabled, agpu_bool writeMask, agpu_compare_function function );
    agpu_error setStencilState ( agpu_bool enabled, agpu_int writeMask, agpu_int readMask );
    agpu_error setStencilFrontFace(agpu_stencil_operation stencilFailOperation, agpu_stencil_operation depthFailOperation, agpu_stencil_operation stencilDepthPassOperation, agpu_compare_function stencilFunction);
    agpu_error setStencilBackFace(agpu_stencil_operation stencilFailOperation, agpu_stencil_operation depthFailOperation, agpu_stencil_operation stencilDepthPassOperation, agpu_compare_function stencilFunction);
    agpu_error setRenderTargetCount ( agpu_int count );
    agpu_error setPrimitiveType(agpu_primitive_topology type);
    agpu_error setVertexLayout(agpu_vertex_layout* layout);

    agpu_error setRenderTargetFormat(agpu_uint index, agpu_texture_format format);
    agpu_error setDepthStencilFormat(agpu_texture_format format);
    agpu_error setSampleDescription(agpu_uint sample_count, agpu_uint sample_quality);

    agpu_error reset();

public:
    agpu_device *device;

    // States
    agpu_bool depthEnabled;
    agpu_bool depthWriteMask;
    agpu_compare_function depthFunction;

    // Face culling
    agpu_face_winding frontFaceWinding;
    agpu_cull_mode cullingMode;

    // Render targets
    std::vector<agpu_texture_format> renderTargetFormats;
    agpu_texture_format depthStencilFormat;

    // Color buffer
    agpu_bool blendingEnabled;
    agpu_bool redMask;
    agpu_bool greenMask;
    agpu_bool blueMask;
    agpu_bool alphaMask;
    agpu_blending_factor sourceBlendFactor;
    agpu_blending_factor destBlendFactor;
    agpu_blending_operation blendOperation;
    agpu_blending_factor sourceBlendFactorAlpha;
    agpu_blending_factor destBlendFactorAlpha;
    agpu_blending_operation blendOperationAlpha;

    // Stencil testing
    agpu_bool stencilEnabled;
    int stencilWriteMask;
    int stencilReadMask;

    agpu_stencil_operation stencilFrontFailOp;
    agpu_stencil_operation stencilFrontDepthFailOp;
    agpu_stencil_operation stencilFrontDepthPassOp;
    agpu_compare_function stencilFrontFunc;

    agpu_stencil_operation stencilBackFailOp;
    agpu_stencil_operation stencilBackDepthFailOp;
    agpu_stencil_operation stencilBackDepthPassOp;
    agpu_compare_function stencilBackFunc;

    // Multisampling
    agpu_uint sampleCount;
    agpu_uint sampleQuality;

    // Miscellaneos
    agpu_primitive_topology primitiveType;

    // Error messages
    std::string errorMessages;

    agpu_shader_signature *shaderSignature;
    std::vector<std::pair<agpu_shader*, std::string>> shaders;

private:
    void buildTextureWithSampleCombinationMapInto(TextureWithSamplerCombinationMap &map, std::vector<MappedTextureWithSamplerCombination> &usedCombinations);
};


#endif //AGPU_PIPELINE_BUILDER_HPP_
