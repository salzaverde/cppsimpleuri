// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

namespace salzaverde {
    template<typename T> class CartesianProduct {
    public:
        CartesianProduct(std::vector<std::vector<T>> sequences) {
            std::vector<std::vector<T>> accum;
            std::vector<T> stack;
            if (sequences.size() > 0)
                recurse(accum, stack, sequences);
            
            _result = accum;
        }
        
        std::vector<std::vector<T>> get() {
            return _result;
        }
    private:
        std::vector<std::vector<T>> _result;
        
        void recurse(std::vector<std::vector<T>> &accum, std::vector<T> stack, std::vector<std::vector<T>> sequences, int index = 0) {
            std::vector<T> sequence = sequences[index];
            for (T& i : sequence) {
                stack.push_back(i);
                if (index == sequences.size() - 1)
                    accum.push_back(stack);
                else
                    recurse(accum, stack, sequences, index + 1);
                stack.pop_back();
            }
        }
    };
}