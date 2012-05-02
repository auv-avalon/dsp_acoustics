#pragma once

#include <cstring>
#include <algorithm> // for std::min

namespace dsp
{
    
template<class T>
class CircularBuffer
{
public:
    CircularBuffer(size_t capacity);
    ~CircularBuffer();

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    // Return number of bytes written.
    size_t write(const T *data, size_t samples);
    // Return number of bytes read.
    size_t read(T *data, size_t samples);

private:
    size_t beg_index_, end_index_, size_, capacity_;
    T *data_;
};

template <class T>
CircularBuffer<T>::CircularBuffer(size_t capacity)
    : beg_index_(0)
    , end_index_(0)
    , size_(0)
    , capacity_(capacity)
{
    data_ = new T[capacity];
}

template <class T>
CircularBuffer<T>::~CircularBuffer()
{
    delete [] data_;
}

template <class T>
size_t CircularBuffer<T>::write(const T *data, size_t samples)
{
    if (samples == 0) return 0;

    size_t capacity = capacity_;
    size_t samples_to_write = std::min(samples, capacity - size_);

    // Write in a single step
    if (samples_to_write <= capacity - end_index_)
    {
        memcpy(data_ + end_index_, data, samples_to_write * sizeof(T));
        end_index_ += samples_to_write;
        if (end_index_ == capacity) end_index_ = 0;
    }
    // Write in two steps
    else
    {
        size_t size_1 = capacity - end_index_;
        memcpy(data_ + end_index_, data, size_1 * sizeof(T));
        size_t size_2 = samples_to_write - size_1;
        memcpy(data_, data + size_1, size_2 * sizeof(T));
        end_index_ = size_2;
    }

    size_ += samples_to_write;
    return samples_to_write;
}

template <class T>
size_t CircularBuffer<T>::read(T *data, size_t samples)
{
    if (samples == 0) return 0;

    size_t capacity = capacity_;
    size_t samples_to_read = std::min(samples, size_);

    // Read in a single step
    if (samples_to_read <= capacity - beg_index_)
    {
        memcpy(data, data_ + beg_index_, samples_to_read * sizeof(T));
        beg_index_ += samples_to_read;
        if (beg_index_ == capacity) beg_index_ = 0;
    }
    // Read in two steps
    else
    {
        size_t size_1 = capacity - beg_index_;
        memcpy(data, data_ + beg_index_, size_1 * sizeof(T));
        size_t size_2 = samples_to_read - size_1;
        memcpy(data + size_1, data_, size_2 * sizeof(T));
        beg_index_ = size_2;
    }

    size_ -= samples_to_read;
    return samples_to_read;
}


}