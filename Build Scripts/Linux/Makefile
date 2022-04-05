################################################################
#                  Tyrant Game Engine Makefile                 #
#                Copyright © 2014 Coldsnap Games               #
################################################################



################################################################
##                         Variables                          ##
################################################################

# Default variables, you probably don't want/need to change these
# CC - The compiler
# INC - The include directories
# CFLAGS - Compiler flags to use
# LDFLAGS - Linker flags to use
# GMON_OUT_PREFIX - Sets the name of the profiler data file to be used with gprof when compiling in Debug
CC	 = g++
INC	 = -I../../include -I../../extlibs/headers -I../../extlibs/headers/jpeg -I../../extlibs/headers/libfreetype/linux -I../../extlibs/headers/libsndfile/linux
CFLAGS	 = $(INC) -std=c++0x -fPIC -pthread -DBUILDING_TYRANT
LDFLAGS	 = -lX11 -lGL -lGLEW -lfreetype -ljpeg -lopenal -lsndfile
GMON_OUT_PREFIX = TGE_PROFILER_DATA


# Build specific variables, if need be define these when running make
# BUILD - Either Debug or Release, defaults to Release
# ARCH - Either 32 or 64, defaults to OS type
BUILD ?= RELEASE

ifeq ($(shell uname -p),x86_64)
	ARCH ?= 64
else
	ARCH ?= 32
endif

ifeq ($(BUILD),DEBUG)
	CFLAGS  := $(CFLAGS) -m$(ARCH) -g -pg -Og
	LDFLAGS := $(LDFLAGS) -m$(ARCH)
endif

ifeq ($(BUILD),RELEASE)
	CFLAGS  := $(CFLAGS) -m$(ARCH) -s -O3
	LDFLAGS := $(LDFLAGS) -m$(ARCH)
endif


# Directory variables, no need to change these
# SRCPATH - The directory for source files
# OBJDIR - Directory path for .o files
# BINPATH - Where to put the built library
SRCPATH	= ../../src/
OBJDIR	= ./obj/$(BUILD)/$(ARCH)-bit
BINPATH	= ./bin/$(BUILD)/$(ARCH)-bit


# File variables, should only need to change when adding source files
# SRC_SYSTEM - Path to files in the System module
# SRC_GRAPHICS - Path to files in the GRAPHICS module
# SRC_NETWORK - Path to files in the Network module
# SRC_WINDOW - Path to files in the Window module
# SRC_AUDIO - Path to files in the Audio module
# SRC_FRAMEWORK - Path to files in the Framework module
# SOURCES - Path to all source files
# OBJECTS - Path to output individual object files
SRC_SYSTEM = System/Time.cpp System/Mutex.cpp System/Log.cpp System/Clock.cpp System/Sleep.cpp System/Unix/ClockImpl.cpp System/Unix/MutexImpl.cpp System/Unix/SleepImpl.cpp System/Unix/ThreadImpl.cpp System/Unix/ThreadLocalImpl.cpp System/Lock.cpp System/String.cpp System/ThreadLocal.cpp System/Thread.cpp
SRC_GRAPHICS = Graphics/RectangleShape.cpp Graphics/VertexArray.cpp Graphics/Shader.cpp Graphics/ConvexShape.cpp Graphics/ImageLoader.cpp Graphics/Sprite.cpp Graphics/RenderTexture.cpp Graphics/BlendMode.cpp Graphics/Shape.cpp Graphics/CircleShape.cpp Graphics/TextureSaver.cpp Graphics/Vertex.cpp Graphics/RenderTextureImpl.cpp Graphics/Texture.cpp Graphics/Text.cpp Graphics/GLExtensions.cpp Graphics/Image.cpp Graphics/RenderTextureImplFBO.cpp Graphics/GLCheck.cpp Graphics/RenderTextureImplDefault.cpp Graphics/Color.cpp Graphics/Transformable.cpp Graphics/RenderTarget.cpp Graphics/Transform.cpp Graphics/View.cpp Graphics/RenderStates.cpp Graphics/RenderWindow.cpp Graphics/Font.cpp
SRC_NETWORK = Network/Ftp.cpp Network/TcpListener.cpp Network/Packet.cpp Network/IpAddress.cpp Network/TcpSocket.cpp Network/Socket.cpp Network/Unix/SocketImpl.cpp Network/UdpSocket.cpp Network/SocketSelector.cpp Network/Http.cpp
SRC_WINDOW = Window/JoystickManager.cpp Window/Joystick.cpp Window/Window.cpp Window/Keyboard.cpp Window/GlResource.cpp Window/Unix/JoystickImpl.cpp Window/Unix/WindowImplX11.cpp Window/Unix/GlxContext.cpp Window/Unix/Display.cpp Window/Unix/VideoModeImpl.cpp Window/Unix/InputImpl.cpp Window/VideoMode.cpp Window/Mouse.cpp Window/GlContext.cpp Window/Context.cpp Window/WindowImpl.cpp
SRC_AUDIO = Audio/SoundRecorder.cpp Audio/SoundBuffer.cpp Audio/SoundSource.cpp Audio/AudioDevice.cpp Audio/ALCheck.cpp Audio/Sound.cpp Audio/Music.cpp Audio/SoundFile.cpp Audio/SoundStream.cpp Audio/SoundBufferRecorder.cpp Audio/Listener.cpp
SRC_FRAMEWORK = Framework/Game.cpp Framework/InputMap.cpp Framework/StateManager.cpp Framework/ResourceManager.cpp
SOURCES	= $(SRC_SYSTEM) $(SRC_GRAPHICS) $(SRC_NETWORK) $(SRC_WINDOW) $(SRC_AUDIO) $(SRC_FRAMEWORK)
OBJECTS	= $(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))


################################################################
##                     Build Targets                          ##
################################################################

# Default build target is for a shared library
all: SHARED

# Builds a shared library
SHARED: $(addprefix $(SRCPATH),$(SOURCES)) $(SOURCES) ENSUREDIR
	$(CC) $(LDFLAGS) $(OBJECTS) -shared -o $(BINPATH)/libTyrant$(ARCH).so

# Builds a static library
STATIC: $(addprefix $(SRCPATH),$(SOURCES)) $(SOURCES) ENSUREDIR
	ar rcs $(BINPATH)/libTyrant$(ARCH).a $(OBJECTS)

# Compiles individual source files into object files
$(SOURCES): ENSUREDIR
	$(CC) $(CFLAGS) -c $(SRCPATH)$@ -o $(patsubst %.cpp,%.o,$(OBJDIR)/$@)

# Ensures the existance of the directory structure
ENSUREDIR:
	mkdir -p $(BINPATH)
	mkdir -p $(OBJDIR)/Audio
	mkdir -p $(OBJDIR)/Graphics
	mkdir -p $(OBJDIR)/Network/Unix
	mkdir -p $(OBJDIR)/System/Unix
	mkdir -p $(OBJDIR)/Window/Unix
	mkdir -p $(OBJDIR)/Framework

# Deletes object files
clean:
	rm -rf ./obj
	rm -rf ./bin

