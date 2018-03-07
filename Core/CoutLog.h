#include <iostream>

#include <Log.h>
#include <Context.h>

namespace TierGine {

class CoutLogger : public ILogger {
public:
    enum T_LogColor {
        LC_None = 0,
        LC_Red = 31,
        LC_Green = 32,
        LC_Yellow = 33,
        LC_Cyan = 36
    };

    CoutLogger(T_LogColor color = LC_None) : color(color) {}

    virtual std::ostream& Write(const std::string& input) override
        { return std::cout << "\033[" << color << "m" << input << "\033[" << LC_None << "m"; }

private:
    T_LogColor color;
};

}
