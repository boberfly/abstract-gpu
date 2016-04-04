#ifndef AGPU_METAL_SHADER_SIGNATURE_BUILDER_HPP
#define AGPU_METAL_SHADER_SIGNATURE_BUILDER_HPP

#include "device.hpp"

struct _agpu_shader_signature_builder: public Object<_agpu_shader_signature_builder>
{
public:
    _agpu_shader_signature_builder(agpu_device *device);
    void lostReferences();

    static _agpu_shader_signature_builder *create(agpu_device *device);

    agpu_shader_signature* build (  );
    agpu_error addBindingConstant (  );
    agpu_error addBindingElement ( agpu_shader_binding_type type, agpu_uint maxBindings );
    agpu_error addBindingBank ( agpu_shader_binding_type type, agpu_uint bindingPointCount, agpu_uint maxBindings );

    agpu_device *device;
};

#endif //AGPU_METAL_SHADER_SIGNATURE_BUILDER_HPP