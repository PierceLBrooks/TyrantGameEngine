/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_IMAGELOADER_HPP
#define TGE_IMAGELOADER_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/NonCopyable.hpp>
#include <Tyrant/System/Vector2.hpp>
#include <string>
#include <vector>


namespace TGE
{
class InputStream;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Load/save image files
///
////////////////////////////////////////////////////////////
class ImageLoader : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the unique instance of the class
    ///
    /// \return Reference to the ImageLoader instance
    ///
    ////////////////////////////////////////////////////////////
    static ImageLoader& getInstance();

    ////////////////////////////////////////////////////////////
    /// \brief Load an image from a file on disk
    ///
    /// \param filename Path of image file to load
    /// \param pixels   Array of pixels to fill with loaded image
    /// \param size     Size of loaded image, in pixels
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool loadImageFromFile(const std::string& filename, std::vector<Uint8>& pixels, Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Load an image from a file in memory
    ///
    /// \param data     Pointer to the file data in memory
    /// \param dataSize Size of the data to load, in bytes
    /// \param pixels   Array of pixels to fill with loaded image
    /// \param size     Size of loaded image, in pixels
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool loadImageFromMemory(const void* data, std::size_t dataSize, std::vector<Uint8>& pixels, Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Load an image from a custom stream
    ///
    /// \param stream Source stream to read from
    /// \param pixels Array of pixels to fill with loaded image
    /// \param size   Size of loaded image, in pixels
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool loadImageFromStream(InputStream& stream, std::vector<Uint8>& pixels, Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Save an array of pixels as an image file
    ///
    /// \param filename Path of image file to save
    /// \param pixels   Array of pixels to save to image
    /// \param size     Size of image to save, in pixels
    ///
    /// \return True if saving was successful
    ///
    ////////////////////////////////////////////////////////////
    bool saveImageToFile(const std::string& filename, const std::vector<Uint8>& pixels, const Vector2u& size);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ImageLoader();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ImageLoader();

    ////////////////////////////////////////////////////////////
    /// \brief Save an image file in JPEG format
    ///
    /// \param filename Path of image file to save
    /// \param pixels   Array of pixels to save to image
    /// \param width    Width of image to save, in pixels
    /// \param height   Height of image to save, in pixels
    ///
    /// \return True if saving was successful
    ///
    ////////////////////////////////////////////////////////////
    bool writeJpg(const std::string& filename, const std::vector<Uint8>& pixels, unsigned int width, unsigned int height);
};

} // namespace priv

} // namespace TGE


#endif // TGE_IMAGELOADER_HPP
