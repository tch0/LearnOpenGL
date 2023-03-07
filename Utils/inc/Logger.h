#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <source_location>

using namespace std::string_literals; // for anyone who use Logger

namespace Utils
{

class Logger
{
public:
    enum LogLevel
    {
        Trace,              // the most detailed informations, usually do not use, for detailed tracing.
        Debug,              // general debug output.
        Info,               // usual informations.
        Warning,            // warning, maybe something wrong but little problem, maybe not.
        Error,              // something is definitely wrong, the program can be continued.
        Fatal,              // something is definitely wrong, better to kill the program.
        FinalLevel = 100    // no meaning, for comparing, do not use.
    };

    Logger(std::ostream& os = std::cout);
    ~Logger();
    // set the lowest output log level, default to Info.
    void setLowestOutputLevel(LogLevel level);
    // block specific log level to output, for higher level than output log level, default to none.
    void blockLevel(LogLevel level);
    
    // log functions
    void trace(const std::string& str, std::source_location loc = std::source_location::current());
    void debug(const std::string& str, std::source_location loc = std::source_location::current());
    void info(const std::string& str, std::source_location loc = std::source_location::current());
    void warning(const std::string& str, std::source_location loc = std::source_location::current());
    void error(const std::string& str, std::source_location loc = std::source_location::current());
    void fatal(const std::string& str, std::source_location loc = std::source_location::current());

    // a global logger to use
    static Logger& globalLogger();
    static Logger& defaultLogger();
    static void setLogger(Logger* pLogger);
private:
    std::ostream& m_Out;
    bool m_bBlockTrace = false;
    bool m_bBlockDebug = false;
    bool m_bBlockInfo = false;
    bool m_bBlockWarning = false;
    bool m_bBlockError = false;
    bool m_bBlockFatal = false;
    LogLevel m_LowestLevel = Info;

    inline static Logger* s_pGlobalLogger = nullptr;

    // help function
    // strip file path in file name, only keep file name itself
    static std::string stripFilePath(const char* file);
};

} // namespace Utils