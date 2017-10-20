/// @file   gptValue.hpp
/// @author Enrico Fraccaroli
/// @date   Oct 20 2017
/// @copyright
/// Copyright (c) 2017 Enrico Fraccaroli <enrico.fraccaroli@univr.it>

#pragma once

#include <algorithm>    // std::move
#include <cassert>      // assert
#include <iostream>

/// @brief Abstract class for storing pointers to traced variables.
class GPTVariable
{
protected:
    /// The name of the trace.
    std::string name;
    /// The format used to print the trace.
    std::string format;
    /// The width of the line.
    unsigned int lineWidth;

public:

    /// @brief Constructor.
    /// @param _name        The name of the trace.
    /// @param _format      The format used to print the trace.
    /// @param _lineWidth   The width of the line.
    explicit GPTVariable(std::string _name,
                         std::string _format,
                         const unsigned int & _lineWidth) :
        name(std::move(_name)),
        format(std::move(_format)),
        lineWidth(_lineWidth)
    {
        // Nothing to do.
    }

    /// @brief Destructor.
    virtual ~GPTVariable() = default;

    /// @brief Provides the name.
    inline std::string getName() const
    {
        return name;
    }

    /// @brief Provides the format.
    inline std::string getFormat() const
    {
        return format;
    }

    /// @brief Provides the width of the line.
    inline unsigned int getLineWidth() const
    {
        return lineWidth;
    }

    /// @brief Samples the value from the traced variable.
    virtual void sampleValue() = 0;

    /// @brief Checks if there are values.
    virtual bool empty() = 0;

    /// @brief Provides the number of stored values.
    virtual size_t size() = 0;

    /// @brief Provides the sampled value at the given position inside the
    /// vector of sampled values.
    /// @param pos The position inside the sampling vector.
    /// @return The sampled value.
    virtual double get(const size_t & pos) = 0;
};

/// @brief Generalized class for storing pointers to traced variables.
template<typename T>
class GPTGenericVariable :
    public GPTVariable
{
private:
    /// The traced variable.
    T * variable;
    /// The sampled values.
    std::vector<T> values;

public:

    /// @brief Constructor.
    GPTGenericVariable(T * _variable,
                       const std::string & _name,
                       const std::string & _format,
                       const unsigned int & _lineWidth) :
        GPTVariable(_name, _format, _lineWidth),
        variable(_variable)
    {
        // Nothing to do.
    }

    inline void sampleValue() override
    {
        assert(variable);
        values.emplace_back(*variable);
    }

    inline bool empty() override
    {
        return values.empty();
    }

    inline size_t size() override
    {
        return values.size();
    }

    inline double get(const size_t & pos) override
    {
        assert(pos < this->size());
        return values[pos];
    }
};