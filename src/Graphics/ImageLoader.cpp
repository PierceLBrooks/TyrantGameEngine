/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/ImageLoader.hpp>
#include <Tyrant/System/InputStream.hpp>
#include <Tyrant/System/Log.hpp>
#include <Tyrant/Graphics/stb_image/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Tyrant/Graphics/stb_image/stb_image_write.h>
extern "C"
{
    #include <jpeglib.h>
    #include <jerror.h>
}
#include <cctype>


namespace
{
    // Convert a string to lower case
    std::string toLower(std::string str)
    {
        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            *i = static_cast<char>(std::tolower(*i));
        return str;
    }

    // stb_image callbacks that operate on a TGE::InputStream
    int read(void* user, char* data, int size)
    {
        TGE::InputStream* stream = static_cast<TGE::InputStream*>(user);
        return static_cast<int>(stream->read(data, size));
    }
    void skip(void* user, unsigned int size)
    {
        TGE::InputStream* stream = static_cast<TGE::InputStream*>(user);
        stream->seek(stream->tell() + size);
    }
    int eof(void* user)
    {
        TGE::InputStream* stream = static_cast<TGE::InputStream*>(user);
        return stream->tell() >= stream->getSize();
    }
}


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
ImageLoader& ImageLoader::getInstance()
{
    static ImageLoader Instance;

    return Instance;
}


