#include "my_data.hh"
#include <iostream>
#include <cmath>

void my_data::print_baseline()
{
    std::cout << "Baseline: " << this->baseline <<std::endl;
}

void my_data::print_noise()
{
    std::cout << "Noise: " << this->noise <<std::endl;
}

void my_data::print_t0()
{
    std::cout << "t0: " << this->t0 <<std::endl;
}

void my_data::print_zero_crossing()
{
    std::cout << "Zero crossing: " << this->zero_crossing <<std::endl;
}

void my_data::print_integral()
{
    std::cout << "Integral: " << this->integral <<std::endl;
}

void my_data::print_amplitude()
{
    std::cout << "Amplitude: " << this->amplitude <<std::endl;
}

void my_data::print_all()
{
    this->print_baseline();
    this->print_noise();
    this->print_t0();
    this->print_zero_crossing();
    this->print_integral();
    this->print_amplitude();
}

void my_data::calc_baseline(const int index)
{
    this->baseline=0;
    for(int i = 0; i<index; i++)
    {
        this->baseline+=this->adcs[i];
    }
    this->baseline/=index;
}

void my_data::calc_noise(const int index)
{
    this->noise = 0;
    for(int i = 0; i<index; i++)
    {
        double delta = this->adcs[i] - this->baseline;
        this->noise += delta * delta;
    }
    this->noise = std::sqrt(this->noise/index);
}

void my_data::calc_t0(const int index,const int index_lim)
{
    float threshold = this->baseline - 3 * this->noise;

    for (int i = index + 1; i < index_lim ; ++i) 
    {
        if (this->adcs[i] < threshold && this->adcs[i - 1] >= threshold) 
        {
            
            float x1 = (i - 1);
            float y1 = this->adcs[i - 1];
            float x2 = i;
            float y2 = this->adcs[i];

            float m = (float)((y2 - y1) / (x2 - x1));
            float t0 = x1 + (float)((threshold - y1) / m);
            this->t0 = t0;
            return;
        }
    }

    this->t0 = -1000; //error
    return; 
}

void my_data::calc_zero_crossing(const int index,const int index_lim)
{
    float threshold = this->baseline + 3 * this->noise;

    //std::cout << threshold << std::endl;
    for (int i = index + 1; i < index_lim ; ++i) 
    {
        if (this->adcs[i] > threshold && this->adcs[i - 1] <= threshold) 
        {
            
            float x1 = (i - 1);
            float y1 = this->adcs[i - 1];
            float x2 = i;
            float y2 = this->adcs[i];

            float m = (float)((y2 - y1) / (x2 - x1));
            float zc = x1 + (float)((threshold - y1) / m);
            this->zero_crossing = zc;
            return;
        }
    }

    this->zero_crossing = -1000; //error
    return; 
}

void my_data::calc_integral()
{
    this->integral = 0;
    if(this->zero_crossing != -1000 && this->t0 != -1000)
    {
        if(this->zero_crossing >= this->t0)
        {
            int start = (int)this->t0;
            int end = (int)this->zero_crossing;

            for(int i = start; i<=end ; i++)
            {
                this->integral-=(this->adcs[i]-this->baseline);
            }
            return;

        }
    }
    this->integral=-1000;
    return;
}

void my_data::calc_amplitude()
{
    this->amplitude = 1000;
    if(this->zero_crossing != -1000 && this->t0 != -1000)
    {
        if(this->zero_crossing >= this->t0)
        {
            int start = (int)this->t0;
            int end = (int)this->zero_crossing;

            for(int i = start; i<=end ; i++)
            {
                double value = this->adcs[i]-this->baseline;
                if(this->amplitude >= value)
                {
                    this->amplitude = value;
                }
            }

        }
    }
    this->amplitude=-this->amplitude;
    return;
}
