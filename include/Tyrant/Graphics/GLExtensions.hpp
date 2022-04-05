/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_GLEXTENSIONS_HPP
#define TGE_GLEXTENSIONS_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>

#include <GL/glew.h>
#include <Tyrant/OpenGL.hpp>

#define GLEXT_blend_func_separate              GLEW_EXT_blend_func_separate
#define GLEXT_blend_equation_separate          GLEW_EXT_blend_equation_separate
#define GLEXT_glBlendFuncSeparate              glBlendFuncSeparateEXT
#define GLEXT_glBlendEquationSeparate          glBlendEquationSeparateEXT
#define GLEXT_framebuffer_object               GLEW_EXT_framebuffer_object
#define GLEXT_glGenFramebuffers                glGenFramebuffersEXT
#define GLEXT_glGenRenderbuffers               glGenRenderbuffersEXT
#define GLEXT_glBindFramebuffer                glBindFramebufferEXT
#define GLEXT_glBindRenderbuffer               glBindRenderbufferEXT
#define GLEXT_glDeleteFramebuffers             glDeleteFramebuffersEXT
#define GLEXT_glDeleteRenderbuffers            glDeleteRenderbuffersEXT
#define GLEXT_glRenderbufferStorage            glRenderbufferStorageEXT
#define GLEXT_glFramebufferRenderbuffer        glFramebufferRenderbufferEXT
#define GLEXT_glFramebufferTexture2D           glFramebufferTexture2DEXT
#define GLEXT_glCheckFramebufferStatus         glCheckFramebufferStatusEXT
#define GLEXT_GL_FRAMEBUFFER                   GL_FRAMEBUFFER_EXT
#define GLEXT_GL_FRAMEBUFFER_BINDING           GL_FRAMEBUFFER_BINDING_EXT
#define GLEXT_GL_RENDERBUFFER                  GL_RENDERBUFFER_EXT
#define GLEXT_GL_COLOR_ATTACHMENT0             GL_COLOR_ATTACHMENT0_EXT
#define GLEXT_GL_DEPTH_ATTACHMENT              GL_DEPTH_ATTACHMENT_EXT
#define GLEXT_GL_FRAMEBUFFER_COMPLETE          GL_FRAMEBUFFER_COMPLETE_EXT
#define GLEXT_GL_DEPTH_COMPONENT               GL_DEPTH_COMPONENT
#define GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION GL_INVALID_FRAMEBUFFER_OPERATION_EXT
#define GLEXT_texture_non_power_of_two         GLEW_ARB_texture_non_power_of_two

namespace TGE
{
namespace priv
{

////////////////////////////////////////////////////////////
/// \brief Make sure that extensions are initialized
///
////////////////////////////////////////////////////////////
void ensureExtensionsInit();

} // namespace priv

} // namespace TGE


#endif // TGE_GLEXTENSIONS_HPP
