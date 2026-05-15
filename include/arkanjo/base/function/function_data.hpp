#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

class FunctionData {
public:
    std::string path;

    /**
     * @brief Name of the function.
     *
     * Extracted identifier of the function (e.g., "main", "process_data").
     */
    std::string function_name;

    std::unordered_map<std::type_index, std::shared_ptr<void>> features;

    template<typename T>
    void add_feature(std::shared_ptr<T> feature) {
        features[typeid(T)] = feature;
    }

    template<typename T>
    std::shared_ptr<T> get_feature() const {
        auto it = features.find(typeid(T));

        if (it == features.end())
            return nullptr;

        return std::static_pointer_cast<T>(it->second);
    }
};
