

#include <gmock/gmock.h>

#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/RenderTarget.h"

class MockRenderTarget : public pbge::RenderTarget {
public:
    MOCK_METHOD1(setRenderTarget, void(pbge::OpenGL * ogl));
    MOCK_METHOD1(unsetRenderTarget, void(pbge::OpenGL * ogl));
};