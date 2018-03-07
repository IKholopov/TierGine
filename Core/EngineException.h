#include <string>

namespace TierGine {

class EngineException : public std::exception {
public:
    EngineException(const std::string message) : errorMessage(message) {}
    virtual ~EngineException() throw() {}

    virtual char const* what() const noexcept override;

private:
    std::string errorMessage;
};

}
