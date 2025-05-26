#include <iostream>
#include <file_analyser.hh>
#include <beam_time_analyser.hh>
#include <TGraph.h>
#include <TCanvas.h>
#include <TApplication.h> 

#include <TButton.h>
#include <TThread.h>
#include <TSystem.h>
#include <TInterpreter.h>  // Necessário para ProcessLine
#include "./utils/Next_Clicked.cc"

using namespace std;

// Declaração da variável global, compartilhada com o Next_Clicked.cc
bool next_clicked = false;

int main(int argc, char** argv)
{

    int my_index,APA;

    if(argc>1)
    {
        my_index = std::stoi(argv[1]);
    }
    else
    {
        my_index = 0;
    }

    if(argc>2)
    {
        APA = std::stoi(argv[2]);
    }
    else
    {
        APA = 3;
    }

    std::cout << "Looking APA: " << APA << std::endl; 


    TApplication theApp("App", &argc, argv);

    file_analyser my_analyser;
    string file_name = "/home/gabriel/Documents/protodune/data/HD/29917/np04hd_raw_run029917_0000_dataflow0_datawriter_0_20241011T153510_wf_pds.root";
    string tree_name = "waveAna/waveform_tree";
    my_analyser.set_file_name(file_name);
    my_analyser.set_tree_name(tree_name);
    my_analyser.update();

    TCanvas* c = new TCanvas("c", "ADCs", 2000, 4000);
    
    TCanvas* c_button = new TCanvas("c_button", "Controles", 400, 100);

    c_button->cd();
    TButton* b = new TButton("NEXT", ".x utils/Next_Clicked.cc", 0.85, 0.2, 0.98, 0.8);
    b->SetFillColor(38);
    b->Draw();

    beam_time_analyser* my_time_analyser = new beam_time_analyser();
    my_time_analyser->set_filename("/home/gabriel/Documents/protodune/bsm/pds/data/beam_info/spillrun029917.csv");
    my_time_analyser->read_data();

    //std::vector<CoincidenceGroup> all_coincidences = my_analyser.find_coincident_events(APA,5);
    std::vector<CoincidenceGroup> coincidences = my_analyser.find_coincident_events_ranges(APA,5,*my_time_analyser);
    std::vector<CoincidenceGroup> coincidences_no_beam = my_analyser.find_coincident_events_ranges(APA,5,*my_time_analyser,false);

    /* std::cout << all_coincidences.size() << std::endl;
    std::cout << coincidences.size() << std::endl;
    std::cout << coincidences_no_beam.size() << std::endl; */

    std::vector< std::vector<CoincidenceGroup>*> my_coincidences(2);
    my_coincidences[0]=&coincidences;
    my_coincidences[1]=&coincidences_no_beam;

    int n=my_coincidences[my_index]->size();

    for (int index = 0; index < n; index++)
    {
        std::cout << "mostrando index: " << index << std::endl;
        for(my_data data_for:  (*my_coincidences[my_index])[index].events)
        {
            std::cout << data_for.Channel << " -------- "<< data_for.Timestamp << std::endl;
        }
    
        next_clicked = false;
        my_analyser.print_all_channels_group(APA, c, (*my_coincidences[my_index])[index].events);
        c->Update();
        c_button->cd();
        c_button->Update(); 

        // Espera até o botão ser clicado
        while (!next_clicked)
        {
            gSystem->ProcessEvents();
        }
    }

    return 0;
}