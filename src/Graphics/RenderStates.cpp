/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/RenderStates.hpp>
#include <cstddef>


namespace TGE
{
////////////////////////////////////////////////////////////
// We cannot use the default constructor here, because it accesses BlendAlpha, which is also global (and dynamically
// initialized). Initialization order of global objects in different translation units is not defined.
const RenderStates RenderStates::Default(BlendMode(
    BlendMode::SrcAlpha, BlendMode::OneMinusSrcAlpha, BlendMode::Add,
    BlendMode::One, BlendMode::OneMinusSrcAlpha, BlendMode::Add));


////////////////////////////////////////////////////////////
RenderStates::RenderStates() :
blendMode(BlendAlpha),
transform(),
texture  (NULL),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Transform& theTransform) :
blendMode(BlendAlpha),
transform(theTransform),
texture  (NULL),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const BlendMode& theBlendMode) :
blendMode(theBlendMode),
transform(),
texture  (NULL),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Texture* theTexture) :
blendMode(BlendAlpha),
transform(),
texture  (theTexture),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Shader* theShader) :
blendMode(BlendAlpha),
transform(),
texture  (NULL),
shader   (theShader)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const BlendMode& theBlendMode, const Transform& theTransform,
                           const Texture* theTexture, const Shader* theShader) :
blendMode(theBlendMode),
transform(theTransform),
texture  (theTexture),
shader   (theShader)
{
}

} // namespace TGE
