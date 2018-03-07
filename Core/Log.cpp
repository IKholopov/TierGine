#include <Log.h>

#include <assert.h>

namespace TierGine {

Log::Log(ILogger* logger, T_LogLevel logLevel) :
    logLevel(logLevel),
    logger(logger)
{
    if( logLevel & LL_Info ) {
        SwitchableLog::Add(SwitchableLog::Info, this);
    }
    if( logLevel & LL_Warning ) {
        SwitchableLog::Add(SwitchableLog::Warning, this);
    }
    if( logLevel & LL_Error ) {
        SwitchableLog::Add(SwitchableLog::Error, this);
    }
}

Log::~Log() {
    if( logLevel & LL_Info ) {
        SwitchableLog::Remove(SwitchableLog::Info);
    }
    if( logLevel & LL_Warning ) {
        SwitchableLog::Remove(SwitchableLog::Warning);
    }
    if( logLevel & LL_Error ) {
        SwitchableLog::Remove(SwitchableLog::Error);
    }
}

Log& Log::Info()
{
    assert(SwitchableLog::Info != nullptr);
    return *SwitchableLog::Info->GetLog();
}

Log& Log::Warning()
{
    assert(SwitchableLog::Warning != nullptr);
    return *SwitchableLog::Warning->GetLog();
}

Log& Log::Error()
{
    assert(SwitchableLog::Error != nullptr);
    return *SwitchableLog::Error->GetLog();
}

std::ostream& Log::operator<<(const std::string& input)
{
    return logger->Write(input);
}

void Log::SwitchableLog::Add(std::unique_ptr<Log::SwitchableLog>& holder,
                             Log* current)
{
    assert(current != nullptr);
    holder.reset(new SwitchableLog(current, holder.release()));
}

void Log::SwitchableLog::Remove(std::unique_ptr<Log::SwitchableLog>& holder)
{
    assert(holder.get() != nullptr);
    holder.reset(holder->previous.release());
}

std::unique_ptr<Log::SwitchableLog> Log::SwitchableLog::Info = nullptr;
std::unique_ptr<Log::SwitchableLog> Log::SwitchableLog::Warning = nullptr;
std::unique_ptr<Log::SwitchableLog> Log::SwitchableLog::Error = nullptr;

}
