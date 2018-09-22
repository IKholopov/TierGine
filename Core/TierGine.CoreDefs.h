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
#include <memory>

#define interface struct

#define BIT(n) n == 0 ? 0 : 1 << (n-1)
#define Byte unsigned char

typedef int TG_Status;

#define TG_Ok 0
#define TG_Critical 1

template<typename T>
T min(const T& a, const T& b) { return a < b ? a : b; }

template<typename T>
T max(const T& a, const T& b) { return a < b ? b : a; }

template<typename T>
int sign(const T& a) {
    if(a==0) {
        return 0;
    }
    return a > 0 ? 1 : -1;
}

template<typename T>
class Binder {
public:
    Binder(T& bindable): bindable(bindable) { bindable.Bind(); }
    ~Binder() { bindable.Unbind(); }
private:
    T& bindable;
};

template<typename T, typename U>
T to(const U& u) { return static_cast<T>(u); }
