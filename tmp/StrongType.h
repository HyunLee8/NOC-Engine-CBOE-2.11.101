#pragma once

template<typename T, typename Tag>
struct StrongType {
    explicit StrongType(T v) : value(v) {}
    T value;
};
