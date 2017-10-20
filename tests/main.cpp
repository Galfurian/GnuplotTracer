/// @file   main.cpp
/// @author Enrico Fraccaroli
/// @date   Oct 20 2017
/// @copyright
/// Copyright (c) 2017 Enrico Fraccaroli <enrico.fraccaroli@univr.it>

#include <gptTracer.hpp>
#include <cstring>
#include <cmath>

#ifndef M_2_FOR_PI
#define M_2_FOR_PI    6.2831853071795864
#endif

int main(int argc, char * argv[])
{
    (void) argc;
    (void) argv;
    uint32_t time = 0;
    double sine_out = 0.0;
    double sine_off = 0.0;
    double sine_amp = 1.0;
    double sine_frq = 1.0;

    bool clock = true;

    GPTTracer gptTracer("Simulation Traces", "wxt", "", "", 14, 12, 12);
    gptTracer.setVariableX(&time, "Time", "%lf", 3);
    gptTracer.addVariableY(&sine_out, "sine", "%lf", 3);
    gptTracer.addVariableY(&clock, "clock", "%lf", 3);

    std::cout << "Starting simulation...\n";
    while (time < 1e03)
    {
        sine_out = sine_off +
                   sine_amp * std::sin(M_2_FOR_PI * sine_frq * (time * 1e-03));
        if ((time % 100) == 0)
        {
            clock = !clock;
        }
        ++time;
        gptTracer.sampleData();
    }
    std::cout << "Done\n";
    if (argc > 1)
    {
        if (strcmp(argv[1], "test") == 0)
        {
            gptTracer.printData(false);
            return 0;
        }
    }
    gptTracer.printData(true);
    return 0;
}