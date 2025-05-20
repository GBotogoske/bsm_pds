#ifndef BEAM_TIME_ANALYSER_HH
#define BEAM_TIME_ANALYSER_HH

#include <string>
#include <vector>

typedef struct
{
    double time;
    double pot;
}beam_data;


class beam_time_analyser
{
    public:

        beam_time_analyser();
        ~beam_time_analyser();
        std::string get_filename();
        void set_filename(const std::string filename);
        bool read_data();
        const beam_data get_beam_data_index(const int index);
        const int get_N();

    private:

        std::string my_filename;
        std::vector<beam_data> my_data;
        int N_points=0;

};


#endif