/*
   Copyright 2018 Igor Kholopov, All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   ==============================================================================
*/

#pragma once

#include <TierGine.CoreDefs.h>

#include <assert.h>

namespace TierGine {

class Tensor {
public:
    enum Type { T_BOOL, T_BYTE, T_UBYTE, T_SHORT, T_USHORT, T_INT, T_UINT, T_FLOAT, T_DOUBLE };
    interface DataHolder {
        virtual ~DataHolder() {}

        virtual const void* GetRawAddress() = 0;
        virtual int GetTypeSize() const = 0;
        virtual int GetLength() const = 0;
        virtual Type GetType() const = 0;
    };

    Tensor(int size, char channels, std::shared_ptr<DataHolder> data);
    Tensor(const Tensor& other) = default;
    Tensor& operator=(const Tensor& other) = default;
    Tensor(Tensor&& other) = default;
    Tensor& operator=(Tensor&& other) = default;

    const void* GetRawPointer() const { return data->GetRawAddress(); }
    int GetSize() const { return size; }
    char GetChannels() const { return channels; }
    Type GetType() const { return data->GetType(); }

private:
    std::shared_ptr<DataHolder> data;
    const int size;
    const char channels;
};


template <typename T>
class TensorData : public Tensor::DataHolder {
public:
    TensorData(int length, bool owner = true) :
        length(length),
        owner(owner)
    { allocData(); }

    TensorData(T* data, int length, bool owner = true) :
        length(length),
        data(data),
        owner(owner)
    { assert(data != nullptr); }
    ~TensorData() { if(owner) { delete data;  } }

    // DataHolder interface
    virtual const void* GetRawAddress() override { return static_cast<const void*>(data); }
    virtual int GetTypeSize() const override { return sizeof(T); }
    virtual int GetLength() const override { return length; }
    virtual Tensor::Type GetType() const override;

private:
    const int length;
    T* data;
    bool owner;

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

template <typename T>
Tensor CreateTensor(const T& otherImplementation);

template<typename T>
void TensorData<T>::allocData()
{
    assert(length > 0);
    data = new T[length];
}


}
