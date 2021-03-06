
#ifndef AGPU_HPP_
#define AGPU_HPP_

#include <stdexcept>
#include "agpu.h"

/**
 * Abstract GPU exception.
 */
class agpu_exception : public std::runtime_error
{
public:
    explicit agpu_exception(agpu_error error)
        : std::runtime_error("Abstract GPU Error"), errorCode(error)
    {
    }

    agpu_error getErrorCode() const
    {
        return errorCode;
    }

private:
    agpu_error errorCode;
};

/**
 * Abstract GPU reference smart pointer.
 */
template<typename T>
class agpu_ref
{
public:
    agpu_ref()
        : pointer(0)
    {
    }

    agpu_ref(const agpu_ref<T> &other)
    {
        if(other.pointer)
            other.pointer->addReference();
        pointer = other.pointer;
    }

    agpu_ref(T* pointer)
        : pointer(0)
    {
		reset(pointer);
    }

    ~agpu_ref()
    {
        if (pointer)
            pointer->release();
    }

    agpu_ref<T> &operator=(T *newPointer)
    {
        if (pointer)
            pointer->release();
        pointer = newPointer;
        return *this;
    }

    agpu_ref<T> &operator=(const agpu_ref<T> &other)
    {
        if(pointer != other.pointer)
        {
            if(other.pointer)
                other.pointer->addReference();
            if(pointer)
                pointer->release();
            pointer = other.pointer;
        }
        return *this;
    }

	void reset(T *newPointer = nullptr)
	{
		if(pointer)
			pointer->release();
		pointer = newPointer;
	}

    operator bool() const
    {
        return pointer;
    }

    bool operator!() const
    {
        return !pointer;
    }

    T* get() const
    {
        return pointer;
    }

    T *operator->() const
    {
        return pointer;
    }

private:
    T *pointer;
};

/**
 * Helper function to convert an error code into an exception.
 */
inline void agpuThrowIfFailed(agpu_error error)
{
    if(error < 0)
        throw agpu_exception(error);
}

// Interface wrapper for agpu_platform.
struct _agpu_platform
{
private:
	_agpu_platform() {}

public:
	inline agpu_ref<agpu_device> openDevice(agpu_device_open_info* openInfo)
	{
		return agpuOpenDevice(this, openInfo);
	}

	inline agpu_cstring getName()
	{
		return agpuGetPlatformName(this);
	}

	inline agpu_int getVersion()
	{
		return agpuGetPlatformVersion(this);
	}

	inline agpu_int getImplementationVersion()
	{
		return agpuGetPlatformImplementationVersion(this);
	}

	inline agpu_bool hasRealMultithreading()
	{
		return agpuPlatformHasRealMultithreading(this);
	}

	inline agpu_bool isNative()
	{
		return agpuIsNativePlatform(this);
	}

	inline agpu_bool isCrossPlatform()
	{
		return agpuIsCrossPlatform(this);
	}

	inline agpu_ref<agpu_offline_shader_compiler> createOfflineShaderCompiler()
	{
		return agpuCreateOfflineShaderCompiler(this);
	}

};

typedef agpu_ref<agpu_platform> agpu_platform_ref;

// Interface wrapper for agpu_device.
struct _agpu_device
{
private:
	_agpu_device() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddDeviceReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseDevice(this));
	}

	inline agpu_ref<agpu_command_queue> getDefaultCommandQueue()
	{
		return agpuGetDefaultCommandQueue(this);
	}

	inline agpu_ref<agpu_swap_chain> createSwapChain(const agpu_ref<agpu_command_queue>& commandQueue, agpu_swap_chain_create_info* swapChainInfo)
	{
		return agpuCreateSwapChain(this, commandQueue.get(), swapChainInfo);
	}

	inline agpu_ref<agpu_buffer> createBuffer(agpu_buffer_description* description, agpu_pointer initial_data)
	{
		return agpuCreateBuffer(this, description, initial_data);
	}

	inline agpu_ref<agpu_vertex_layout> createVertexLayout()
	{
		return agpuCreateVertexLayout(this);
	}

	inline agpu_ref<agpu_vertex_binding> createVertexBinding(const agpu_ref<agpu_vertex_layout>& layout)
	{
		return agpuCreateVertexBinding(this, layout.get());
	}

	inline agpu_ref<agpu_shader> createShader(agpu_shader_type type)
	{
		return agpuCreateShader(this, type);
	}

	inline agpu_ref<agpu_shader_signature_builder> createShaderSignatureBuilder()
	{
		return agpuCreateShaderSignatureBuilder(this);
	}

	inline agpu_ref<agpu_pipeline_builder> createPipelineBuilder()
	{
		return agpuCreatePipelineBuilder(this);
	}

	inline agpu_ref<agpu_compute_pipeline_builder> createComputePipelineBuilder()
	{
		return agpuCreateComputePipelineBuilder(this);
	}

	inline agpu_ref<agpu_command_allocator> createCommandAllocator(agpu_command_list_type type, const agpu_ref<agpu_command_queue>& queue)
	{
		return agpuCreateCommandAllocator(this, type, queue.get());
	}

	inline agpu_ref<agpu_command_list> createCommandList(agpu_command_list_type type, const agpu_ref<agpu_command_allocator>& allocator, const agpu_ref<agpu_pipeline_state>& initial_pipeline_state)
	{
		return agpuCreateCommandList(this, type, allocator.get(), initial_pipeline_state.get());
	}

	inline agpu_shader_language getPreferredShaderLanguage()
	{
		return agpuGetPreferredShaderLanguage(this);
	}

	inline agpu_shader_language getPreferredIntermediateShaderLanguage()
	{
		return agpuGetPreferredIntermediateShaderLanguage(this);
	}

	inline agpu_shader_language getPreferredHighLevelShaderLanguage()
	{
		return agpuGetPreferredHighLevelShaderLanguage(this);
	}

	inline agpu_ref<agpu_framebuffer> createFrameBuffer(agpu_uint width, agpu_uint height, agpu_uint colorCount, agpu_texture_view_description* colorViews, agpu_texture_view_description* depthStencilView)
	{
		return agpuCreateFrameBuffer(this, width, height, colorCount, colorViews, depthStencilView);
	}

	inline agpu_ref<agpu_renderpass> createRenderPass(agpu_renderpass_description* description)
	{
		return agpuCreateRenderPass(this, description);
	}

	inline agpu_ref<agpu_texture> createTexture(agpu_texture_description* description)
	{
		return agpuCreateTexture(this, description);
	}

	inline agpu_ref<agpu_fence> createFence()
	{
		return agpuCreateFence(this);
	}

	inline agpu_int getMultiSampleQualityLevels(agpu_uint sample_count)
	{
		return agpuGetMultiSampleQualityLevels(this, sample_count);
	}

	inline agpu_bool hasTopLeftNdcOrigin()
	{
		return agpuHasTopLeftNdcOrigin(this);
	}

	inline agpu_bool hasBottomLeftTextureCoordinates()
	{
		return agpuHasBottomLeftTextureCoordinates(this);
	}

	inline agpu_bool isFeatureSupported(agpu_feature feature)
	{
		return agpuIsFeatureSupportedOnDevice(this, feature);
	}

	inline agpu_ref<agpu_vr_system> getVRSystem()
	{
		return agpuGetVRSystem(this);
	}

	inline agpu_ref<agpu_offline_shader_compiler> createOfflineShaderCompiler()
	{
		return agpuCreateOfflineShaderCompilerForDevice(this);
	}

	inline agpu_ref<agpu_state_tracker_cache> createStateTrackerCache(const agpu_ref<agpu_command_queue>& command_queue_family)
	{
		return agpuCreateStateTrackerCache(this, command_queue_family.get());
	}

};

