#include "beam_time_analyser.hh"
#include <fstream>
#include <iostream>
#include <sstream>

beam_time_analyser::beam_time_analyser()
{
}

beam_time_analyser::~beam_time_analyser()
{
}

std::string beam_time_analyser::get_filename()
{
    return this->my_filename;
}

void beam_time_analyser::set_filename(const std::string filename)
{
    this->my_filename=filename;
}

bool beam_time_analyser::read_data()
{
    this->my_data.clear();

    std::ifstream file(this->my_filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo CSV.\n";
        return 1;
    }

    std::string linha;
    std::getline(file, linha);  //jump header

    beam_data aux;

    while (std::getline(file, linha)) 
    {
        std::stringstream ss(linha);
        std::string item;

        double ts = 0.0, inten = 0.0;
        int coluna = 0;

        while (std::getline(ss, item, ','))
         {
            if (coluna == 0)
            {
                ts = std::stod(item);
            } else if (coluna == 1) 
            {
                inten = std::stod(item);
            }
            ++coluna;
        }
      
        aux.pot=inten;
        aux.time=ts;

        this->my_data.push_back(aux);   
    }

    this->N_points = this->my_data.size();
    return 0;
}

const beam_data beam_time_analyser::get_beam_data_index(const int index)
{
    return this->my_data[index];
}

const int beam_time_analyser::get_N()
{
    return this->N_points;
}
