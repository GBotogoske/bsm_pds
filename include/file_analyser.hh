#ifndef FILE_ANALYSER_HH
#define FILE_ANALYSER_HH

#include <string>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>

#include <beam_time_analyser.hh>
#include <my_data.hh>

typedef struct{
    std::vector<my_data> events;
}CoincidenceGroup;


class file_analyser
{
    public:
        file_analyser();
        ~file_analyser();

        void set_file_name(const std::string file);
        const std::string get_file_name();
        void set_tree_name(const std::string tree_name);
        const std::string get_tree_name();
        const TFile* get_root();
        const TTree* get_tree();
        const int get_entries();
        
        void open_root_file();
        void open_ttree();
        void close_root_file();

        
        my_data* get_data_index(const int i);
        const my_data* get_data_index_and_channel(const int i, const int ch);
        std::vector< my_data> get_data_by_channel(const int ch, const int i=-1);
        std::vector<std::vector< my_data>> get_data_by_channels(const std::vector<int> ch = std::vector<int>(1,-1));
        void set_data_by_channels(const std::vector<int> ch = std::vector<int>(1,-1));
        std::vector<std::vector< my_data>> get_data_by_channel();
        void order_by_timestamp_ch(const int ch);
        void order_all();

        void update();

        void calculate_baseline(const int ch, const int index);

        //GRAPH OPTIONS
        void print_waveform(const int ch, const int index, std::string options);
        void print_waveform(const int ch, std::string options, my_data* my_data);
        void print_waveforms(const int ch, const int index = -1);
        void print_all_channels(int APA, TCanvas* c,int nevents = -1);
        void print_all_channels_group(int APA, TCanvas* c,std::vector<my_data> group_events);


        //coincidences
        std::vector<CoincidenceGroup> find_coincident_events(int APA, long deltaT_ns=5);
        std::vector<CoincidenceGroup> find_coincident_events_range(int APA, long deltaT_ns=5, long timestamp_start=0, long timestamp_stop=0);
        std::vector<CoincidenceGroup> find_coincident_events_ranges(const int APA, const long deltaT_ns=5, beam_time_analyser this_time_analyser = beam_time_analyser(),bool in_the_range=true);

    private:

        std::string my_file;
        std::string my_tree_name;
        TFile* my_root;
        TTree* my_tree;
        int n_entries;

        int n_channels = 160;
        my_data aux_data;
        std::vector<short>* adc_ptr = nullptr;
      
        std::vector<std::vector< my_data>> data_by_channel;
        

};

#endif