typedef agpu_ref<agpu_device> agpu_device_ref;

// Interface wrapper for agpu_vr_system.
struct _agpu_vr_system
{
private:
	_agpu_vr_system() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddVRSystemReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseVRSystem(this));
	}

	inline agpu_cstring getVRSystemName()
	{
		return agpuGetVRSystemName(this);
	}

	inline agpu_pointer getNativeHandle()
	{
		return agpuGetVRSystemNativeHandle(this);
	}

	inline void getRecommendedRenderTargetSize(agpu_size2d* size)
	{
		agpuThrowIfFailed(agpuGetVRRecommendedRenderTargetSize(this, size));
	}

	inline void getEyeToHeadTransform(agpu_vr_eye eye, agpu_matrix4x4f* transform)
	{
		agpuThrowIfFailed(agpuGetVREyeToHeadTransformInto(this, eye, transform));
	}

	inline void getProjectionMatrix(agpu_vr_eye eye, agpu_float near_distance, agpu_float far_distance, agpu_matrix4x4f* projection_matrix)
	{
		agpuThrowIfFailed(agpuGetVRProjectionMatrix(this, eye, near_distance, far_distance, projection_matrix));
	}

	inline void getProjectionFrustumTangents(agpu_vr_eye eye, agpu_frustum_tangents* frustum)
	{
		agpuThrowIfFailed(agpuGetVRProjectionFrustumTangents(this, eye, frustum));
	}

	inline void submitEyeRenderTargets(const agpu_ref<agpu_texture>& left_eye, const agpu_ref<agpu_texture>& right_eye)
	{
		agpuThrowIfFailed(agpuSubmitVREyeRenderTargets(this, left_eye.get(), right_eye.get()));
	}

	inline void waitAndFetchPoses()
	{
		agpuThrowIfFailed(agpuWaitAndFetchVRPoses(this));
	}

	inline agpu_size getValidTrackedDevicePoseCount()
	{
		return agpuGetValidVRTrackedDevicePoseCount(this);
	}

	inline void getValidTrackedDevicePoseInto(agpu_size index, agpu_vr_tracked_device_pose* dest)
	{
		agpuThrowIfFailed(agpuGetValidVRTrackedDevicePoseInto(this, index, dest));
	}

	inline agpu_size getValidRenderTrackedDevicePoseCount()
	{
		return agpuGetValidVRRenderTrackedDevicePoseCount(this);
	}

	inline void getValidRenderTrackedDevicePoseInto(agpu_size index, agpu_vr_tracked_device_pose* dest)
	{
		agpuThrowIfFailed(agpuGetValidVRRenderTrackedDevicePoseInto(this, index, dest));
	}

	inline agpu_bool pollEvent(agpu_vr_event* event)
	{
		return agpuPollVREvent(this, event);
	}

};

typedef agpu_ref<agpu_vr_system> agpu_vr_system_ref;

// Interface wrapper for agpu_swap_chain.
struct _agpu_swap_chain
{
private:
	_agpu_swap_chain() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddSwapChainReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseSwapChain(this));
	}

	inline void swapBuffers()
	{
		agpuThrowIfFailed(agpuSwapBuffers(this));
	}

	inline agpu_ref<agpu_framebuffer> getCurrentBackBuffer()
	{
		return agpuGetCurrentBackBuffer(this);
	}

	inline agpu_size getCurrentBackBufferIndex()
	{
		return agpuGetCurrentBackBufferIndex(this);
	}

	inline agpu_size getFramebufferCount()
	{
		return agpuGetFramebufferCount(this);
	}

};

typedef agpu_ref<agpu_swap_chain> agpu_swap_chain_ref;

// Interface wrapper for agpu_compute_pipeline_builder.
struct _agpu_compute_pipeline_builder
{
private:
	_agpu_compute_pipeline_builder() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddComputePipelineBuilderReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseComputePipelineBuilder(this));
	}

	inline agpu_ref<agpu_pipeline_state> build()
	{
		return agpuBuildComputePipelineState(this);
	}

	inline void attachShader(const agpu_ref<agpu_shader>& shader)
	{
		agpuThrowIfFailed(agpuAttachComputeShader(this, shader.get()));
	}

	inline void attachShaderWithEntryPoint(const agpu_ref<agpu_shader>& shader, agpu_shader_type type, agpu_cstring entry_point)
	{
		agpuThrowIfFailed(agpuAttachComputeShaderWithEntryPoint(this, shader.get(), type, entry_point));
	}

	inline agpu_size getBuildingLogLength()
	{
		return agpuGetComputePipelineBuildingLogLength(this);
	}

	inline void getBuildingLog(agpu_size buffer_size, agpu_string_buffer buffer)
	{
		agpuThrowIfFailed(agpuGetComputePipelineBuildingLog(this, buffer_size, buffer));
	}

	inline void setShaderSignature(const agpu_ref<agpu_shader_signature>& signature)
	{
		agpuThrowIfFailed(agpuSetComputePipelineShaderSignature(this, signature.get()));
	}

};

typedef agpu_ref<agpu_compute_pipeline_builder> agpu_compute_pipeline_builder_ref;

// Interface wrapper for agpu_pipeline_builder.
struct _agpu_pipeline_builder
{
private:
	_agpu_pipeline_builder() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddPipelineBuilderReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleasePipelineBuilder(this));
	}

	inline agpu_ref<agpu_pipeline_state> build()
	{
		return agpuBuildPipelineState(this);
	}

	inline void attachShader(const agpu_ref<agpu_shader>& shader)
	{
		agpuThrowIfFailed(agpuAttachShader(this, shader.get()));
	}

	inline void attachShaderWithEntryPoint(const agpu_ref<agpu_shader>& shader, agpu_shader_type type, agpu_cstring entry_point)
	{
		agpuThrowIfFailed(agpuAttachShaderWithEntryPoint(this, shader.get(), type, entry_point));
	}

	inline agpu_size getBuildingLogLength()
	{
		return agpuGetPipelineBuildingLogLength(this);
	}

	inline void getBuildingLog(agpu_size buffer_size, agpu_string_buffer buffer)
	{
		agpuThrowIfFailed(agpuGetPipelineBuildingLog(this, buffer_size, buffer));
	}

	inline void setBlendState(agpu_int renderTargetMask, agpu_bool enabled)
	{
		agpuThrowIfFailed(agpuSetBlendState(this, renderTargetMask, enabled));
	}

	inline void setBlendFunction(agpu_int renderTargetMask, agpu_blending_factor sourceFactor, agpu_blending_factor destFactor, agpu_blending_operation colorOperation, agpu_blending_factor sourceAlphaFactor, agpu_blending_factor destAlphaFactor, agpu_blending_operation alphaOperation)
	{
		agpuThrowIfFailed(agpuSetBlendFunction(this, renderTargetMask, sourceFactor, destFactor, colorOperation, sourceAlphaFactor, destAlphaFactor, alphaOperation));
	}

	inline void setColorMask(agpu_int renderTargetMask, agpu_bool redEnabled, agpu_bool greenEnabled, agpu_bool blueEnabled, agpu_bool alphaEnabled)
	{
		agpuThrowIfFailed(agpuSetColorMask(this, renderTargetMask, redEnabled, greenEnabled, blueEnabled, alphaEnabled));
	}

	inline void setFrontFace(agpu_face_winding winding)
	{
		agpuThrowIfFailed(agpuSetFrontFace(this, winding));
	}

	inline void setCullMode(agpu_cull_mode mode)
	{
		agpuThrowIfFailed(agpuSetCullMode(this, mode));
	}

	inline void setDepthBias(agpu_float constant_factor, agpu_float clamp, agpu_float slope_factor)
	{
		agpuThrowIfFailed(agpuSetDepthBias(this, constant_factor, clamp, slope_factor));
	}

	inline void setDepthState(agpu_bool enabled, agpu_bool writeMask, agpu_compare_function function)
	{
		agpuThrowIfFailed(agpuSetDepthState(this, enabled, writeMask, function));
	}

	inline void setPolygonMode(agpu_polygon_mode mode)
	{
		agpuThrowIfFailed(agpuSetPolygonMode(this, mode));
	}

	inline void setStencilState(agpu_bool enabled, agpu_int writeMask, agpu_int readMask)
	{
		agpuThrowIfFailed(agpuSetStencilState(this, enabled, writeMask, readMask));
	}

	inline void setStencilFrontFace(agpu_stencil_operation stencilFailOperation, agpu_stencil_operation depthFailOperation, agpu_stencil_operation stencilDepthPassOperation, agpu_compare_function stencilFunction)
	{
		agpuThrowIfFailed(agpuSetStencilFrontFace(this, stencilFailOperation, depthFailOperation, stencilDepthPassOperation, stencilFunction));
	}

	inline void setStencilBackFace(agpu_stencil_operation stencilFailOperation, agpu_stencil_operation depthFailOperation, agpu_stencil_operation stencilDepthPassOperation, agpu_compare_function stencilFunction)
	{
		agpuThrowIfFailed(agpuSetStencilBackFace(this, stencilFailOperation, depthFailOperation, stencilDepthPassOperation, stencilFunction));
	}

	inline void setRenderTargetCount(agpu_int count)
	{
		agpuThrowIfFailed(agpuSetRenderTargetCount(this, count));
	}

	inline void setRenderTargetFormat(agpu_uint index, agpu_texture_format format)
	{
		agpuThrowIfFailed(agpuSetRenderTargetFormat(this, index, format));
	}

	inline void setDepthStencilFormat(agpu_texture_format format)
	{
		agpuThrowIfFailed(agpuSetDepthStencilFormat(this, format));
	}

	inline void setPrimitiveType(agpu_primitive_topology type)
	{
		agpuThrowIfFailed(agpuSetPrimitiveType(this, type));
	}

	inline void setVertexLayout(const agpu_ref<agpu_vertex_layout>& layout)
	{
		agpuThrowIfFailed(agpuSetVertexLayout(this, layout.get()));
	}

	inline void setShaderSignature(const agpu_ref<agpu_shader_signature>& signature)
	{
		agpuThrowIfFailed(agpuSetPipelineShaderSignature(this, signature.get()));
	}

	inline void setSampleDescription(agpu_uint sample_count, agpu_uint sample_quality)
	{
		agpuThrowIfFailed(agpuSetSampleDescription(this, sample_count, sample_quality));
	}

};