////////////////////////////////////////////////////////////
ImageLoader::ImageLoader()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
ImageLoader::~ImageLoader()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
bool ImageLoader::loadImageFromFile(const std::string& filename, std::vector<Uint8>& pixels, Vector2u& size)
{
    // Clear the array (just in case)
    pixels.clear();

    // Load the image and get a pointer to the pixels in memory
    int width, height, channels;
    unsigned char* ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (ptr && width && height)
    {
        // Assign the image properties
        size.x = width;
        size.y = height;

        // Copy the loaded pixels to the pixel buffer
        pixels.resize(width * height * 4);
        memcpy(&pixels[0], ptr, pixels.size());

        // Free the loaded pixels (they are now in our own pixel buffer)
        stbi_image_free(ptr);

        return true;
    }
    else
    {
        // Error, failed to load the image
        Log() << "Failed to load image \"" << filename << "\". Reason : " << stbi_failure_reason() << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
bool ImageLoader::loadImageFromMemory(const void* data, std::size_t dataSize, std::vector<Uint8>& pixels, Vector2u& size)
{
    // Check input parameters
    if (data && dataSize)
    {
        // Clear the array (just in case)
        pixels.clear();

        // Load the image and get a pointer to the pixels in memory
        int width, height, channels;
        const unsigned char* buffer = static_cast<const unsigned char*>(data);
        unsigned char* ptr = stbi_load_from_memory(buffer, static_cast<int>(dataSize), &width, &height, &channels, STBI_rgb_alpha);

        if (ptr && width && height)
        {
            // Assign the image properties
            size.x = width;
            size.y = height;

            // Copy the loaded pixels to the pixel buffer
            pixels.resize(width * height * 4);
            memcpy(&pixels[0], ptr, pixels.size());

            // Free the loaded pixels (they are now in our own pixel buffer)
            stbi_image_free(ptr);

            return true;
        }
        else
        {
            // Error, failed to load the image
            Log() << "Failed to load image from memory. Reason : " << stbi_failure_reason() << std::endl;

            return false;
        }
    }
    else
    {
        Log() << "Failed to load image from memory, no data provided" << std::endl;
        return false;
    }
}


////////////////////////////////////////////////////////////
bool ImageLoader::loadImageFromStream(InputStream& stream, std::vector<Uint8>& pixels, Vector2u& size)
{
    // Clear the array (just in case)
    pixels.clear();

    // Make sure that the stream's reading position is at the beginning
    stream.seek(0);

    // Setup the stb_image callbacks
    stbi_io_callbacks callbacks;
    callbacks.read = &read;
    callbacks.skip = &skip;
    callbacks.eof  = &eof;

    // Load the image and get a pointer to the pixels in memory
    int width, height, channels;
    unsigned char* ptr = stbi_load_from_callbacks(&callbacks, &stream, &width, &height, &channels, STBI_rgb_alpha);

    if (ptr && width && height)
    {
        // Assign the image properties
        size.x = width;
        size.y = height;

        // Copy the loaded pixels to the pixel buffer
        pixels.resize(width * height * 4);
        memcpy(&pixels[0], ptr, pixels.size());

        // Free the loaded pixels (they are now in our own pixel buffer)
        stbi_image_free(ptr);

        return true;
    }
    else
    {
        // Error, failed to load the image
        Log() << "Failed to load image from stream. Reason : " << stbi_failure_reason() << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
bool ImageLoader::saveImageToFile(const std::string& filename, const std::vector<Uint8>& pixels, const Vector2u& size)
{
    // Make sure the image is not empty
    if (!pixels.empty() && (size.x > 0) && (size.y > 0))
    {
        // Deduce the image type from its extension
        if (filename.size() > 3)
        {
            // Extract the extension
            std::string extension = toLower(filename.substr(filename.size() - 3));

            if (extension == "bmp")
            {
                // BMP format
                if (stbi_write_bmp(filename.c_str(), size.x, size.y, 4, &pixels[0]))
                    return true;
            }
            else if (extension == "tga")
            {
                // TGA format
                if (stbi_write_tga(filename.c_str(), size.x, size.y, 4, &pixels[0]))
                    return true;
            }
            else if (extension == "png")
            {
                // PNG format
                if (stbi_write_png(filename.c_str(), size.x, size.y, 4, &pixels[0], 0))
                    return true;
            }
            else if (extension == "jpg")
            {
                // JPG format
                if (writeJpg(filename, pixels, size.x, size.y))
                    return true;
            }
        }
    }

    Log() << "Failed to save image \"" << filename << "\"" << std::endl;
    return false;
}


////////////////////////////////////////////////////////////
bool ImageLoader::writeJpg(const std::string& filename, const std::vector<Uint8>& pixels, unsigned int width, unsigned int height)
{
    // Open the file to write in
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file)
        return false;

    // Initialize the error handler
    jpeg_compress_struct compressInfos;
    jpeg_error_mgr errorManager;
    compressInfos.err = jpeg_std_error(&errorManager);

    // Initialize all the writing and compression infos
    jpeg_create_compress(&compressInfos);
    compressInfos.image_width      = width;
    compressInfos.image_height     = height;
    compressInfos.input_components = 3;
    compressInfos.in_color_space   = JCS_RGB;
    jpeg_stdio_dest(&compressInfos, file);
    jpeg_set_defaults(&compressInfos);
    jpeg_set_quality(&compressInfos, 90, TRUE);

    // Get rid of the aplha channel
    std::vector<Uint8> buffer(width * height * 3);
    for (std::size_t i = 0; i < width * height; ++i)
    {
        buffer[i * 3 + 0] = pixels[i * 4 + 0];
        buffer[i * 3 + 1] = pixels[i * 4 + 1];
        buffer[i * 3 + 2] = pixels[i * 4 + 2];
    }
    Uint8* ptr = &buffer[0];

    // Start compression
    jpeg_start_compress(&compressInfos, TRUE);

    // Write each row of the image
    while (compressInfos.next_scanline < compressInfos.image_height)
    {
        JSAMPROW rawPointer = ptr + (compressInfos.next_scanline * width * 3);
        jpeg_write_scanlines(&compressInfos, &rawPointer, 1);
    }

    // Finish compression
    jpeg_finish_compress(&compressInfos);
    jpeg_destroy_compress(&compressInfos);

    // Close the file
    fclose(file);

    return true;
}

} // namespace priv

} // namespace TGE
