#ifndef MY_DATA_HH
#define MY_DATA_HH

#include <vector>

class my_data
{
    public:
        unsigned int Channel;
        long Timestamp;
        std::vector<short> adcs; 
        float integral = 0;
        float amplitude = 0;
        float baseline = 0;
        float noise = 0;
        int n_peaks =0;
        float t0 = 0;
        float zero_crossing = 0;

        void print_baseline();
        void print_noise();
        void print_t0();
        void print_zero_crossing();
        void print_integral();
        void print_amplitude();

        void print_all();

        void calc_baseline(const int index);
        void calc_noise(const int index);
        void calc_t0(const int index,const int index_lim);
        void calc_zero_crossing(const int index,const int index_lim);
        void calc_integral();
        void calc_amplitude();
};


#endif