typedef agpu_ref<agpu_pipeline_builder> agpu_pipeline_builder_ref;

// Interface wrapper for agpu_pipeline_state.
struct _agpu_pipeline_state
{
private:
	_agpu_pipeline_state() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddPipelineStateReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleasePipelineState(this));
	}

};

typedef agpu_ref<agpu_pipeline_state> agpu_pipeline_state_ref;

// Interface wrapper for agpu_command_queue.
struct _agpu_command_queue
{
private:
	_agpu_command_queue() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddCommandQueueReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseCommandQueue(this));
	}

	inline void addCommandList(const agpu_ref<agpu_command_list>& command_list)
	{
		agpuThrowIfFailed(agpuAddCommandList(this, command_list.get()));
	}

	inline void finishExecution()
	{
		agpuThrowIfFailed(agpuFinishQueueExecution(this));
	}

	inline void signalFence(const agpu_ref<agpu_fence>& fence)
	{
		agpuThrowIfFailed(agpuSignalFence(this, fence.get()));
	}

	inline void waitFence(const agpu_ref<agpu_fence>& fence)
	{
		agpuThrowIfFailed(agpuWaitFence(this, fence.get()));
	}

};

typedef agpu_ref<agpu_command_queue> agpu_command_queue_ref;

// Interface wrapper for agpu_command_allocator.
struct _agpu_command_allocator
{
private:
	_agpu_command_allocator() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddCommandAllocatorReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseCommandAllocator(this));
	}

	inline void reset()
	{
		agpuThrowIfFailed(agpuResetCommandAllocator(this));
	}

};

typedef agpu_ref<agpu_command_allocator> agpu_command_allocator_ref;

// Interface wrapper for agpu_command_list.
struct _agpu_command_list
{
private:
	_agpu_command_list() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddCommandListReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseCommandList(this));
	}

	inline void setShaderSignature(const agpu_ref<agpu_shader_signature>& signature)
	{
		agpuThrowIfFailed(agpuSetShaderSignature(this, signature.get()));
	}

	inline void setViewport(agpu_int x, agpu_int y, agpu_int w, agpu_int h)
	{
		agpuThrowIfFailed(agpuSetViewport(this, x, y, w, h));
	}

	inline void setScissor(agpu_int x, agpu_int y, agpu_int w, agpu_int h)
	{
		agpuThrowIfFailed(agpuSetScissor(this, x, y, w, h));
	}

	inline void usePipelineState(const agpu_ref<agpu_pipeline_state>& pipeline)
	{
		agpuThrowIfFailed(agpuUsePipelineState(this, pipeline.get()));
	}

	inline void useVertexBinding(const agpu_ref<agpu_vertex_binding>& vertex_binding)
	{
		agpuThrowIfFailed(agpuUseVertexBinding(this, vertex_binding.get()));
	}

	inline void useIndexBuffer(const agpu_ref<agpu_buffer>& index_buffer)
	{
		agpuThrowIfFailed(agpuUseIndexBuffer(this, index_buffer.get()));
	}

	inline void useIndexBufferAt(const agpu_ref<agpu_buffer>& index_buffer, agpu_size offset, agpu_size index_size)
	{
		agpuThrowIfFailed(agpuUseIndexBufferAt(this, index_buffer.get(), offset, index_size));
	}

	inline void useDrawIndirectBuffer(const agpu_ref<agpu_buffer>& draw_buffer)
	{
		agpuThrowIfFailed(agpuUseDrawIndirectBuffer(this, draw_buffer.get()));
	}

	inline void useComputeDispatchIndirectBuffer(const agpu_ref<agpu_buffer>& buffer)
	{
		agpuThrowIfFailed(agpuUseComputeDispatchIndirectBuffer(this, buffer.get()));
	}

	inline void useShaderResources(const agpu_ref<agpu_shader_resource_binding>& binding)
	{
		agpuThrowIfFailed(agpuUseShaderResources(this, binding.get()));
	}

	inline void useComputeShaderResources(const agpu_ref<agpu_shader_resource_binding>& binding)
	{
		agpuThrowIfFailed(agpuUseComputeShaderResources(this, binding.get()));
	}

	inline void drawArrays(agpu_uint vertex_count, agpu_uint instance_count, agpu_uint first_vertex, agpu_uint base_instance)
	{
		agpuThrowIfFailed(agpuDrawArrays(this, vertex_count, instance_count, first_vertex, base_instance));
	}

	inline void drawArraysIndirect(agpu_size offset, agpu_size drawcount)
	{
		agpuThrowIfFailed(agpuDrawArraysIndirect(this, offset, drawcount));
	}

	inline void drawElements(agpu_uint index_count, agpu_uint instance_count, agpu_uint first_index, agpu_int base_vertex, agpu_uint base_instance)
	{
		agpuThrowIfFailed(agpuDrawElements(this, index_count, instance_count, first_index, base_vertex, base_instance));
	}

	inline void drawElementsIndirect(agpu_size offset, agpu_size drawcount)
	{
		agpuThrowIfFailed(agpuDrawElementsIndirect(this, offset, drawcount));
	}

	inline void dispatchCompute(agpu_uint group_count_x, agpu_uint group_count_y, agpu_uint group_count_z)
	{
		agpuThrowIfFailed(agpuDispatchCompute(this, group_count_x, group_count_y, group_count_z));
	}

	inline void dispatchComputeIndirect(agpu_size offset)
	{
		agpuThrowIfFailed(agpuDispatchComputeIndirect(this, offset));
	}

	inline void setStencilReference(agpu_uint reference)
	{
		agpuThrowIfFailed(agpuSetStencilReference(this, reference));
	}

	inline void executeBundle(const agpu_ref<agpu_command_list>& bundle)
	{
		agpuThrowIfFailed(agpuExecuteBundle(this, bundle.get()));
	}

	inline void close()
	{
		agpuThrowIfFailed(agpuCloseCommandList(this));
	}

	inline void reset(const agpu_ref<agpu_command_allocator>& allocator, const agpu_ref<agpu_pipeline_state>& initial_pipeline_state)
	{
		agpuThrowIfFailed(agpuResetCommandList(this, allocator.get(), initial_pipeline_state.get()));
	}

	inline void resetBundle(const agpu_ref<agpu_command_allocator>& allocator, const agpu_ref<agpu_pipeline_state>& initial_pipeline_state, agpu_inheritance_info* inheritance_info)
	{
		agpuThrowIfFailed(agpuResetBundleCommandList(this, allocator.get(), initial_pipeline_state.get(), inheritance_info));
	}

	inline void beginRenderPass(const agpu_ref<agpu_renderpass>& renderpass, const agpu_ref<agpu_framebuffer>& framebuffer, agpu_bool bundle_content)
	{
		agpuThrowIfFailed(agpuBeginRenderPass(this, renderpass.get(), framebuffer.get(), bundle_content));
	}

	inline void endRenderPass()
	{
		agpuThrowIfFailed(agpuEndRenderPass(this));
	}

	inline void resolveFramebuffer(const agpu_ref<agpu_framebuffer>& destFramebuffer, const agpu_ref<agpu_framebuffer>& sourceFramebuffer)
	{
		agpuThrowIfFailed(agpuResolveFramebuffer(this, destFramebuffer.get(), sourceFramebuffer.get()));
	}

	inline void resolveTexture(const agpu_ref<agpu_texture>& sourceTexture, agpu_uint sourceLevel, agpu_uint sourceLayer, const agpu_ref<agpu_texture>& destTexture, agpu_uint destLevel, agpu_uint destLayer, agpu_uint levelCount, agpu_uint layerCount, agpu_texture_aspect aspect)
	{
		agpuThrowIfFailed(agpuResolveTexture(this, sourceTexture.get(), sourceLevel, sourceLayer, destTexture.get(), destLevel, destLayer, levelCount, layerCount, aspect));
	}

	inline void pushConstants(agpu_uint offset, agpu_uint size, agpu_pointer values)
	{
		agpuThrowIfFailed(agpuPushConstants(this, offset, size, values));
	}

	inline void memoryBarrier(agpu_pipeline_stage_flags source_stage, agpu_pipeline_stage_flags dest_stage, agpu_access_flags source_accesses, agpu_access_flags dest_accesses)
	{
		agpuThrowIfFailed(agpuMemoryBarrier(this, source_stage, dest_stage, source_accesses, dest_accesses));
	}

};

