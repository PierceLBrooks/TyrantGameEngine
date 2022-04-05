################################################################
#                  Tyrant Game Engine Makefile                 #
#                Copyright © 2014 Coldsnap Games               #
################################################################



################################################################
##                         Variables                          ##
################################################################

# Default variables, you probably don't want\need to change these
# CC - The compiler
# INC - The include directories
# CFLAGS - Compiler flags to use
# LDFLAGS - Linker flags to use
# GMON_OUT_PREFIX - Sets the name of the profiler data file to be used with gprof when compiling in Debug
CC	= g++
INC	= -I..\..\include -I..\..\extlibs\headers -I..\..\extlibs\headers\jpeg -I..\..\extlibs\headers\libfreetype\windows -I..\..\extlibs\headers\libsndfile\windows
CFLAGS	= $(INC) -std=c++0x -DBUILDING_TYRANT -DGLEW_STATIC -DUNICODE -D_UNICODE
GMON_OUT_PREFIX = TYRANT_PROFILER_DATA


# Build specific variables, if need be define these when running make
# BUILD - Either Debug or Release, defaults to Release
# ARCH - Either 32 or 64, defaults to 32
BUILD ?= RELEASE
ARCH ?= 32

ifeq ($(ARCH),64)
	LDFLAGS := -L..\..\extlibs\libs-mingw\x64 -m64 -lglew32 -lopengl32 -lwinmm -lgdi32 -lwsock32 -lws2_32 -lfreetype -ljpeg -lopenal32 -lsndfile -pg -static-libgcc -static-libstdc++
else
	LDFLAGS := -L..\..\extlibs\libs-mingw\x32 -m32 -lglew -lopengl32 -lwinmm -lgdi32 -lwsock32 -lws2_32 -lfreetype -ljpeg -lopenal32 -lsndfile -pg -static-libgcc -static-libstdc++
endif

ifeq ($(BUILD),DEBUG)
	CFLAGS  := $(CFLAGS) -m$(ARCH) -g3 -pg -Og
endif

ifeq ($(BUILD),RELEASE)
	CFLAGS  := $(CFLAGS) -m$(ARCH) -s -O3
endif


# Directory variables, no need to change these
# SRCPATH - The directory for source files
# OBJPATH - Directory path for .o files
# BINPATH - Where to put the built library
SRCPATH	= ..\..\src
OBJPATH	= .\obj\$(BUILD)\$(ARCH)-bit
BINPATH	= .\bin\$(BUILD)\$(ARCH)-bit


# File variables, should only need to change when adding source files
# SOURCES - Path to each individual source file
# OBJECTS - Path to output individual object files
SOURCES	= System\Time.cpp System\Mutex.cpp System\Log.cpp System\Win32\ClockImpl.cpp System\Win32\MutexImpl.cpp System\Win32\SleepImpl.cpp System\Win32\ThreadImpl.cpp System\Win32\ThreadLocalImpl.cpp System\Clock.cpp System\Sleep.cpp System\Lock.cpp System\String.cpp System\ThreadLocal.cpp System\Thread.cpp Audio\SoundRecorder.cpp Audio\SoundBuffer.cpp Audio\SoundSource.cpp Audio\AudioDevice.cpp Audio\ALCheck.cpp Audio\Sound.cpp Audio\Music.cpp Audio\SoundFile.cpp Audio\SoundStream.cpp Audio\SoundBufferRecorder.cpp Audio\Listener.cpp Graphics\RectangleShape.cpp Graphics\VertexArray.cpp Graphics\Shader.cpp Graphics\ConvexShape.cpp Graphics\ImageLoader.cpp Graphics\Sprite.cpp Graphics\RenderTexture.cpp Graphics\BlendMode.cpp Graphics\Shape.cpp Graphics\CircleShape.cpp Graphics\TextureSaver.cpp Graphics\Vertex.cpp Graphics\RenderTextureImpl.cpp Graphics\Texture.cpp Graphics\Text.cpp Graphics\GLExtensions.cpp Graphics\Image.cpp Graphics\RenderTextureImplFBO.cpp Graphics\GLCheck.cpp Graphics\RenderTextureImplDefault.cpp Graphics\Color.cpp Graphics\Transformable.cpp Graphics\RenderTarget.cpp Graphics\Transform.cpp Graphics\View.cpp Graphics\RenderStates.cpp Graphics\RenderWindow.cpp Graphics\Font.cpp Window\JoystickManager.cpp Window\Joystick.cpp Window\Window.cpp Window\Win32\JoystickImpl.cpp Window\Win32\WindowImplWin32.cpp Window\Win32\WglContext.cpp Window\Win32\VideoModeImpl.cpp Window\Win32\InputImpl.cpp Window\Keyboard.cpp Window\GlResource.cpp Window\VideoMode.cpp Window\Mouse.cpp Window\GlContext.cpp Window\Context.cpp Window\WindowImpl.cpp Network\Ftp.cpp Network\TcpListener.cpp Network\Win32\SocketImpl.cpp Network\Packet.cpp Network\IpAddress.cpp Network\TcpSocket.cpp Network\Socket.cpp Network\UdpSocket.cpp Network\SocketSelector.cpp Network\Http.cpp Framework\InputMap.cpp Framework\StateManager.cpp Framework\ResourceManager.cpp Framework\Game.cpp
OBJECTS	= $(addprefix $(OBJPATH)\,$(SOURCES:.cpp=.o))


################################################################
##                     Build Targets                          ##
################################################################

# Default build target builds dll
all: SHARED

# Builds a shared library
SHARED: $(SOURCES) ENSUREDIR
	$(CC) $(OBJECTS) $(LDFLAGS) -shared -o $(BINPATH)\libTyrant$(ARCH).dll

# Builds a static library
STATIC: $(SOURCES) ENSUREDIR
	ar rcs $(BINPATH)\libTyrant$(ARCH).a $(OBJECTS)

# Compiles individual source files into object files
$(SOURCES): ENSUREDIR
	$(CC) $(CFLAGS) -c $(SRCPATH)\$@ -o $(patsubst %.cpp,%.o,$(OBJPATH)\$@)

# Ensures the existence of the directory structure
ENSUREDIR:
	if not exist $(BINPATH) md $(BINPATH)
	if not exist $(OBJPATH) md $(OBJPATH)
	if not exist $(OBJPATH)\Audio md $(OBJPATH)\Audio
	if not exist $(OBJPATH)\Graphics md $(OBJPATH)\Graphics
	if not exist $(OBJPATH)\Network\Win32 md $(OBJPATH)\Network\Win32
	if not exist $(OBJPATH)\System\Win32 md $(OBJPATH)\System\Win32
	if not exist $(OBJPATH)\Window\Win32 md $(OBJPATH)\Window\Win32
	if not exist $(OBJPATH)\Framework md $(OBJPATH)\Framework

# Deletes object files
clean:
	rd /Q /S .\obj
	rd /Q /S .\bin
