#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// Gets created under the current directory
#define LOG_FILE_NAME   "log.txt"

namespace Logger
{
    // base case
    void Format(const char *s, std::string & output)
    {
        if (*s == '\0' || !s )
           return;

        output += std::string(s);
    }

    template<typename T, typename ... Args>
    void Format(const char * fmt, std::string & output, T value, Args ... args)
    {
        while (*fmt != '\0')
        {
            char ch = *fmt;
            // Current char is not % 
            // or % followed by % ex - "ABC%%gh" 
            // or ending with last % ex "ABC%"
            if ( (ch != '%') || 
                 (ch == '%' && *(fmt + 1) == '%') ||
                 (ch == '%' && *(fmt + 1) == '\0') 
                 )
            {
                output += ch;
                fmt++;
                continue;
            }
            // Valid format specifier - supports basic formatting %d, %s and such.
            // TODO: Conversion of value to actual format specifier.
   			std::stringstream stream;
			stream << value;
            output += stream.str();
            fmt += 2;
            Format(fmt, output, args...);
            break; 
        }
    }

    void LogToFile(std::string & s)
    {
        std::ofstream logfile;
        logfile.open (LOG_FILE_NAME, std::ios::app);
        logfile << s << "\n";
        logfile.close();
    }

    void Log(const char *s)
    {
        if (*s == '\0' || !s )
           return;

        std::string  str(s);
        LogToFile( str );
    }

    // Logs to console and file ( convenience expecially when
    // it gets huge ).
	// Eventually there can be configuration parameters
	// to control where to log.
    template< typename T, typename ... Args >
    void Log(const char * fmt, T value, Args ... args)
    {
        std::string output;
        Format(fmt, output, value, args ...);
        LogToFile( output );
    }   
}