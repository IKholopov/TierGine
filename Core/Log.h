#pragma once

#include <TierGine.CoreDefs.h>
#include <ostream>

namespace TierGine {

interface ILogger {
    virtual ~ILogger() {}
    virtual std::ostream& Write(const std::string& input) = 0;
};


class Log {
public:
    enum T_LogLevel {
        LL_Info = BIT(1),
        LL_Warning = BIT(2),
        LL_Error = BIT(3),
        LL_All = LL_Info | LL_Warning | LL_Error
    };

    Log(ILogger* logger, T_LogLevel logLevel);
    ~Log();

    static Log& Info();
    static Log& Warning();
    static Log& Error();

    std::ostream& operator<<(const std::string& input);

private:
    class SwitchableLog {
    public:
        static std::unique_ptr<SwitchableLog> Info;
        static std::unique_ptr<SwitchableLog> Warning;
        static std::unique_ptr<SwitchableLog> Error;

        SwitchableLog(Log* current, SwitchableLog* previous) :
            current(current),
            previous(previous)
        {}

        Log* GetLog() { return current; }

        static void Add(std::unique_ptr<SwitchableLog>& holder, Log* current);
        static void Remove(std::unique_ptr<SwitchableLog>& holder);

    private:
        Log* current;
        std::unique_ptr<SwitchableLog> previous;
    };

    T_LogLevel logLevel;
    std::unique_ptr<ILogger> logger;

};

}
