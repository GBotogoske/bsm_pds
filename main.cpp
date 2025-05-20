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

    //std::vector<CoincidenceGroup> coincidences = my_analyser.find_coincident_events(4);

    //long t1=108041308336874992;
    //long t2=t1+(long)4.8/16e-9;

    //std::cout<< t1 << " --> " << t2 << endl;

    //std::vector<CoincidenceGroup> coincidences = my_analyser.find_coincident_events_range(4,5,t1,t2);

    std::vector<CoincidenceGroup> coincidences = my_analyser.find_coincident_events_ranges(4,5,*my_time_analyser);

    int n=coincidences.size();

    for (int index = 0; index < n; index++)
    {
        std::cout << "mostrando index: " << index << std::endl;
        for(my_data data_for:  coincidences[index].events)
        {
            std::cout << data_for.Channel << " -------- "<< data_for.Timestamp << std::endl;
        }
    
        next_clicked = false;
        my_analyser.print_all_channels_group(4, c, coincidences[index].events);
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