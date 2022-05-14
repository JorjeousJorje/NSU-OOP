#pragma once
#include <vector>

class square_matrix
{
    class array_proxy
    {
    public:
        explicit array_proxy() = default;

        float& operator[] (const std::size_t index) {
            return array_[index];
        }

        float operator[] (const std::size_t index) const {
            return array_[index];
        }

    	void resize(const std::size_t size) {
            array_.resize(size);
        }
    	
    private:
       std::vector<float> array_{};
    };
public:
    explicit square_matrix(const std::size_t size) {
        resize(size);
    }

	void resize(const std::size_t size)
    {
        array_matrix_.resize(size);
        for (auto& array : array_matrix_) {
            array.resize(size);
        }
    }

    array_proxy operator[](const std::size_t index) const {
        return array_matrix_[index];
    }

    array_proxy& operator[](const std::size_t index) {
        return array_matrix_[index];
    }

private:
    std::vector<array_proxy> array_matrix_{};
};