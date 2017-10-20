/// @file   gptBound.hpp
/// @author Enrico Fraccaroli
/// @date   Oct 20 2017
/// @copyright
/// Copyright (c) 2017 Enrico Fraccaroli <enrico.fraccaroli@univr.it>

#pragma once

/// @brief Container for lower and upper bound.
class GPTBound
{
private:
    /// The lower bound.
    double lower_bound;
    /// The upper bound.
    double upper_bound;

public:
    /// @brief Constructor.
    GPTBound() :
        lower_bound(-std::numeric_limits<double>::infinity()),
        upper_bound(+std::numeric_limits<double>::infinity())
    {
        // Nothing to do.
    }

    /// @brief Constructor.
    GPTBound(const double & _lower_bound,
             const double & _upper_bound) :
        lower_bound(_lower_bound),
        upper_bound(_upper_bound)
    {
        // Nothing to do.
    }

    /// @brief Provides the lower bound.
    inline double getLowerBound() const
    {
        return lower_bound;
    }

    /// @brief Provides the upper bound.
    inline double getUpperBound() const
    {
        return upper_bound;
    }
};