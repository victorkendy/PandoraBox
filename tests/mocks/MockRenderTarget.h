

#include <gmock/gmock.h>

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/RenderTarget.h"

class MockRenderTarget : public pbge::RenderTarget {
public:
    MOCK_METHOD1(setRenderTarget, void(pbge::GraphicAPI * ogl));
    MOCK_METHOD1(unsetRenderTarget, void(pbge::GraphicAPI * ogl));
};