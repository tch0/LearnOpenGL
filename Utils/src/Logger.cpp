#include <Logger.h>
#include <algorithm>
#include <iomanip>

namespace Utils
{

Logger::Logger(std::ostream& os)
    : m_Out(os)
{
}

Logger::~Logger()
{
    if (this == s_pGlobalLogger)
    {
        s_pGlobalLogger = nullptr; // reset global logger to nullptr if current logger is global logger.
    }
}

// set the lowest output log level, default to Info.
void Logger::setLowestOutputLevel(LogLevel level)
{
    m_LowestLevel = level;
}

// block specific log level to output, for higher level than output log level, default to none.
void Logger::blockLevel(LogLevel level)
{
    switch (level)
    {
    case Trace:
        m_bBlockTrace = true;
        break;
    case Debug:
        m_bBlockDebug = true;
        break;
    case Info:
        m_bBlockInfo = true;
        break;
    case Warning:
        m_bBlockWarning = true;
        break;
    case Error:
        m_bBlockError = true;
        break;
    case Fatal:
        m_bBlockFatal = true;
        break;
    }
}

// log functions
void Logger::trace(const std::string& str, std::source_location loc)
{
    if (!m_bBlockTrace && m_LowestLevel <= Trace)
    {
        m_Out << "[ Trace   ][ " << std::setw(20) << std::right << stripFilePath(loc.file_name()) << " : " << std::setw(4) << std::right << loc.line() 
            << " : " << std::setw(30) << std::left << loc.function_name() <<  " ]: " << str << std::endl;
    }
}
void Logger::debug(const std::string& str, std::source_location loc)
{
    if (!m_bBlockDebug && m_LowestLevel <= Debug)
    {
        m_Out << "[ Debug   ][ " << std::setw(20) << std::right << stripFilePath(loc.file_name()) << " : " << std::setw(4) << std::right << loc.line() 
            << " : " << std::setw(30) << std::left << loc.function_name() <<  " ]: " << str << std::endl;
    }
}
void Logger::info(const std::string& str, std::source_location loc)
{
    if (!m_bBlockInfo && m_LowestLevel <= Info)
    {
        m_Out << "[ Info    ][ " << std::setw(20) << std::right << stripFilePath(loc.file_name()) << " : " << std::setw(4) << std::right << loc.line() 
            << " : " << std::setw(30) << std::left << loc.function_name() <<  " ]: " << str << std::endl;
    }
}
void Logger::warning(const std::string& str, std::source_location loc)
{
    if (!m_bBlockWarning && m_LowestLevel <= Warning)
    {
        m_Out << "[ Warning ][ " << std::setw(20) << std::right << stripFilePath(loc.file_name()) << " : " << std::setw(4) << std::right << loc.line() 
            << " : " << std::setw(30) << std::left << loc.function_name() <<  " ]: " << str << std::endl;
    }
}
void Logger::error(const std::string& str, std::source_location loc)
{
    if (!m_bBlockError && m_LowestLevel <= Error)
    {
        m_Out << "[ Error   ][ " << std::setw(20) << std::right << stripFilePath(loc.file_name()) << " : " << std::setw(4) << std::right << loc.line() 
            << " : " << std::setw(30) << std::left << loc.function_name() <<  " ]: " << str << std::endl;
    }
}
void Logger::fatal(const std::string& str, std::source_location loc)
{
    if (!m_bBlockFatal && m_LowestLevel <= Fatal)
    {
        m_Out << "[ Fatal   ][ " << std::setw(20) << std::right << stripFilePath(loc.file_name()) << " : " << std::setw(4) << std::right << loc.line() 
            << " : " << std::setw(30) << std::left << loc.function_name() <<  " ]: " << str << std::endl;
    }
}

// a global logger to use
Logger& Logger::globalLogger()
{
    if (s_pGlobalLogger)
    {
        return *s_pGlobalLogger;
    }
    return defaultLogger();
}

Logger& Logger::defaultLogger()
{
    static Logger coutLogger(std::cout);
    return coutLogger;
}
void Logger::setLogger(Logger* pLogger)
{
    s_pGlobalLogger = pLogger;
}

std::string Logger::stripFilePath(const char* file)
{
    std::string str(file);
    for (auto rit = str.rbegin(); rit != str.rend(); rit++)
    {
        if (*rit == '/' || *rit == '\\')
        {
            return std::string(rit.base(), str.end());
        }
    }
    return str;
}

} // namespace Utils