typedef agpu_ref<agpu_command_list> agpu_command_list_ref;

// Interface wrapper for agpu_texture.
struct _agpu_texture
{
private:
	_agpu_texture() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddTextureReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseTexture(this));
	}

	inline void getDescription(agpu_texture_description* description)
	{
		agpuThrowIfFailed(agpuGetTextureDescription(this, description));
	}

	inline agpu_pointer mapLevel(agpu_int level, agpu_int arrayIndex, agpu_mapping_access flags, agpu_region3d* region)
	{
		return agpuMapTextureLevel(this, level, arrayIndex, flags, region);
	}

	inline void unmapLevel()
	{
		agpuThrowIfFailed(agpuUnmapTextureLevel(this));
	}

	inline void readTextureData(agpu_int level, agpu_int arrayIndex, agpu_int pitch, agpu_int slicePitch, agpu_pointer buffer)
	{
		agpuThrowIfFailed(agpuReadTextureData(this, level, arrayIndex, pitch, slicePitch, buffer));
	}

	inline void uploadTextureData(agpu_int level, agpu_int arrayIndex, agpu_int pitch, agpu_int slicePitch, agpu_pointer data)
	{
		agpuThrowIfFailed(agpuUploadTextureData(this, level, arrayIndex, pitch, slicePitch, data));
	}

	inline void uploadTextureSubData(agpu_int level, agpu_int arrayIndex, agpu_int pitch, agpu_int slicePitch, agpu_size3d* sourceSize, agpu_region3d* destRegion, agpu_pointer data)
	{
		agpuThrowIfFailed(agpuUploadTextureSubData(this, level, arrayIndex, pitch, slicePitch, sourceSize, destRegion, data));
	}

	inline void discardUploadBuffer()
	{
		agpuThrowIfFailed(agpuDiscardTextureUploadBuffer(this));
	}

	inline void discardReadbackBuffer()
	{
		agpuThrowIfFailed(agpuDiscardTextureReadbackBuffer(this));
	}

	inline void getFullViewDescription(agpu_texture_view_description* result)
	{
		agpuThrowIfFailed(agpuGetTextureFullViewDescription(this, result));
	}

};

typedef agpu_ref<agpu_texture> agpu_texture_ref;

// Interface wrapper for agpu_buffer.
struct _agpu_buffer
{
private:
	_agpu_buffer() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddBufferReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseBuffer(this));
	}

	inline agpu_pointer mapBuffer(agpu_mapping_access flags)
	{
		return agpuMapBuffer(this, flags);
	}

	inline void unmapBuffer()
	{
		agpuThrowIfFailed(agpuUnmapBuffer(this));
	}

	inline void getDescription(agpu_buffer_description* description)
	{
		agpuThrowIfFailed(agpuGetBufferDescription(this, description));
	}

	inline void uploadBufferData(agpu_size offset, agpu_size size, agpu_pointer data)
	{
		agpuThrowIfFailed(agpuUploadBufferData(this, offset, size, data));
	}

	inline void readBufferData(agpu_size offset, agpu_size size, agpu_pointer data)
	{
		agpuThrowIfFailed(agpuReadBufferData(this, offset, size, data));
	}

	inline void flushWholeBuffer()
	{
		agpuThrowIfFailed(agpuFlushWholeBuffer(this));
	}

	inline void invalidateWholeBuffer()
	{
		agpuThrowIfFailed(agpuInvalidateWholeBuffer(this));
	}

};

typedef agpu_ref<agpu_buffer> agpu_buffer_ref;

// Interface wrapper for agpu_vertex_binding.
struct _agpu_vertex_binding
{
private:
	_agpu_vertex_binding() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddVertexBindingReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseVertexBinding(this));
	}

	inline void bindVertexBuffers(agpu_uint count, agpu_ref<agpu_buffer>* vertex_buffers)
	{
		agpuThrowIfFailed(agpuBindVertexBuffers(this, count, reinterpret_cast<agpu_buffer**> (vertex_buffers)));
	}

	inline void bindVertexBuffersWithOffsets(agpu_uint count, agpu_ref<agpu_buffer>* vertex_buffers, agpu_size* offsets)
	{
		agpuThrowIfFailed(agpuBindVertexBuffersWithOffsets(this, count, reinterpret_cast<agpu_buffer**> (vertex_buffers), offsets));
	}

};

typedef agpu_ref<agpu_vertex_binding> agpu_vertex_binding_ref;

