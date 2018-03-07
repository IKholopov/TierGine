#include <Tensor.h>

namespace TierGine {

Tensor::Tensor(int size, char channels, std::shared_ptr<TierGine::Tensor::DataHolder> data) :
    data(data),
    size(size),
    channels(channels)
{
}

}
