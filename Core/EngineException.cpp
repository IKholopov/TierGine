#include <EngineException.h>

namespace TierGine {

char const* EngineException::what() const noexcept
{
    return errorMessage.c_str();
}

}