// Interface wrapper for agpu_vertex_layout.
struct _agpu_vertex_layout
{
private:
	_agpu_vertex_layout() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddVertexLayoutReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseVertexLayout(this));
	}

	inline void addVertexAttributeBindings(agpu_uint vertex_buffer_count, agpu_size* vertex_strides, agpu_size attribute_count, agpu_vertex_attrib_description* attributes)
	{
		agpuThrowIfFailed(agpuAddVertexAttributeBindings(this, vertex_buffer_count, vertex_strides, attribute_count, attributes));
	}

};

typedef agpu_ref<agpu_vertex_layout> agpu_vertex_layout_ref;

// Interface wrapper for agpu_shader.
struct _agpu_shader
{
private:
	_agpu_shader() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddShaderReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseShader(this));
	}

	inline void setShaderSource(agpu_shader_language language, agpu_string sourceText, agpu_string_length sourceTextLength)
	{
		agpuThrowIfFailed(agpuSetShaderSource(this, language, sourceText, sourceTextLength));
	}

	inline void compileShader(agpu_cstring options)
	{
		agpuThrowIfFailed(agpuCompileShader(this, options));
	}

	inline agpu_size getCompilationLogLength()
	{
		return agpuGetShaderCompilationLogLength(this);
	}

	inline void getCompilationLog(agpu_size buffer_size, agpu_string_buffer buffer)
	{
		agpuThrowIfFailed(agpuGetShaderCompilationLog(this, buffer_size, buffer));
	}

};

typedef agpu_ref<agpu_shader> agpu_shader_ref;

// Interface wrapper for agpu_framebuffer.
struct _agpu_framebuffer
{
private:
	_agpu_framebuffer() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddFramebufferReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseFramebuffer(this));
	}

};

typedef agpu_ref<agpu_framebuffer> agpu_framebuffer_ref;

// Interface wrapper for agpu_renderpass.
struct _agpu_renderpass
{
private:
	_agpu_renderpass() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddRenderPassReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseRenderPass(this));
	}

	inline void setDepthStencilClearValue(agpu_depth_stencil_value value)
	{
		agpuThrowIfFailed(agpuSetDepthStencilClearValue(this, value));
	}

	inline void setColorClearValue(agpu_uint attachment_index, agpu_color4f value)
	{
		agpuThrowIfFailed(agpuSetColorClearValue(this, attachment_index, value));
	}

	inline void setColorClearValueFrom(agpu_uint attachment_index, agpu_color4f* value)
	{
		agpuThrowIfFailed(agpuSetColorClearValueFrom(this, attachment_index, value));
	}

	inline void getColorAttachmentFormats(agpu_uint* color_attachment_count, agpu_texture_format* formats)
	{
		agpuThrowIfFailed(agpuGetRenderPassColorAttachmentFormats(this, color_attachment_count, formats));
	}

	inline agpu_texture_format getDepthStencilAttachmentFormat()
	{
		return agpuGetRenderPassgetDepthStencilAttachmentFormat(this);
	}

};

typedef agpu_ref<agpu_renderpass> agpu_renderpass_ref;

// Interface wrapper for agpu_shader_signature_builder.
struct _agpu_shader_signature_builder
{
private:
	_agpu_shader_signature_builder() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddShaderSignatureBuilderReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseShaderSignatureBuilder(this));
	}

	inline agpu_ref<agpu_shader_signature> build()
	{
		return agpuBuildShaderSignature(this);
	}

	inline void addBindingConstant()
	{
		agpuThrowIfFailed(agpuAddShaderSignatureBindingConstant(this));
	}

	inline void addBindingElement(agpu_shader_binding_type type, agpu_uint maxBindings)
	{
		agpuThrowIfFailed(agpuAddShaderSignatureBindingElement(this, type, maxBindings));
	}

	inline void beginBindingBank(agpu_uint maxBindings)
	{
		agpuThrowIfFailed(agpuBeginShaderSignatureBindingBank(this, maxBindings));
	}

	inline void addBindingBankElement(agpu_shader_binding_type type, agpu_uint bindingPointCount)
	{
		agpuThrowIfFailed(agpuAddShaderSignatureBindingBankElement(this, type, bindingPointCount));
	}

};

typedef agpu_ref<agpu_shader_signature_builder> agpu_shader_signature_builder_ref;

// Interface wrapper for agpu_shader_signature.
struct _agpu_shader_signature
{
private:
	_agpu_shader_signature() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddShaderSignature(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseShaderSignature(this));
	}

	inline agpu_ref<agpu_shader_resource_binding> createShaderResourceBinding(agpu_uint element)
	{
		return agpuCreateShaderResourceBinding(this, element);
	}

};

typedef agpu_ref<agpu_shader_signature> agpu_shader_signature_ref;

// Interface wrapper for agpu_shader_resource_binding.
struct _agpu_shader_resource_binding
{
private:
	_agpu_shader_resource_binding() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddShaderResourceBindingReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseShaderResourceBinding(this));
	}

	inline void bindUniformBuffer(agpu_int location, const agpu_ref<agpu_buffer>& uniform_buffer)
	{
		agpuThrowIfFailed(agpuBindUniformBuffer(this, location, uniform_buffer.get()));
	}

	inline void bindUniformBufferRange(agpu_int location, const agpu_ref<agpu_buffer>& uniform_buffer, agpu_size offset, agpu_size size)
	{
		agpuThrowIfFailed(agpuBindUniformBufferRange(this, location, uniform_buffer.get(), offset, size));
	}

	inline void bindStorageBuffer(agpu_int location, const agpu_ref<agpu_buffer>& storage_buffer)
	{
		agpuThrowIfFailed(agpuBindStorageBuffer(this, location, storage_buffer.get()));
	}

	inline void bindStorageBufferRange(agpu_int location, const agpu_ref<agpu_buffer>& storage_buffer, agpu_size offset, agpu_size size)
	{
		agpuThrowIfFailed(agpuBindStorageBufferRange(this, location, storage_buffer.get(), offset, size));
	}

	inline void bindTexture(agpu_int location, const agpu_ref<agpu_texture>& texture, agpu_uint startMiplevel, agpu_int miplevels, agpu_float lodclamp)
	{
		agpuThrowIfFailed(agpuBindTexture(this, location, texture.get(), startMiplevel, miplevels, lodclamp));
	}

	inline void bindTextureArrayRange(agpu_int location, const agpu_ref<agpu_texture>& texture, agpu_uint startMiplevel, agpu_int miplevels, agpu_int firstElement, agpu_int numberOfElements, agpu_float lodclamp)
	{
		agpuThrowIfFailed(agpuBindTextureArrayRange(this, location, texture.get(), startMiplevel, miplevels, firstElement, numberOfElements, lodclamp));
	}

	inline void bindImage(agpu_int location, const agpu_ref<agpu_texture>& texture, agpu_int level, agpu_int layer, agpu_mapping_access access, agpu_texture_format format)
	{
		agpuThrowIfFailed(agpuBindImage(this, location, texture.get(), level, layer, access, format));
	}

	inline void createSampler(agpu_int location, agpu_sampler_description* description)
	{
		agpuThrowIfFailed(agpuCreateSampler(this, location, description));
	}

};

typedef agpu_ref<agpu_shader_resource_binding> agpu_shader_resource_binding_ref;

// Interface wrapper for agpu_fence.
struct _agpu_fence
{
private:
	_agpu_fence() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddFenceReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseFenceReference(this));
	}

	inline void waitOnClient()
	{
		agpuThrowIfFailed(agpuWaitOnClient(this));
	}

};

typedef agpu_ref<agpu_fence> agpu_fence_ref;

