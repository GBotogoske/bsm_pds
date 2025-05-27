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
#include <TFile.h>

#include "./utils/Next_Clicked.cc"
#include "./utils/data_utils.cc"

using namespace std;

// Declaração da variável global, compartilhada com o Next_Clicked.cc
bool next_clicked = false;

int main(int argc, char** argv)
{
    // args: 1) my_index print wfs in the range or not ( only usable in operation 0 ) 
    //       2) APA which APA to look 
    //       3) operation (0 - print wfs / 1 - save root)
    int my_index,APA,operation;

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

    if(argc>3)
    {
        operation = std::stoi(argv[3]);
    }
    else
    {
        operation = 0;
    }

    std::cout << "Looking APA: " << APA << std::endl; 


    TApplication theApp("App", &argc, argv);

    file_analyser my_analyser;
   
    string tree_name = "waveAna/waveform_tree";
    std::string folder_path("/home/gabriel/Documents/protodune/data/HD/29917/");
    auto paths = return_paths(folder_path,29917);

    for(auto& this_path:paths)
    {
        my_analyser.set_file_name(folder_path+this_path);
    }
    my_analyser.set_tree_name(tree_name);

    beam_time_analyser* my_time_analyser = new beam_time_analyser();
    my_time_analyser->set_filename("/home/gabriel/Documents/protodune/bsm/pds/data/beam_info/spillrun029917.csv");
    my_time_analyser->read_data();

    //std::vector<CoincidenceGroup> all_coincidences = my_analyser.find_coincident_events(APA,5);
    int time_diff = 12*5;
    std::vector<CoincidenceGroup> coincidences = my_analyser.find_coincident_events_ranges(APA,time_diff,*my_time_analyser);
    std::vector<CoincidenceGroup> coincidences_no_beam = my_analyser.find_coincident_events_ranges(APA,time_diff,*my_time_analyser,false);

    my_analyser.calc_parameters_coincidence_group(&coincidences);
    my_analyser.calc_parameters_coincidence_group(&coincidences_no_beam);

    std::vector< std::vector<CoincidenceGroup>*> my_coincidences(2);
    my_coincidences[0]=&coincidences;
    my_coincidences[1]=&coincidences_no_beam;

    if(operation == 1)
    {
        // ROOT configuration output
        auto myFile = new TFile("data.root","RECREATE");
        auto myTree = new TTree("my_tree","Data");

        my_data aux_data;
        auto myBranch = myTree->Branch("data", &aux_data);

        for(int my_range=0;my_range<2;my_range++)
        {
            int n=my_coincidences[my_range]->size();
            for (int i = 0; i < n; i++)
            {
                int m = (*my_coincidences[my_range])[i].events.size();
                for (int j = 0; j < m; j++)
                {
                    aux_data = (*my_coincidences[my_range])[i].events[j];
                    aux_data.range=my_range;
                    aux_data.event=i;
                    aux_data.number=m;
                    aux_data.APA=check_apa(aux_data.Channel);
                    
                    myTree->Fill();
                }
            }
            
        }
        myTree->Write("", TObject::kOverwrite);
        myFile->Close();

    //--------------------------------------------
    }
    else if(operation == 0)
    {

        //CANVAS configuration
        TCanvas* c = new TCanvas("c", "ADCs", 2000, 4000);
    
        TCanvas* c_button = new TCanvas("c_button", "Controles", 400, 100);
    
        c_button->cd();
        TButton* b = new TButton("NEXT", ".x utils/Next_Clicked.cc", 0.85, 0.2, 0.98, 0.8);
        b->SetFillColor(38);
        b->Draw();
        //----------------------------------------------

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
    }

    return 0;
}