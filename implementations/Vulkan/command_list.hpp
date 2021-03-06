#ifndef AGPU_COMMAND_LIST_HPP
#define AGPU_COMMAND_LIST_HPP

#include "device.hpp"

namespace AgpuVulkan
{

class AVkCommandList : public agpu::command_list
{
public:
    AVkCommandList(const agpu::device_ref &device);
    ~AVkCommandList();

    static agpu::command_list_ref create(const agpu::device_ref &device, agpu_command_list_type type, const agpu::command_allocator_ref &allocator, const agpu::pipeline_state_ref &initial_pipeline_state);

    virtual agpu_error setShaderSignature(const agpu::shader_signature_ref &signature) override;
    virtual agpu_error setViewport(agpu_int x, agpu_int y, agpu_int w, agpu_int h) override;
    virtual agpu_error setScissor(agpu_int x, agpu_int y, agpu_int w, agpu_int h) override;
    virtual agpu_error usePipelineState(const agpu::pipeline_state_ref &pipeline) override;
    virtual agpu_error useVertexBinding(const agpu::vertex_binding_ref &vertex_binding) override;
    virtual agpu_error useIndexBuffer(const agpu::buffer_ref &index_buffer) override;
    virtual agpu_error useIndexBufferAt(const agpu::buffer_ref &index_buffer, agpu_size offset, agpu_size index_size) override;
    virtual agpu_error useDrawIndirectBuffer(const agpu::buffer_ref &draw_buffer) override;
    virtual agpu_error useComputeDispatchIndirectBuffer(const agpu::buffer_ref &dispatch_buffer) override;
    virtual agpu_error useShaderResources(const agpu::shader_resource_binding_ref &binding) override;
    virtual agpu_error useComputeShaderResources(const agpu::shader_resource_binding_ref &binding) override;
    virtual agpu_error pushConstants (agpu_uint offset, agpu_uint size, agpu_pointer values) override;
    virtual agpu_error drawArrays(agpu_uint vertex_count, agpu_uint instance_count, agpu_uint first_vertex, agpu_uint base_instance) override;
    virtual agpu_error drawArraysIndirect(agpu_size offset, agpu_size drawcount) override;
    virtual agpu_error drawElements(agpu_uint index_count, agpu_uint instance_count, agpu_uint first_index, agpu_int base_vertex, agpu_uint base_instance) override;
    virtual agpu_error drawElementsIndirect(agpu_size offset, agpu_size drawcount) override;
    virtual agpu_error dispatchCompute ( agpu_uint group_count_x, agpu_uint group_count_y, agpu_uint group_count_z ) override;
    virtual agpu_error dispatchComputeIndirect ( agpu_size offset ) override;
    virtual agpu_error setStencilReference(agpu_uint reference) override;
    virtual agpu_error executeBundle(const agpu::command_list_ref &bundle) override;

    virtual agpu_error close() override;
    virtual agpu_error reset(const agpu::command_allocator_ref &allocator, const agpu::pipeline_state_ref &initial_pipeline_state) override;
    virtual agpu_error resetBundle (const agpu::command_allocator_ref &allocator, const agpu::pipeline_state_ref &initial_pipeline_state, agpu_inheritance_info* inheritance_info ) override;

    virtual agpu_error beginRenderPass(const agpu::renderpass_ref &renderpass, const agpu::framebuffer_ref &framebuffer, agpu_bool secondaryContent) override;
    virtual agpu_error endRenderPass() override;
    virtual agpu_error resolveFramebuffer(const agpu::framebuffer_ref &destFramebuffer, const agpu::framebuffer_ref &sourceFramebuffer) override;
    virtual agpu_error resolveTexture (const agpu::texture_ref &sourceTexture, agpu_uint sourceLevel, agpu_uint sourceLayer, const agpu::texture_ref &destTexture, agpu_uint destLevel, agpu_uint destLayer, agpu_uint levelCount, agpu_uint layerCount, agpu_texture_aspect aspect ) override;

    virtual agpu_error memoryBarrier(agpu_pipeline_stage_flags source_stage, agpu_pipeline_stage_flags dest_stage, agpu_access_flags source_accesses, agpu_access_flags dest_accesses) override;

    agpu::device_ref device;
    agpu::command_allocator_ref allocator;
    agpu_command_list_type type;
    agpu_uint queueFamilyIndex;
    VkCommandBuffer commandBuffer;

private:
    void resetState();
    agpu_error setImageLayout(VkImage image, VkImageSubresourceRange range, VkImageAspectFlags aspect, VkImageLayout sourceLayout, VkImageLayout destLayout, VkAccessFlags srcAccessMask);

    agpu::framebuffer_ref currentFramebuffer;
    agpu_bool isClosed;
    agpu_bool isSecondaryContent;

    agpu::buffer_ref drawIndirectBuffer;
    agpu::buffer_ref computeDispatchIndirectBuffer;
    agpu::shader_signature_ref shaderSignature;
};

} // End of namespace AgpuVulkan

#endif //AGPU_COMMAND_LIST_HPP