// Interface wrapper for agpu_offline_shader_compiler.
struct _agpu_offline_shader_compiler
{
private:
	_agpu_offline_shader_compiler() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddOfflineShaderCompilerReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseOfflineShaderCompiler(this));
	}

	inline agpu_bool isShaderLanguageSupported(agpu_shader_language language)
	{
		return agpuisShaderLanguageSupportedByOfflineCompiler(this, language);
	}

	inline agpu_bool isTargetShaderLanguageSupported(agpu_shader_language language)
	{
		return agpuisTargetShaderLanguageSupportedByOfflineCompiler(this, language);
	}

	inline void setShaderSource(agpu_shader_language language, agpu_shader_type stage, agpu_string sourceText, agpu_string_length sourceTextLength)
	{
		agpuThrowIfFailed(agpuSetOfflineShaderCompilerSource(this, language, stage, sourceText, sourceTextLength));
	}

	inline void compileShader(agpu_shader_language target_language, agpu_cstring options)
	{
		agpuThrowIfFailed(agpuCompileOfflineShader(this, target_language, options));
	}

	inline agpu_size getCompilationLogLength()
	{
		return agpuGetOfflineShaderCompilationLogLength(this);
	}

	inline void getCompilationLog(agpu_size buffer_size, agpu_string_buffer buffer)
	{
		agpuThrowIfFailed(agpuGetOfflineShaderCompilationLog(this, buffer_size, buffer));
	}

	inline agpu_size getCompilationResultLength()
	{
		return agpuGetOfflineShaderCompilationResultLength(this);
	}

	inline void getCompilationResult(agpu_size buffer_size, agpu_string_buffer buffer)
	{
		agpuThrowIfFailed(agpuGetOfflineShaderCompilationResult(this, buffer_size, buffer));
	}

	inline agpu_ref<agpu_shader> getResultAsShader()
	{
		return agpuGetOfflineShaderCompilerResultAsShader(this);
	}

};

typedef agpu_ref<agpu_offline_shader_compiler> agpu_offline_shader_compiler_ref;

// Interface wrapper for agpu_state_tracker_cache.
struct _agpu_state_tracker_cache
{
private:
	_agpu_state_tracker_cache() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddStateTrackerCacheReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseStateTrackerCacheReference(this));
	}

	inline agpu_ref<agpu_state_tracker> createStateTracker(agpu_command_list_type type, const agpu_ref<agpu_command_queue>& command_queue)
	{
		return agpuCreateStateTracker(this, type, command_queue.get());
	}

	inline agpu_ref<agpu_state_tracker> createStateTrackerWithCommandAllocator(agpu_command_list_type type, const agpu_ref<agpu_command_queue>& command_queue, const agpu_ref<agpu_command_allocator>& command_allocator)
	{
		return agpuCreateStateTrackerWithCommandAllocator(this, type, command_queue.get(), command_allocator.get());
	}

	inline agpu_ref<agpu_state_tracker> createStateTrackerWithFrameBuffering(agpu_command_list_type type, const agpu_ref<agpu_command_queue>& command_queue, agpu_uint framebuffering_count)
	{
		return agpuCreateStateTrackerWithFrameBuffering(this, type, command_queue.get(), framebuffering_count);
	}

	inline agpu_ref<agpu_immediate_renderer> createImmediateRenderer()
	{
		return agpuCreateImmediateRenderer(this);
	}

};

typedef agpu_ref<agpu_state_tracker_cache> agpu_state_tracker_cache_ref;

