/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_IMAGE_HPP
#define TGE_IMAGE_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Graphics/Color.hpp>
#include <Tyrant/Graphics/Rect.hpp>
#include <string>
#include <vector>


namespace TGE
{
class InputStream;

////////////////////////////////////////////////////////////
/// \brief Class for loading, manipulating and saving images
///
////////////////////////////////////////////////////////////
class TGE_API Image
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty image.
    ///
    ////////////////////////////////////////////////////////////
    Image();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Image();

    ////////////////////////////////////////////////////////////
    /// \brief Create the image and fill it with a unique color
    ///
    /// \param width  Width of the image
    /// \param height Height of the image
    /// \param color  Fill color
    ///
    ////////////////////////////////////////////////////////////
    void create(unsigned int width, unsigned int height, const Color& color = Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// \brief Create the image from an array of pixels
    ///
    /// The \a pixel array is assumed to contain 32-bits RGBA pixels,
    /// and have the given \a width and \a height. If not, this is
    /// an undefined behaviour.
    /// If \a pixels is null, an empty image is created.
    ///
    /// \param width  Width of the image
    /// \param height Height of the image
    /// \param pixels Array of pixels to copy to the image
    ///
    ////////////////////////////////////////////////////////////
    void create(unsigned int width, unsigned int height, const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// \brief Load the image from a file on disk
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr and pic. Some format options are not supported,
    /// like progressive jpeg.
    /// If this function fails, the image is left unchanged.
    ///
    /// \param filename Path of the image file to load
    ///
    /// \return True if loading was successful
    ///
    /// \see loadFromMemory, loadFromStream, saveToFile
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Load the image from a file in memory
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr and pic. Some format options are not supported,
    /// like progressive jpeg.
    /// If this function fails, the image is left unchanged.
    ///
    /// \param data Pointer to the file data in memory
    /// \param size Size of the data to load, in bytes
    ///
    /// \return True if loading was successful
    ///
    /// \see loadFromFile, loadFromStream
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromMemory(const void* data, std::size_t size);

    ////////////////////////////////////////////////////////////
    /// \brief Load the image from a custom stream
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr and pic. Some format options are not supported,
    /// like progressive jpeg.
    /// If this function fails, the image is left unchanged.
    ///
    /// \param stream Source stream to read from
    ///
    /// \return True if loading was successful
    ///
    /// \see loadFromFile, loadFromMemory
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromStream(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Save the image to a file on disk
    ///
    /// The format of the image is automatically deduced from
    /// the extension. The supported image formats are bmp, png,
    /// tga and jpg. The destination file is overwritten
    /// if it already exists. This function fails if the image is empty.
    ///
    /// \param filename Path of the file to save
    ///
    /// \return True if saving was successful
    ///
    /// \see create, loadFromFile, loadFromMemory
    ///
    ////////////////////////////////////////////////////////////
    bool saveToFile(const std::string& filename) const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the size (width and height) of the image
    ///
    /// \return Size of the image, in pixels
    ///
    ////////////////////////////////////////////////////////////
    Vector2u getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Create a transparency mask from a specified color-key
    ///
    /// This function sets the alpha value of every pixel matching
    /// the given color to \a alpha (0 by default), so that they
    /// become transparent.
    ///
    /// \param color Color to make transparent
    /// \param alpha Alpha value to assign to transparent pixels
    ///
    ////////////////////////////////////////////////////////////
    void createMaskFromColor(const Color& color, Uint8 alpha = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Copy pixels from another image onto this one
    ///
    /// This function does a slow pixel copy and should not be
    /// used intensively. It can be used to prepare a complex
    /// static image from several others, but if you need this
    /// kind of feature in real-time you'd better use TGE::RenderTexture.
    ///
    /// If \a sourceRect is empty, the whole image is copied.
    /// If \a applyAlpha is set to true, the transparency of
    /// source pixels is applied. If it is false, the pixels are
    /// copied unchanged with their alpha value.
    ///
    /// \param source     Source image to copy
    /// \param destX      X coordinate of the destination position
    /// \param destY      Y coordinate of the destination position
    /// \param sourceRect Sub-rectangle of the source image to copy
    /// \param applyAlpha Should the copy take in account the source transparency?
    ///
    ////////////////////////////////////////////////////////////
    void copy(const Image& source, unsigned int destX, unsigned int destY, const IntRect& sourceRect = IntRect(0, 0, 0, 0), bool applyAlpha = false);

    ////////////////////////////////////////////////////////////
    /// \brief Change the color of a pixel
    ///
    /// This function doesn't check the validity of the pixel
    /// coordinates, using out-of-range values will result in
    /// an undefined behaviour.
    ///
    /// \param x     X coordinate of pixel to change
    /// \param y     Y coordinate of pixel to change
    /// \param color New color of the pixel
    ///
    /// \see getPixel
    ///
    ////////////////////////////////////////////////////////////
    void setPixel(unsigned int x, unsigned int y, const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Get the color of a pixel
    ///
    /// This function doesn't check the validity of the pixel
    /// coordinates, using out-of-range values will result in
    /// an undefined behaviour.
    ///
    /// \param x X coordinate of pixel to get
    /// \param y Y coordinate of pixel to get
    ///
    /// \return Color of the pixel at coordinates (x, y)
    ///
    /// \see setPixel
    ///
    ////////////////////////////////////////////////////////////
    Color getPixel(unsigned int x, unsigned int y) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-only pointer to the array of pixels
    ///
    /// The returned value points to an array of RGBA pixels made of
    /// 8 bits integers components. The size of the array is
    /// width * height * 4 (getSize().x * getSize().y * 4).
    /// Warning: the returned pointer may become invalid if you
    /// modify the image, so you should never store it for too long.
    /// If the image is empty, a null pointer is returned.
    ///
    /// \return Read-only pointer to the array of pixels
    ///
    ////////////////////////////////////////////////////////////
    const Uint8* getPixelsPtr() const;

    ////////////////////////////////////////////////////////////
    /// \brief Flip the image horizontally (left <-> right)
    ///
    ////////////////////////////////////////////////////////////
    void flipHorizontally();

    ////////////////////////////////////////////////////////////
    /// \brief Flip the image vertically (top <-> bottom)
    ///
    ////////////////////////////////////////////////////////////
    void flipVertically();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2u           m_size;   ///< Image size
    std::vector<Uint8> m_pixels; ///< Pixels of the image
};

} // namespace TGE


#endif // TGE_IMAGE_HPP


////////////////////////////////////////////////////////////
/// \class TGE::Image
/// \ingroup graphics
///
/// TGE::Image is an abstraction to manipulate images
/// as bidimensional arrays of pixels. The class provides
/// functions to load, read, write and save pixels, as well
/// as many other useful functions.
///
/// TGE::Image can handle a unique internal representation of
/// pixels, which is RGBA 32 bits. This means that a pixel
/// must be composed of 8 bits red, green, blue and alpha
/// channels -- just like a TGE::Color.
/// All the functions that return an array of pixels follow
/// this rule, and all parameters that you pass to TGE::Image
/// functions (such as loadFromPixels) must use this
/// representation as well.
///
/// A TGE::Image can be copied, but it is a heavy resource and
/// if possible you should always use [const] references to
/// pass or return them to avoid useless copies.
///
/// Usage example:
/// \code
/// // Load an image file from a file
/// TGE::Image background;
/// if (!background.loadFromFile("background.jpg"))
///     return -1;
///
/// // Create a 20x20 image filled with black color
/// TGE::Image image;
/// image.create(20, 20, TGE::Color::Black);
///
/// // Copy image1 on image2 at position (10, 10)
/// image.copy(background, 10, 10);
///
/// // Make the top-left pixel transparent
/// TGE::Color color = image.getPixel(0, 0);
/// color.a = 0;
/// image.setPixel(0, 0, color);
///
/// // Save the image to a file
/// if (!image.saveToFile("result.png"))
///     return -1;
/// \endcode
///
/// \see TGE::Texture
///
////////////////////////////////////////////////////////////