/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Log.hpp>
#include <streambuf>
#include <cstdio>
#include <time.h>


namespace
{
// Get current date/time for log file name, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buffer[80];
    tstruct = *localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", &tstruct);

    return buffer;
}

// This class will be used as the default streambuf of TGE::Log,
// it outputs to stderr by default (to keep the default behaviour)
class DefaultLogStreamBuf : public std::streambuf
{
public :

    DefaultLogStreamBuf()
    {
    	// Set log file name
    	fileName = currentDateTime() + ".log";

        // Allocate the write buffer
        static const int size = 64;
        char* buffer = new char[size];
        setp(buffer, buffer + size);
    }

    ~DefaultLogStreamBuf()
    {
        // Synchronize
        sync();

        // Delete the write buffer
        delete[] pbase();
    }

private :

    virtual int overflow(int character)
    {
        if ((character != EOF) && (pptr() != epptr()))
        {
            // Valid character
            return sputc(static_cast<char>(character));
        }
        else if (character != EOF)
        {
            // Not enough space in the buffer: synchronize output and try again
            sync();
            return overflow(character);
        }
        else
        {
            // Invalid character: synchronize output
            return sync();
        }
    }

    virtual int sync()
    {
		std::size_t size = static_cast<int>(pptr() - pbase());

		#if defined(TGE_DEBUG)
			fwrite(pbase(), 1, size, stderr);
		#endif

		FILE* logFile = fopen(fileName.c_str(), "a");
		fwrite(pbase(), 1, size, logFile);
		fclose(logFile);

		setp(pbase(), epptr());


        return 0;
    }

    std::string fileName;
};
}

namespace TGE
{
////////////////////////////////////////////////////////////
std::ostream& Log()
{
    static DefaultLogStreamBuf buffer;
    static std::ostream stream(&buffer);

    return stream;
}


} // namespace TGE