// Interface wrapper for agpu_state_tracker.
struct _agpu_state_tracker
{
private:
	_agpu_state_tracker() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddStateTrackerReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseStateTrackerReference(this));
	}

	inline void beginRecordingCommands()
	{
		agpuThrowIfFailed(agpuStateTrackerBeginRecordingCommands(this));
	}

	inline agpu_ref<agpu_command_list> endRecordingCommands()
	{
		return agpuStateTrackerEndRecordingCommands(this);
	}

	inline void endRecordingAndFlushCommands()
	{
		agpuThrowIfFailed(agpuStateTrackerEndRecordingAndFlushCommands(this));
	}

	inline void reset()
	{
		agpuThrowIfFailed(agpuStateTrackerReset(this));
	}

	inline void resetGraphicsPipeline()
	{
		agpuThrowIfFailed(agpuStateTrackerResetGraphicsPipeline(this));
	}

	inline void resetComputePipeline()
	{
		agpuThrowIfFailed(agpuStateTrackerResetComputePipeline(this));
	}

	inline void setComputeStage(const agpu_ref<agpu_shader>& shader, agpu_cstring entryPoint)
	{
		agpuThrowIfFailed(agpuStateTrackerSetComputeStage(this, shader.get(), entryPoint));
	}

	inline void setVertexStage(const agpu_ref<agpu_shader>& shader, agpu_cstring entryPoint)
	{
		agpuThrowIfFailed(agpuStateTrackerSetVertexStage(this, shader.get(), entryPoint));
	}

	inline void setFragmentStage(const agpu_ref<agpu_shader>& shader, agpu_cstring entryPoint)
	{
		agpuThrowIfFailed(agpuStateTrackerSetFragmentStage(this, shader.get(), entryPoint));
	}

	inline void setGeometryStage(const agpu_ref<agpu_shader>& shader, agpu_cstring entryPoint)
	{
		agpuThrowIfFailed(agpuStateTrackerSetGeometryStage(this, shader.get(), entryPoint));
	}

	inline void setTessellationControlStage(const agpu_ref<agpu_shader>& shader, agpu_cstring entryPoint)
	{
		agpuThrowIfFailed(agpuStateTrackerSetTessellationControlStage(this, shader.get(), entryPoint));
	}

	inline void setTessellationEvaluationStage(const agpu_ref<agpu_shader>& shader, agpu_cstring entryPoint)
	{
		agpuThrowIfFailed(agpuStateTrackerSetTessellationEvaluationStage(this, shader.get(), entryPoint));
	}

	inline void setBlendState(agpu_int renderTargetMask, agpu_bool enabled)
	{
		agpuThrowIfFailed(agpuStateTrackerSetBlendState(this, renderTargetMask, enabled));
	}

	inline void setBlendFunction(agpu_int renderTargetMask, agpu_blending_factor sourceFactor, agpu_blending_factor destFactor, agpu_blending_operation colorOperation, agpu_blending_factor sourceAlphaFactor, agpu_blending_factor destAlphaFactor, agpu_blending_operation alphaOperation)
	{
		agpuThrowIfFailed(agpuStateTrackerSetBlendFunction(this, renderTargetMask, sourceFactor, destFactor, colorOperation, sourceAlphaFactor, destAlphaFactor, alphaOperation));
	}

	inline void setColorMask(agpu_int renderTargetMask, agpu_bool redEnabled, agpu_bool greenEnabled, agpu_bool blueEnabled, agpu_bool alphaEnabled)
	{
		agpuThrowIfFailed(agpuStateTrackerSetColorMask(this, renderTargetMask, redEnabled, greenEnabled, blueEnabled, alphaEnabled));
	}

	inline void setFrontFace(agpu_face_winding winding)
	{
		agpuThrowIfFailed(agpuStateTrackerSetFrontFace(this, winding));
	}

	inline void setCullMode(agpu_cull_mode mode)
	{
		agpuThrowIfFailed(agpuStateTrackerSetCullMode(this, mode));
	}

	inline void setDepthBias(agpu_float constant_factor, agpu_float clamp, agpu_float slope_factor)
	{
		agpuThrowIfFailed(agpuStateTrackerSetDepthBias(this, constant_factor, clamp, slope_factor));
	}

	inline void setDepthState(agpu_bool enabled, agpu_bool writeMask, agpu_compare_function function)
	{
		agpuThrowIfFailed(agpuStateTrackerSetDepthState(this, enabled, writeMask, function));
	}

	inline void setPolygonMode(agpu_polygon_mode mode)
	{
		agpuThrowIfFailed(agpuStateTrackerSetPolygonMode(this, mode));
	}

	inline void setStencilState(agpu_bool enabled, agpu_int writeMask, agpu_int readMask)
	{
		agpuThrowIfFailed(agpuStateTrackerSetStencilState(this, enabled, writeMask, readMask));
	}

	inline void setStencilFrontFace(agpu_stencil_operation stencilFailOperation, agpu_stencil_operation depthFailOperation, agpu_stencil_operation stencilDepthPassOperation, agpu_compare_function stencilFunction)
	{
		agpuThrowIfFailed(agpuStateTrackerSetStencilFrontFace(this, stencilFailOperation, depthFailOperation, stencilDepthPassOperation, stencilFunction));
	}

	inline void setStencilBackFace(agpu_stencil_operation stencilFailOperation, agpu_stencil_operation depthFailOperation, agpu_stencil_operation stencilDepthPassOperation, agpu_compare_function stencilFunction)
	{
		agpuThrowIfFailed(agpuStateTrackerSetStencilBackFace(this, stencilFailOperation, depthFailOperation, stencilDepthPassOperation, stencilFunction));
	}

	inline void setPrimitiveType(agpu_primitive_topology type)
	{
		agpuThrowIfFailed(agpuStateTrackerSetPrimitiveType(this, type));
	}

	inline void setVertexLayout(const agpu_ref<agpu_vertex_layout>& layout)
	{
		agpuThrowIfFailed(agpuStateTrackerSetVertexLayout(this, layout.get()));
	}

	inline void setShaderSignature(const agpu_ref<agpu_shader_signature>& signature)
	{
		agpuThrowIfFailed(agpuStateTrackerSetShaderSignature(this, signature.get()));
	}

	inline void setSampleDescription(agpu_uint sample_count, agpu_uint sample_quality)
	{
		agpuThrowIfFailed(agpuStateTrackerSetSampleDescription(this, sample_count, sample_quality));
	}

	inline void setViewport(agpu_int x, agpu_int y, agpu_int w, agpu_int h)
	{
		agpuThrowIfFailed(agpuStateTrackerSetViewport(this, x, y, w, h));
	}

	inline void setScissor(agpu_int x, agpu_int y, agpu_int w, agpu_int h)
	{
		agpuThrowIfFailed(agpuStateTrackerSetScissor(this, x, y, w, h));
	}

	inline void useVertexBinding(const agpu_ref<agpu_vertex_binding>& vertex_binding)
	{
		agpuThrowIfFailed(agpuStateTrackerUseVertexBinding(this, vertex_binding.get()));
	}

	inline void useIndexBuffer(const agpu_ref<agpu_buffer>& index_buffer)
	{
		agpuThrowIfFailed(agpuStateTrackerUseIndexBuffer(this, index_buffer.get()));
	}

	inline void useIndexBufferAt(const agpu_ref<agpu_buffer>& index_buffer, agpu_size offset, agpu_size index_size)
	{
		agpuThrowIfFailed(agpuStateTrackerUseIndexBufferAt(this, index_buffer.get(), offset, index_size));
	}

	inline void useDrawIndirectBuffer(const agpu_ref<agpu_buffer>& draw_buffer)
	{
		agpuThrowIfFailed(agpuStateTrackerUseDrawIndirectBuffer(this, draw_buffer.get()));
	}

	inline void useComputeDispatchIndirectBuffer(const agpu_ref<agpu_buffer>& buffer)
	{
		agpuThrowIfFailed(agpuStateTrackerUseComputeDispatchIndirectBuffer(this, buffer.get()));
	}

	inline void useShaderResources(const agpu_ref<agpu_shader_resource_binding>& binding)
	{
		agpuThrowIfFailed(agpuStateTrackerUseShaderResources(this, binding.get()));
	}

	inline void useComputeShaderResources(const agpu_ref<agpu_shader_resource_binding>& binding)
	{
		agpuThrowIfFailed(agpuStateTrackerUseComputeShaderResources(this, binding.get()));
	}

	inline void drawArrays(agpu_uint vertex_count, agpu_uint instance_count, agpu_uint first_vertex, agpu_uint base_instance)
	{
		agpuThrowIfFailed(agpuStateTrackerDrawArrays(this, vertex_count, instance_count, first_vertex, base_instance));
	}

	inline void drawArraysIndirect(agpu_size offset, agpu_size drawcount)
	{
		agpuThrowIfFailed(agpuStateTrackerDrawArraysIndirect(this, offset, drawcount));
	}

	inline void drawElements(agpu_uint index_count, agpu_uint instance_count, agpu_uint first_index, agpu_int base_vertex, agpu_uint base_instance)
	{
		agpuThrowIfFailed(agpuStateTrackerDrawElements(this, index_count, instance_count, first_index, base_vertex, base_instance));
	}

	inline void drawElementsIndirect(agpu_size offset, agpu_size drawcount)
	{
		agpuThrowIfFailed(agpuStateTrackerDrawElementsIndirect(this, offset, drawcount));
	}

	inline void dispatchCompute(agpu_uint group_count_x, agpu_uint group_count_y, agpu_uint group_count_z)
	{
		agpuThrowIfFailed(agpuStateTrackerDispatchCompute(this, group_count_x, group_count_y, group_count_z));
	}

	inline void dispatchComputeIndirect(agpu_size offset)
	{
		agpuThrowIfFailed(agpuStateTrackerDispatchComputeIndirect(this, offset));
	}

	inline void setStencilReference(agpu_uint reference)
	{
		agpuThrowIfFailed(agpuStateTrackerSetStencilReference(this, reference));
	}

	inline void executeBundle(const agpu_ref<agpu_command_list>& bundle)
	{
		agpuThrowIfFailed(agpuStateTrackerExecuteBundle(this, bundle.get()));
	}

	inline void beginRenderPass(const agpu_ref<agpu_renderpass>& renderpass, const agpu_ref<agpu_framebuffer>& framebuffer, agpu_bool bundle_content)
	{
		agpuThrowIfFailed(agpuStateTrackerBeginRenderPass(this, renderpass.get(), framebuffer.get(), bundle_content));
	}

	inline void endRenderPass()
	{
		agpuThrowIfFailed(agpuStateTrackerEndRenderPass(this));
	}

	inline void resolveFramebuffer(const agpu_ref<agpu_framebuffer>& destFramebuffer, const agpu_ref<agpu_framebuffer>& sourceFramebuffer)
	{
		agpuThrowIfFailed(agpuStateTrackerResolveFramebuffer(this, destFramebuffer.get(), sourceFramebuffer.get()));
	}

	inline void resolveTexture(const agpu_ref<agpu_texture>& sourceTexture, agpu_uint sourceLevel, agpu_uint sourceLayer, const agpu_ref<agpu_texture>& destTexture, agpu_uint destLevel, agpu_uint destLayer, agpu_uint levelCount, agpu_uint layerCount, agpu_texture_aspect aspect)
	{
		agpuThrowIfFailed(agpuStateTrackerResolveTexture(this, sourceTexture.get(), sourceLevel, sourceLayer, destTexture.get(), destLevel, destLayer, levelCount, layerCount, aspect));
	}

	inline void pushConstants(agpu_uint offset, agpu_uint size, agpu_pointer values)
	{
		agpuThrowIfFailed(agpuStateTrackerPushConstants(this, offset, size, values));
	}

	inline void memoryBarrier(agpu_pipeline_stage_flags source_stage, agpu_pipeline_stage_flags dest_stage, agpu_access_flags source_accesses, agpu_access_flags dest_accesses)
	{
		agpuThrowIfFailed(agpuStateTrackerMemoryBarrier(this, source_stage, dest_stage, source_accesses, dest_accesses));
	}

};

