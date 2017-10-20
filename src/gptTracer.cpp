/// @file   gnuplotHelper.cpp
/// @author Enrico Fraccaroli
/// @date   Oct 20 2017
/// @copyright
/// Copyright (c) 2017 Enrico Fraccaroli <enrico.fraccaroli@univr.it>

#include "gptTracer.hpp"

GPTTracer::GPTTracer() :
    _title("Simulation Traces"),
    _terminal("wxt"),
    _background("white"),
    _foreground("black"),
    _font_key(14),
    _font_size_x_axis(14),
    _font_size_y_axis(14),
    _bound_x(),
    _bound_y(),
    _pipe(),
    _x_variable(),
    _y_variables()
{
    // Nothing to do.
}

GPTTracer::GPTTracer(std::string title,
                     std::string terminal,
                     std::string background,
                     std::string foreground,
                     const unsigned int & font_key,
                     const unsigned int & font_size_x_axis,
                     const unsigned int & font_size_y_axis) :
    _title(std::move(title)),
    _terminal("wxt"),
    _background(std::move(background)),
    _foreground(std::move(foreground)),
    _font_key(font_key),
    _font_size_x_axis(font_size_x_axis),
    _font_size_y_axis(font_size_y_axis),
    _bound_x(),
    _bound_y(),
    _pipe(),
    _x_variable(),
    _y_variables()
{
    const bool supported = (terminal == "dumb" ||
                            terminal == "x11" ||
                            terminal == "wxt");
    if (!supported)
    {
        std::cerr << "WARNING: gnuplot terminal type not tested: "
                  << terminal << "\n";
    }
    else
    {
        _terminal = terminal;
    }
}

GPTTracer::~GPTTracer()
{
    this->close();
    delete(_bound_x);
    delete(_bound_y);
}

void GPTTracer::addPlotLine()
{
    fprintf(_pipe, "plot ");
    for (size_t i = 0; i < _y_variables.size(); ++i)
    {
        GPTVariable * gpValue = _y_variables[i];
        fprintf(_pipe,
                "'-' w l lw %d t '%s'",
                gpValue->getLineWidth(),
                gpValue->getName().c_str());
        if (i < (_y_variables.size() - 1))
        {
            fprintf(_pipe, ", ");
        }
    }
    fprintf(_pipe, "\n");
}

void GPTTracer::addDataPoints()
{
    for (auto it : _y_variables)
    {
        auto format = _x_variable->getFormat() + " " +
                      it->getFormat() + "\n";
        for (size_t j = 0; j < it->size(); ++j)
        {
            fprintf(_pipe,
                    format.c_str(),
                    _x_variable->get(j),
                    it->get(j));
        }
        fprintf(_pipe, "end\n");
    }
}

void GPTTracer::setup()
{
    if (!this->isOpened())
    {
        return;
    }
    auto bg = (_background.empty()) ? "white" : _background.c_str();
    auto fg = (_foreground.empty()) ? "black" : _foreground.c_str();

    fprintf(_pipe, "reset\n");
    if (!_terminal.empty())
    {
        fprintf(_pipe,
                "set terminal %s background rgb '%s'\n",
                _terminal.c_str(), bg);
    }
    if (!_title.empty())
    {
        fprintf(_pipe,
                "set title '%s' font ',%d'  textcolor rgbcolor '%s'\n",
                _title.c_str(), _font_key, fg);
    }
    fprintf(_pipe,
            "set key Left font ',%d' textcolor rgbcolor '%s'\n",
            _font_key, fg);
    fprintf(_pipe,
            "set xlabel 'Time (s)' font ',%d' textcolor rgbcolor '%s'\n",
            _font_size_x_axis, fg);
    fprintf(_pipe,
            "set ylabel 'Voltage (V)' font ',%d' textcolor rgbcolor '%s'\n",
            _font_size_y_axis, fg);
    fprintf(_pipe, "set grid\n");
    if(_bound_x!= nullptr)
    {
    fprintf(_pipe, "set xrange [%lf:%lf]\n",
            _bound_x->getLowerBound(),
            _bound_x->getUpperBound());

    }
    if(_bound_x!= nullptr)
    {
        fprintf(_pipe, "set yrange [%lf:%lf]\n",
                _bound_y->getLowerBound(),
                _bound_y->getUpperBound());
    }
    fprintf(_pipe, "set border linecolor rgbcolor '%s'\n",
            fg);
}