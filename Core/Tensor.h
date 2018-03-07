#pragma once

#include <TierGine.CoreDefs.h>

#include <assert.h>

namespace TierGine {

class Tensor {
public:
    interface DataHolder {
        virtual ~DataHolder() {}

        virtual void* GetRawAddress() = 0;
        virtual int GetTypeSize() const = 0;
        virtual int GetLength() const = 0;
    };

    Tensor(int size, char channels, std::shared_ptr<DataHolder> data);
    Tensor(const Tensor& other) = default;
    Tensor& operator=(const Tensor& other) = default;
    Tensor(Tensor&& other) = default;
    Tensor& operator=(Tensor&& other) = default;

    void* GetRawPointer() const { return data->GetRawAddress(); }
    int GetSize() const { return size; }
    char GetChannels() const { return channels; }

private:
    std::shared_ptr<DataHolder> data;
    const int size;
    const char channels;
};


template <typename T>
class TensorData : public Tensor::DataHolder {
public:
    TensorData(int length) : length(length) { allocData(); }
    TensorData(T* data, int length) :
        length(length),
        data(data)
            { assert(data != nullptr); }
    ~TensorData() { delete data; }

    // DataHolder interface
    virtual void* GetRawAddress() override { return static_cast<void*>(data); }
    virtual int GetTypeSize() const override { return sizeof(T); }
    virtual int GetLength() const override { return length; }

private:
    const int length;
    T* data;

    void allocData();
};

template <typename T>
Tensor CreateTensor(int size, int channels, T* ptr = nullptr)
{
    if(ptr == nullptr)
    {
        return Tensor(size, channels, std::shared_ptr<Tensor::DataHolder>(new TensorData<T>(size*channels)));
    }
    return Tensor(size, channels, std::shared_ptr<Tensor::DataHolder>(new TensorData<T>(ptr, size*channels)));
}

template <typename T>
Tensor CreateTensor(int size, int channels, std::initializer_list<T> values)
{
    assert(values.size() == channels * size);
    T* ptr = new T[values.size()];
    for(int i = 0; i < values.size(); ++i) {
        ptr[i] = *(values.begin()+i);
    }

    return Tensor(size, channels, std::shared_ptr<Tensor::DataHolder>(new TensorData<T>(ptr, size*channels)));
}


template<typename T>
void TensorData<T>::allocData()
{
    assert(length > 0);
    data = new T[length];
}


}
