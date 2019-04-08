#include "device.hpp"

namespace AgpuGL
{

class GLPlatform : public agpu::platform
{
public:
    GLPlatform();
    ~GLPlatform();

    virtual agpu::device_ptr openDevice(agpu_device_open_info* openInfo) override;
	virtual agpu_cstring getName() override;
	virtual agpu_int getVersion() override;
	virtual agpu_int getImplementationVersion() override;
	virtual agpu_bool hasRealMultithreading() override;
	virtual agpu_bool isNative() override;
	virtual agpu_bool isCrossPlatform() override;
};

static agpu::platform_ref theGLPlatform;

GLPlatform::GLPlatform()
{
}

GLPlatform::~GLPlatform()
{
}

agpu::device_ptr GLPlatform::openDevice(agpu_device_open_info* openInfo)
{
    return GLDevice::open(openInfo).disown();
}

agpu_cstring GLPlatform::getName()
{
    return "OpenGL 4.x Core";
}

agpu_int GLPlatform::getVersion()
{
    return 100;
}

agpu_int GLPlatform::getImplementationVersion()
{
    return 1;
}

agpu_bool GLPlatform::hasRealMultithreading()
{
    return false;
}

agpu_bool GLPlatform::isNative()
{
    return false;
}

agpu_bool GLPlatform::isCrossPlatform()
{
    return true;
}

} // End of namespace AgpuGL

AGPU_EXPORT agpu_error agpuGetPlatforms ( agpu_size numplatforms, agpu_platform** platforms, agpu_size* ret_numplatforms )
{
    using namespace AgpuGL;
    if(!theGLPlatform)
        theGLPlatform = agpu::makeObject<GLPlatform> ();

    if (!platforms && numplatforms == 0)
    {
        CHECK_POINTER(ret_numplatforms);
        *ret_numplatforms = 1;
        return AGPU_OK;
    }

    if(ret_numplatforms)
        *ret_numplatforms = 1;
    platforms[0] = reinterpret_cast<agpu_platform*> (theGLPlatform.asPtrWithoutNewRef());
    return AGPU_OK;
}
