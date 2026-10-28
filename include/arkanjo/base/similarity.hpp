#pragma once

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#ifndef NO_USE_SIMILARITY_CLAMP
#define USE_SIMILARITY_CLAMP
#endif

#ifdef USE_SIMILARITY_CLAMP
#include <algorithm>
#endif

struct Similarity {
    static constexpr const double MAXIMUM = 1.0; ///< Maximum possible similarity score
    static constexpr const double MINIMUM = 0.0; ///< Minimum possible similarity score

    static constexpr const double EPS_ERROR_MARGIN = 1e-8; ///< Floating-point comparison margin

    double value_;

    static double clamp(double value) {
        if (!std::isfinite(value))
            throw std::invalid_argument("Similarity must be finite.");

#ifdef USE_SIMILARITY_CLAMP
        return std::clamp(value, MINIMUM, MAXIMUM);
#else
        if (value < MINIMUM || value > MAXIMUM)
            throw std::out_of_range(
                "Similarity must be in the range [0.0, 1.0]. (value " + std::to_string(value) + ")"
            );

        return value;
#endif
    }

    /**
     * @brief Converts a normalized similarity score to a percentage.
     *
     * @param similarity Similarity score to convert.
     * @return Percentage in the range [0.0, 100.0].
     */
    static double write(const Similarity& similarity) {
        return similarity.value_ * 100.0;
    }

    /**
     * @brief Converts a percentage to a normalized similarity score.
     *
     * @param value Percentage to convert.
     * @return Similarity score in the range [0.0, 1.0].
     *
     * @throws std::invalid_argument If the resulting score is not finite.
     * @throws std::out_of_range If the resulting score is outside the valid
     *         range and clamping is disabled.
     */
    static Similarity read(const double& value) {
        return Similarity{value / 100.0};
    }

    static Similarity read(const std::string& value) {
        return read(std::stod(value));
    }

    Similarity(double value = MINIMUM)
        : value_(clamp(value)) {}

    Similarity& operator=(double value) {
        value_ = clamp(value);
        return *this;
    }

    double value() const {
        return value_;
    }

    /**
     * @brief Checks whether two similarity scores are approximately equal.
     *
     * \f[
     * |value - other| \leq \varepsilon
     * \f]
     *
     * @param other Similarity score to compare against.
     * @return true if the scores are approximately equal; false otherwise.
     */
    bool approximately_equal(const Similarity& other) const {
        return std::abs(value_ - other.value_) <= EPS_ERROR_MARGIN;
    }

    /**
    * @brief Checks whether this similarity is approximately less than another.
    *
    * \f[
    * value < other - \varepsilon
    * \f]
    *
    * @param other Similarity score to compare against.
    * @return true if this score is approximately less; false otherwise.
    */
    bool approximately_less(const Similarity& other) const {
        return value_ < other.value_ - EPS_ERROR_MARGIN;
    }

    /**
    * @brief Checks whether this similarity is approximately greater than another.
    *
    * \f[
    * value > other + \varepsilon
    * \f]
    *
    * @param other Similarity score to compare against.
    * @return true if this score is approximately greater; false otherwise.
    */
    bool approximately_greater(const Similarity& other) const {
        return value_ > other.value_ + EPS_ERROR_MARGIN;
    }

    /**
    * @brief Checks whether this similarity is approximately greater than
    * or equal to another.
    *
    * \f[
    * value \geq other - \varepsilon
    * \f]
    *
    * @param other Similarity score to compare against.
    * @return true if this score is approximately greater than or equal
    *         to the other score; false otherwise.
    */
    bool approximately_at_least(const Similarity& other) const {
        return value_ >= other.value_ - EPS_ERROR_MARGIN;
    }

    /**
    * @brief Checks whether this similarity is approximately less than
    * or equal to another.
    *
    * \f[
    * value \leq other + \varepsilon
    * \f]
    *
    * @param other Similarity score to compare against.
    * @return true if this score is approximately less than or equal
    *         to the other score; false otherwise.
    */
    bool approximately_at_most(const Similarity& other) const {
        return value_ <= other.value_ + EPS_ERROR_MARGIN;
    }

    bool operator<(const Similarity& similarity) const {
        return value_ < similarity.value_;
    }

    bool operator<=(const Similarity& similarity) const {
        return value_ <= similarity.value_;
    }

    bool operator<(const double& v) const {
        return value_ < v;
    }

    bool operator<=(const double& v) const {
        return value_ <= v;
    }

    bool operator>(const Similarity& similarity) const {
        return value_ > similarity.value_;
    }

    bool operator>=(const Similarity& similarity) const {
        return value_ >= similarity.value_;
    }

    bool operator>(const double& v) const {
        return value_ > v;
    }

    bool operator>=(const double& v) const {
        return value_ >= v;
    }

    bool operator==(const Similarity& similarity) const {
        return value_ == similarity.value_;
    }

    bool operator==(const double& v) const {
        return value_ == v;
    }

    bool operator!=(const Similarity& similarity) const {
        return value_ != similarity.value_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Similarity& similarity) {
        return os << std::fixed << std::setprecision(2) << write(similarity);
    }

    friend std::istream& operator>>(std::istream& is, Similarity& similarity) {
        double value;

        if (is >> value)
            similarity = read(value);

        return is;
    }
};
