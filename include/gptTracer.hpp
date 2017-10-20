/// @file   gnuplotHelper.hpp
/// @author Enrico Fraccaroli
/// @date   Oct 02 2017
/// @copyright
/// Copyright (c) 2017 Enrico Fraccaroli <enrico.fraccaroli@univr.it>

#pragma once

#include "gptValue.hpp"
#include "gptBound.hpp"

#include <iostream>

#ifdef WIN32
    #define GNUPLOT_NAME "pgnuplot -persistent"
#else
    #define GNUPLOT_NAME "gnuplot"
#endif

/// @brief Gnuplot trace generation tracer.
class GPTTracer
{
private:
    /// The title of the trace.
    std::string _title;
    /// The type of terminal.
    std::string _terminal;
    /// The background color.
    std::string _background;
    /// The foreground color.
    std::string _foreground;
    /// The font dimension of the keys.
    unsigned int _font_key;
    /// The font dimension for the x axis label.
    unsigned int _font_size_x_axis;
    /// The font dimension for the y axis label.
    unsigned int _font_size_y_axis;
    /// The boundaries for the x axis.
    GPTBound * _bound_x;
    /// The boundaries for the y axis.
    GPTBound * _bound_y;
    /// Pipe used to communicate with gnuplot.
    FILE * _pipe;
    /// The x axis variable.
    GPTVariable * _x_variable;
    /// The y axis variables.
    std::vector<GPTVariable *> _y_variables;

public:

    /// @brief Constructor.
    GPTTracer();

    /// @brief Constructor.
    GPTTracer(std::string title,
              std::string terminal,
              std::string background,
              std::string foreground,
              const unsigned int & font_key,
              const unsigned int & font_size_x_axis,
              const unsigned int & font_size_y_axis);

    /// @brief Destructor.
    ~GPTTracer();

    /// @brief Open the pipe to gnuplot.
    inline void open()
    {
        this->close();
#ifdef WIN32
        _pipe = _popen(GNUPLOT_NAME, "w");
#else
        _pipe = popen(GNUPLOT_NAME, "w");
#endif
    }

    /// @brief Checks whether the pipe is open.
    inline bool isOpened() const
    {
        return _pipe != nullptr;
    }

    /// @brief Closes the pipe.
    inline void close()
    {
        if (this->isOpened())
        {
            #ifdef WIN32
            _pclose(_pipe);
            #else
            pclose(_pipe);
            #endif
            _pipe = nullptr;
        }
    }

    /// @brief Flushes data on pipe.
    inline void flush()
    {
        if (this->isOpened())
        {
            fflush(_pipe);
        }
    }

    /// @brief Allows to set the boundaries for the x axis.
    inline void setBoundX(double _lower_bound, double _upper_bound)
    {
        _bound_x = new GPTBound(_lower_bound, _upper_bound);
    }

    /// @brief Allows to set the boundaries for the y axis.
    inline void setBoundY(double _lower_bound, double _upper_bound)
    {
        _bound_y = new GPTBound(_lower_bound, _upper_bound);
    }

    /// @brief Allows to set the variable for the x axis.
    template<typename T>
    inline void setVariableX(T * _variable,
                             const std::string & _name,
                             const std::string & _format,
                             const unsigned int & _lineWidth)
    {
        assert(_variable);
        _x_variable = new GPTGenericVariable<T>(_variable,
                                                _name,
                                                _format,
                                                _lineWidth);
    }

    /// @brief Allows to add a variable on the y axis.
    template<typename T>
    inline void addVariableY(T * _variable,
                             const std::string & _name,
                             const std::string & _format,
                             const unsigned int & _lineWidth)
    {
        assert(_variable);
        _y_variables.emplace_back(
            new GPTGenericVariable<T>(_variable,
                                      _name,
                                      _format,
                                      _lineWidth));
    }

    /// @brief Prints the data.
    inline void printData(const bool & visualize)
    {
        this->open();
        this->setup();
        this->addPlotLine();
        this->addDataPoints();
        this->flush();
        // Waits for the user to look at the graph.
        if (visualize)
        {
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cin.get();
        }
    }

    /// @brief Samples the data.
    inline void sampleData()
    {
        _x_variable->sampleValue();
        for (auto value : _y_variables)
        {
            value->sampleValue();
        }
    }

private:

    void addPlotLine();

    void addDataPoints();

    void setup();
};
