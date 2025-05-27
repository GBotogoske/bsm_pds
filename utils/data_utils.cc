#include <iostream>
#include <filesystem>
#include <string>
#include <vector>


int check_apa(int channel)
{
    for(int APA=1;APA<=4;APA++)
    {
        int min = 40*(4-APA);
        int max = min + 40;
        if(min<=channel && channel<max)
        {
            return APA;
        }
            
    }
    return -1;
}

std::vector<std::string> return_paths(std::string folder, int run)
{
    std::vector<std::string> files{};
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();

            if (filename.find("run0"+std::to_string(run)) != std::string::npos) {
                std::cout << filename << std::endl;
                files.push_back(filename);
            }
        }
    }
    return files;
}

double return_time(std::string file_name,long min_time,long max_time)
{
    beam_time_analyser* my_time_analyser = new beam_time_analyser();
    my_time_analyser->set_filename(file_name);
    my_time_analyser->read_data();
    int N = my_time_analyser->get_N();

    long tmin=min_time;
    long tmax=max_time;
    long deltat= 4.8/16e-9;
    double min_pot=1e12;

    int start_index=0;
    int end_index=0;
    double percent=0.0;

    for(int i = N-1;i>=0;i--)
    {
        beam_data beam_aux = my_time_analyser->get_beam_data_index(i);
        double pot = beam_aux.pot;
        long start_time = beam_aux.time/16e-9;
        long end_time = start_time + deltat;
        
        if(tmin>=start_time)
        {
            if(tmin<=end_time)
            {
                start_index=i+1;
                if(pot > min_pot)
                {
                    percent = (double) (end_time - tmin)/deltat;
                }
                break;
            }
            else
            {
                start_index=i+1;
                break;
            }
        }
    }
    for(int i = start_index ;i<N;i++)
    {
        beam_data beam_aux = my_time_analyser->get_beam_data_index(i);
        double pot = beam_aux.pot;
        long start_time = beam_aux.time/16e-9;
        long end_time = start_time + deltat;
        
        if(tmax<=end_time)
        {
            if(tmax>=start_time)
            {
                if(pot > min_pot)
                {
                    percent += (double)(tmax - start_time)/deltat;
                }
                end_index=i-1;
                break;
            }
            else
            {
                end_index=i-1;
                break;
            }
        }
    }
    float total = (float) end_index - start_index + percent + 1;
    for(int i = start_index ;i<=end_index;i++)
    {
        beam_data beam_aux = my_time_analyser->get_beam_data_index(i);
        double pot = beam_aux.pot;
        if(pot < min_pot)
        {
            total = total - 1 ;
        }
        
    }
    return total*4.8;
}