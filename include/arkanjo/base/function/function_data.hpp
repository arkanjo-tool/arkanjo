#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

/**
 * @brief Container holding metadata and type-erased features for an extracted function.
 */
class FunctionData {
public:
    std::string path;          ///< File path where the function is located.
    std::string function_name; ///< Extracted identifier of the function.

    std::unordered_map<std::type_index, std::shared_ptr<void>> features;

    /**
     * @brief Associates a feature of type T with this function.
     *
     * @tparam T Feature type.
     * @param feature Shared pointer to the feature instance.
     */
    template<typename T>
    void add_feature(std::shared_ptr<T> feature) {
        features[typeid(T)] = feature;
    }

    /**
     * @brief Retrieves a feature of type T associated with this function.
     *
     * @tparam T Feature type to retrieve.
     * @return Shared pointer to feature of type T, or nullptr if not found.
     */
    template<typename T>
    std::shared_ptr<T> get_feature() const {
        auto it = features.find(typeid(T));

        if (it == features.end())
            return nullptr;

        return std::static_pointer_cast<T>(it->second);
    }
};
