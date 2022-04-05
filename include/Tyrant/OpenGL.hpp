/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_OPENGL_HPP
#define TGE_OPENGL_HPP


/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>


////////////////////////////////////////////////////////////
/// This file just includes the OpenGL (GL and GLU) headers,
/// which have actually different paths on each system
////////////////////////////////////////////////////////////
#if defined(OS_WINDOWS)

    // The Visual C++ version of gl.h uses WINGDIAPI and APIENTRY but doesn't define them
    #ifdef _MSC_VER
        #include <windows.h>
    #endif

    #include <GL/gl.h>
    #include <GL/glu.h>

#elif defined(OS_LINUX)

	#include <GL/gl.h>
	#include <GL/glu.h>

#elif defined(OS_MAC)

    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>

#endif

#endif // TGE_OPENGL_HPP