typedef agpu_ref<agpu_state_tracker> agpu_state_tracker_ref;

// Interface wrapper for agpu_immediate_renderer.
struct _agpu_immediate_renderer
{
private:
	_agpu_immediate_renderer() {}

public:
	inline void addReference()
	{
		agpuThrowIfFailed(agpuAddImmediateRendererReference(this));
	}

	inline void release()
	{
		agpuThrowIfFailed(agpuReleaseImmediateRendererReference(this));
	}

	inline void beginRendering(const agpu_ref<agpu_state_tracker>& state_tracker)
	{
		agpuThrowIfFailed(agpuBeginImmediateRendering(this, state_tracker.get()));
	}

	inline void endRendering()
	{
		agpuThrowIfFailed(agpuEndImmediateRendering(this));
	}

	inline void setBlendState(agpu_int renderTargetMask, agpu_bool enabled)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetBlendState(this, renderTargetMask, enabled));
	}

	inline void setBlendFunction(agpu_int renderTargetMask, agpu_blending_factor sourceFactor, agpu_blending_factor destFactor, agpu_blending_operation colorOperation, agpu_blending_factor sourceAlphaFactor, agpu_blending_factor destAlphaFactor, agpu_blending_operation alphaOperation)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetBlendFunction(this, renderTargetMask, sourceFactor, destFactor, colorOperation, sourceAlphaFactor, destAlphaFactor, alphaOperation));
	}

	inline void setColorMask(agpu_int renderTargetMask, agpu_bool redEnabled, agpu_bool greenEnabled, agpu_bool blueEnabled, agpu_bool alphaEnabled)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetColorMask(this, renderTargetMask, redEnabled, greenEnabled, blueEnabled, alphaEnabled));
	}

	inline void setFrontFace(agpu_face_winding winding)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetFrontFace(this, winding));
	}

	inline void setCullMode(agpu_cull_mode mode)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetCullMode(this, mode));
	}

	inline void setDepthBias(agpu_float constant_factor, agpu_float clamp, agpu_float slope_factor)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetDepthBias(this, constant_factor, clamp, slope_factor));
	}

	inline void setDepthState(agpu_bool enabled, agpu_bool writeMask, agpu_compare_function function)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetDepthState(this, enabled, writeMask, function));
	}

	inline void setPolygonMode(agpu_polygon_mode mode)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetPolygonMode(this, mode));
	}

	inline void setStencilState(agpu_bool enabled, agpu_int writeMask, agpu_int readMask)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetStencilState(this, enabled, writeMask, readMask));
	}

	inline void setStencilFrontFace(agpu_stencil_operation stencilFailOperation, agpu_stencil_operation depthFailOperation, agpu_stencil_operation stencilDepthPassOperation, agpu_compare_function stencilFunction)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetStencilFrontFace(this, stencilFailOperation, depthFailOperation, stencilDepthPassOperation, stencilFunction));
	}

	inline void setStencilBackFace(agpu_stencil_operation stencilFailOperation, agpu_stencil_operation depthFailOperation, agpu_stencil_operation stencilDepthPassOperation, agpu_compare_function stencilFunction)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetStencilBackFace(this, stencilFailOperation, depthFailOperation, stencilDepthPassOperation, stencilFunction));
	}

	inline void setViewport(agpu_int x, agpu_int y, agpu_int w, agpu_int h)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetViewport(this, x, y, w, h));
	}

	inline void setScissor(agpu_int x, agpu_int y, agpu_int w, agpu_int h)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetScissor(this, x, y, w, h));
	}

	inline void setStencilReference(agpu_uint reference)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetStencilReference(this, reference));
	}

	inline void projectionMatrixMode()
	{
		agpuThrowIfFailed(agpuImmediateRendererProjectionMatrixMode(this));
	}

	inline void modelViewMatrixMode()
	{
		agpuThrowIfFailed(agpuImmediateRendererModelViewMatrixMode(this));
	}

	inline void loadIdentity()
	{
		agpuThrowIfFailed(agpuImmediateRendererIdentity(this));
	}

	inline void pushMatrix()
	{
		agpuThrowIfFailed(agpuImmediateRendererPushMatrix(this));
	}

	inline void popMatrix()
	{
		agpuThrowIfFailed(agpuImmediateRendererPopMatrix(this));
	}

	inline void ortho(agpu_float left, agpu_float right, agpu_float bottom, agpu_float top, agpu_float near, agpu_float far)
	{
		agpuThrowIfFailed(agpuImmediateRendererOrtho(this, left, right, bottom, top, near, far));
	}

	inline void frustum(agpu_float left, agpu_float right, agpu_float bottom, agpu_float top, agpu_float near, agpu_float far)
	{
		agpuThrowIfFailed(agpuImmediateRendererFrustum(this, left, right, bottom, top, near, far));
	}

	inline void perspective(agpu_float fovy, agpu_float aspect, agpu_float near, agpu_float far)
	{
		agpuThrowIfFailed(agpuImmediateRendererPerspective(this, fovy, aspect, near, far));
	}

	inline void rotate(agpu_float angle, agpu_float x, agpu_float y, agpu_float z)
	{
		agpuThrowIfFailed(agpuImmediateRendererRotate(this, angle, x, y, z));
	}

	inline void translate(agpu_float x, agpu_float y, agpu_float z)
	{
		agpuThrowIfFailed(agpuImmediateRendererTranslate(this, x, y, z));
	}

	inline void scale(agpu_float x, agpu_float y, agpu_float z)
	{
		agpuThrowIfFailed(agpuImmediateRendererScale(this, x, y, z));
	}

	inline void setFlatShading(agpu_bool enabled)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetFlatShading(this, enabled));
	}

	inline void setLightingEnabled(agpu_bool enabled)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetLightingEnabled(this, enabled));
	}

	inline void clearLights()
	{
		agpuThrowIfFailed(agpuImmediateRendererClearLights(this));
	}

	inline void setAmbientLighting(agpu_float r, agpu_float g, agpu_float b, agpu_float a)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetAmbientLighting(this, r, g, b, a));
	}

	inline void setLight(agpu_uint index, agpu_bool enabled, agpu_immediate_renderer_light* state)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetLight(this, index, enabled, state));
	}

	inline void setTexturingEnabled(agpu_bool enabled)
	{
		agpuThrowIfFailed(agpuImmediateRendererSetTextureEnabled(this, enabled));
	}

	inline void bindTexture(const agpu_ref<agpu_texture>& texture)
	{
		agpuThrowIfFailed(agpuImmediateRendererBindTexture(this, texture.get()));
	}

	inline void beginPrimitives(agpu_primitive_topology type)
	{
		agpuThrowIfFailed(agpuBeginImmediateRendererPrimitives(this, type));
	}

	inline void endPrimitives()
	{
		agpuThrowIfFailed(agpuEndImmediateRendererPrimitives(this));
	}

	inline void color(agpu_float r, agpu_float g, agpu_float b, agpu_float a)
	{
		agpuThrowIfFailed(agpuSetImmediateRendererColor(this, r, g, b, a));
	}

	inline void texcoord(agpu_float x, agpu_float y)
	{
		agpuThrowIfFailed(agpuSetImmediateRendererTexcoord(this, x, y));
	}

	inline void normal(agpu_float x, agpu_float y, agpu_float z)
	{
		agpuThrowIfFailed(agpuSetImmediateRendererNormal(this, x, y, z));
	}

	inline void vertex(agpu_float x, agpu_float y, agpu_float z)
	{
		agpuThrowIfFailed(agpuAddImmediateRendererVertex(this, x, y, z));
	}

};

typedef agpu_ref<agpu_immediate_renderer> agpu_immediate_renderer_ref;


#endif /* AGPU_HPP_ */
