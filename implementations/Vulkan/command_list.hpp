#ifndef AGPU_COMMAND_LIST_HPP
#define AGPU_COMMAND_LIST_HPP

#include "device.hpp"

struct _agpu_command_list : public Object<_agpu_command_list>
{
public:
    _agpu_command_list(agpu_device *device);
    void lostReferences();

    static _agpu_command_list* create(agpu_device* device, agpu_command_list_type type, agpu_command_allocator* allocator, agpu_pipeline_state* initial_pipeline_state);

    agpu_error setShaderSignature(agpu_shader_signature* signature);
    agpu_error setViewport(agpu_int x, agpu_int y, agpu_int w, agpu_int h);
    agpu_error setScissor(agpu_int x, agpu_int y, agpu_int w, agpu_int h);
    agpu_error usePipelineState(agpu_pipeline_state* pipeline);
    agpu_error useVertexBinding(agpu_vertex_binding* vertex_binding);
    agpu_error useIndexBuffer(agpu_buffer* index_buffer);
    agpu_error setPrimitiveTopology(agpu_primitive_topology topology);
    agpu_error useDrawIndirectBuffer(agpu_buffer* draw_buffer);
    agpu_error useComputeDispatchIndirectBuffer(agpu_buffer* draw_buffer);
    agpu_error useShaderResources(agpu_shader_resource_binding* binding);
    agpu_error useComputeShaderResources(agpu_shader_resource_binding* binding);
    agpu_error pushConstants (agpu_uint offset, agpu_uint size, agpu_pointer values);
    agpu_error drawArrays(agpu_uint vertex_count, agpu_uint instance_count, agpu_uint first_vertex, agpu_uint base_instance);
    agpu_error drawArraysIndirect(agpu_size offset, agpu_size drawcount);
    agpu_error drawElements(agpu_uint index_count, agpu_uint instance_count, agpu_uint first_index, agpu_int base_vertex, agpu_uint base_instance);
    agpu_error drawElementsIndirect(agpu_size offset, agpu_size drawcount);
    agpu_error dispatchCompute ( agpu_uint group_count_x, agpu_uint group_count_y, agpu_uint group_count_z );
    agpu_error dispatchComputeIndirect ( agpu_size offset );
    agpu_error setStencilReference(agpu_uint reference);
    agpu_error executeBundle(agpu_command_list* bundle);

    agpu_error close();
    agpu_error reset(agpu_command_allocator* allocator, agpu_pipeline_state* initial_pipeline_state);
    agpu_error resetBundle (agpu_command_allocator* allocator, agpu_pipeline_state* initial_pipeline_state, agpu_inheritance_info* inheritance_info );

    agpu_error beginRenderPass(agpu_renderpass *renderpass, agpu_framebuffer* framebuffer, agpu_bool secondaryContent);
    agpu_error endRenderPass();
    agpu_error resolveFramebuffer(agpu_framebuffer* destFramebuffer, agpu_framebuffer* sourceFramebuffer);
    agpu_error resolveTexture ( agpu_texture* sourceTexture, agpu_uint sourceLevel, agpu_uint sourceLayer, agpu_texture* destTexture, agpu_uint destLevel, agpu_uint destLayer, agpu_uint levelCount, agpu_uint layerCount, agpu_texture_aspect aspect );
    
    agpu_device *device;
    agpu_command_allocator* allocator;
    agpu_command_list_type type;
    agpu_uint queueFamilyIndex;
    VkCommandBuffer commandBuffer;

private:
    void resetState();
    agpu_error setImageLayout(VkImage image, VkImageSubresourceRange range, VkImageAspectFlags aspect, VkImageLayout sourceLayout, VkImageLayout destLayout, VkAccessFlags srcAccessMask);

    agpu_framebuffer *currentFramebuffer;
    agpu_bool isClosed;
    agpu_bool isSecondaryContent;

    agpu_buffer *drawIndirectBuffer;
    agpu_buffer *computeDispatchIndirectBuffer;
    agpu_shader_signature *shaderSignature;
};

#endif //AGPU_COMMAND_LIST_HPP
