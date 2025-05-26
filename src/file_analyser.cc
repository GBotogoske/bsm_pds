#include "file_analyser.hh"

#include <TGraph.h>

#include <iostream>

#include <TPaveText.h>

#include <TLatex.h>

file_analyser::file_analyser()
{
}

file_analyser::~file_analyser()
{
 
}

void file_analyser::set_file_name(const std::string file)
{
    this->my_file=file;
}

const std::string file_analyser::get_file_name()
{
    return this->my_file;
}

void file_analyser::set_tree_name(const std::string tree_name)
{
    this->my_tree_name=tree_name;
}

const std::string file_analyser::get_tree_name()
{
    return this->my_tree_name;;
}

const TFile* file_analyser::get_root()
{
    return this->my_root;
}

const TTree* file_analyser::get_tree()
{
    return this->my_tree;
}

const int file_analyser::get_entries()
{
    return this->n_entries;
}

void file_analyser::open_root_file()
{
    this->my_root = new TFile(this->my_file.c_str(), "READ");
}

void file_analyser::open_ttree()
{
    this->my_tree = (TTree*)this->my_root->Get(this->my_tree_name.c_str());
    this->n_entries = this->my_tree->GetEntries();
    this->my_tree->SetBranchAddress("offline_channel",&this->aux_data.Channel);
    this->my_tree->SetBranchAddress("timestamp",&this->aux_data.Timestamp);
    this->my_tree->SetBranchAddress("adc",&adc_ptr);
}


void file_analyser::close_root_file()
{
    this->my_root->Close();
    delete this->my_root;
}

std::vector<my_data> file_analyser::get_data_by_channel(const int ch, const int i)
{
    std::vector<my_data> vec_data;
    int cont = 0;
    for(int j = 0; j<this->n_entries; j++)
    {
        if(i != -1 && cont >= i)  // Corrigido para usar >= e garantir que o loop pare quando atingir 'i'
        {
            break;
        }
        const my_data* this_data = get_data_index(j);

        if(this_data->Channel == ch)
        {
            vec_data.push_back(*this_data);  // Aqui copiamos o conteúdo de 'this_data'
            cont++;
        }
    }

    return vec_data;
}

std::vector<std::vector<my_data>> file_analyser::get_data_by_channels(const std::vector<int> ch)
{
    int valid = ch[0];
    if (valid == -1)
    {
        std::cout << "no channel was inserted" << std::endl;
    }

    std::vector<std::vector<my_data>> datas(this->n_channels, std::vector<my_data>());

    for(int j = 0; j<this->n_entries; j++)
    {
        const my_data* this_data = get_data_index(j);

        if(this_data->Channel < this->n_channels)
        {
            datas[this_data->Channel].push_back(*this_data);  
        }
    }

    return datas;
}

void file_analyser::set_data_by_channels(const std::vector<int> ch)
{
    this->data_by_channel = this->get_data_by_channels(ch);
}

std::vector<std::vector<my_data>> file_analyser::get_data_by_channel()
{
    return this->data_by_channel;
}

void file_analyser::order_by_timestamp_ch(const int ch)
{
    if(ch>=160)
    {
        std::cout << "ch: " << ch << " not valid" <<std::endl;
    }

    std::sort(this->data_by_channel[ch].begin(), this->data_by_channel[ch].end(), [](const my_data& a, const my_data& b) {return a.Timestamp < b.Timestamp;});

}

void file_analyser::order_all()
{
    for(int i =0;i<this->n_channels;i++)
    {
        order_by_timestamp_ch(i);
    }
}

void file_analyser::update()
{
    this->open_root_file();
    this->open_ttree();
}

my_data *file_analyser::get_data_index(const int i)
{
    if(i >= this->n_entries || i<0)
    {
        std::cout << "index not valid" <<std::endl;
        return nullptr;
    }
    this->my_tree->GetEntry(i);

    this->aux_data.adcs = *this->adc_ptr;

    return &this->aux_data;
}

const my_data *file_analyser::get_data_index_and_channel(const int i, const int ch)
{
    if(ch>=160)
    {
        std::cout << "ch: " << ch << " not valid" <<std::endl;
    }

    int tam = this->data_by_channel[ch].size();
    if(i>tam)
    {
        std::cout << "index not valid" <<std::endl;
    }

    return &this->data_by_channel[ch][i];
}

void file_analyser::calculate_baseline(const int ch, const int index)
{
    if(this->data_by_channel[ch].empty())
    {
        std::cout << ch << " Array is Empty !!! " << std::endl; 
    }
}

void file_analyser::print_waveform(const int ch, const int index, std::string options)
{
    auto data = this->get_data_index_and_channel(index,ch);
    
    const int n = data->adcs.size();

    TGraph* g = new TGraph(n);
    for(int i = 0; i < n; i++)
    {
        g->SetPoint(i, i, data->adcs[i]);
    }
    
    g->SetTitle(Form("ADCs do Canal %d;Sample;ADC", data->Channel));
    g->SetLineColor(kBlue);
    g->Draw(options.c_str()); 
}

void file_analyser::print_waveform(const int ch, std::string options, my_data* my_data)
{
    auto data = my_data;
    
    const int n = data->adcs.size();

    TGraph* g = new TGraph(n);
    for(int i = 0; i < n; i++)
    {
        g->SetPoint(i, i, data->adcs[i]);
    }
 
    g->SetLineColor(kBlue);
    g->Draw(options.c_str()); 
    TLatex* latex = new TLatex();
    latex->SetNDC(); // usa coordenadas normalizadas
    latex->SetTextSize(0.11);
    latex->DrawLatex(0.2, 0.93, Form("Channel %d", data->Channel));
}

void file_analyser::print_waveforms(const int ch, const int index)
{
    if(this->data_by_channel[ch].empty())
    {
        std::cout << ch << " Array is Empty !!! " << std::endl; 
    }

    std::string options1 = "AL";
    std::string options2 = "SAME";

    int max_index = index;
    if(index == -1)
    {
        max_index = this->data_by_channel[ch].size();
    }

    this->print_waveform(ch,0,options1);
    for(int i = 1; i < max_index; i++)
    {
        this->print_waveform(ch,i,options2);   
    }
}

void file_analyser::print_all_channels(int APA, TCanvas* c, int nevents)
{
    c->Divide(4, 10);
    if(APA<=0 || APA >4)
    {
        std::cout << " APA index not valid !!!" << std::endl; 
    }
    int ch_min=-40*APA+160;
    int i = 0;
    for (int ch = ch_min; ch < ch_min + 40; ch++)
    {

        int row = i % 10;           // linha (0 = topo)
        int col = i / 10;           // coluna
        int pad_number = row * 4 + col;

        c->cd(pad_number + 1); 
        this->print_waveforms(ch, nevents);
        i++;

        // Adiciona o texto com o número do canal no pad
        TPaveText* label = new TPaveText(0.1, 0.8, 0.9, 0.9, "NDC");
        label->AddText(Form("Channel %d", ch));
        label->SetFillColor(0);
        label->SetBorderSize(0);
        label->Draw();
    }

}

void file_analyser::print_all_channels_group(int APA, TCanvas *c, std::vector<my_data> group_events)
{    
    c->Clear(); 
    c->Divide(4, 10);
    if(APA<=0 || APA >4)
    {
        std::cout << " APA index not valid !!!" << std::endl; 
    }

    std::string options1 = "AL";
    std::string options2 = "SAME";

    std::vector<bool> is_there(40,false);

    for(int i=0;i<40;i++)
    {
        c->cd(i + 1); 

        TBox* box = new TBox(0.0, 0.0, 1.0, 1.0);
        box->SetFillStyle(0);        // sem preenchimento
        box->SetLineColor(kGray+2);  // cor da borda
        box->SetLineWidth(5);        // espessura da borda
        box->Draw("l");              // "l" = linha (sem preenchimento)
    }

    for(auto& evt : group_events)
    {
        int ch = evt.Channel;
        int row = ch % 10;          
        int col = ch / 10;           
        int pad_number = row * 4 + col;
        c->cd(pad_number + 1); 

        if(is_there[ch])
        {
            print_waveform(ch,options2,&evt);
        }
        else
        {
            print_waveform(ch,options1,&evt);
            is_there[ch]=true;
        }
    }
}

std::vector<CoincidenceGroup> file_analyser::find_coincident_events(int APA, long deltaT_ns)
{
    std::vector<my_data> all_events;

    int ch_min=-40*APA+160;
    for(int i=ch_min;i<ch_min+40;i++)
    {
        for (const auto& evt : get_data_by_channel(i)) 
        {
            all_events.push_back(evt); 
        }
    }

    // Ordena por timestamp
    std::sort(all_events.begin(), all_events.end(), [](const my_data& a, const my_data& b) 
    {
        return a.Timestamp < b.Timestamp;
    });

    std::vector<CoincidenceGroup> coincidences;
    CoincidenceGroup current_group;

    for (size_t i = 0; i < all_events.size(); ++i)
    {
        if (current_group.events.empty()) 
        {
            current_group.events.push_back(all_events[i]);
        } 
        else 
        {
            long time_diff = std::abs(all_events[i].Timestamp - current_group.events.begin()->Timestamp);
            //std::cout << all_events[i].Timestamp << " "<< current_group.events.begin()->Timestamp << " "<< time_diff <<std::endl;

            if (time_diff <= deltaT_ns)
            {
                current_group.events.push_back(all_events[i]);
            } 
            else 
            {
                if (current_group.events.size() > 1) 
                {
                    coincidences.push_back(current_group);
                }
                current_group.events.clear();
                current_group.events.push_back(all_events[i]);
            }
        }
    }

    // Adiciona o último grupo
    if (current_group.events.size() > 1)
    {
        coincidences.push_back(current_group);
    }

    return coincidences;
}

std::vector<CoincidenceGroup> file_analyser::find_coincident_events_range(int APA, long deltaT_ns, long timestamp_start, long timestamp_stop)
{
    std::vector<my_data> all_events;

    bool i=false;
    int ch_min=-40*APA+160;
    long t_max;
    long t_min;
    for(int i=ch_min;i<ch_min+40;i++)
    {
        for (const auto& evt : get_data_by_channel(i)) 
        {
            long t = evt.Timestamp;
            if(!i)
            {
                t_max = t;
                t_min = t;
                i = true;
            }
            else
            {
                if(t>t_max)
                {
                    t_max = t;
                }
                if(t<t_min)
                {
                    t_min=t;
                }
            }
            if(t>=timestamp_start && t<=timestamp_stop)
                all_events.push_back(evt); 
        }
    }

    std::cout << "Searching in range:" << std::endl << t_min << " --> " << t_max << std::endl;

    // Ordena por timestamp
    std::sort(all_events.begin(), all_events.end(), [](const my_data& a, const my_data& b) 
    {
        return a.Timestamp < b.Timestamp;
    });

    std::vector<CoincidenceGroup> coincidences;
    CoincidenceGroup current_group;

    for (size_t i = 0; i < all_events.size(); ++i)
    {
        if (current_group.events.empty()) 
        {
            current_group.events.push_back(all_events[i]);
        } 
        else 
        {
            long time_diff = std::abs(all_events[i].Timestamp - current_group.events.begin()->Timestamp);
            //std::cout << all_events[i].Timestamp << " "<< current_group.events.begin()->Timestamp << " "<< time_diff <<std::endl;

            if (time_diff <= deltaT_ns)
            {
                current_group.events.push_back(all_events[i]);
            } 
            else 
            {
                if (current_group.events.size() > 1) 
                {
                    coincidences.push_back(current_group);
                }
                current_group.events.clear();
                current_group.events.push_back(all_events[i]);
            }
        }
    }

    // Adiciona o último grupo
    if (current_group.events.size() > 1)
    {
        coincidences.push_back(current_group);
    }

    return coincidences;
}

std::vector<CoincidenceGroup> file_analyser::find_coincident_events_ranges(const int APA, const long deltaT_ns,beam_time_analyser this_time_analyser,bool in_the_range)
{
    std::vector<my_data> all_events;

    bool i=false;
    int ch_min=-40*APA+160;
    long t_max;
    long t_min;

    int N_time = this_time_analyser.get_N();

    bool flag_in_the_range=true;

    for(int i=ch_min;i<ch_min+40;i++)
    {
        for (const auto& evt : get_data_by_channel(i)) 
        {
            flag_in_the_range = false;
            long t = evt.Timestamp;
            if(!i)
            {
                t_max = t;
                t_min = t;
                i = true;
            }
            else
            {
                if(t>t_max)
                {
                    t_max = t;
                }
                if(t<t_min)
                {
                    t_min=t;
                }
            }
            for(int index_time=0;index_time<N_time;index_time++)
            {
                long timestamp_start = (long)(this_time_analyser.get_beam_data_index(index_time).time/16e-9);
                long timestamp_stop = timestamp_start + (long)(4.8/16e-9);
                if(in_the_range)
                {
                    if(t>=timestamp_start && t<=timestamp_stop)
                    {
                        long pot = (long)(this_time_analyser.get_beam_data_index(index_time).pot);
                        if(pot>=1e12)
                        {
                            all_events.push_back(evt);
                            break; 
                        }
                    }
                }
                else
                {
                    if(t>=timestamp_start && t<=timestamp_stop)
                    {
                        long pot = (long)(this_time_analyser.get_beam_data_index(index_time).pot);
                        if(pot>=1e12)
                        {
                            flag_in_the_range = true;
                            break; 
                        }
                    }
                }
            }
            if(!(in_the_range) && !(flag_in_the_range))
            {
                all_events.push_back(evt); 
            }
            
        }
    }

    std::cout << "Searching in range:" << std::endl << t_min << " --> " << t_max << std::endl;

    // Ordena por timestamp
    std::sort(all_events.begin(), all_events.end(), [](const my_data& a, const my_data& b) 
    {
        return a.Timestamp < b.Timestamp;
    });

    std::vector<CoincidenceGroup> coincidences;
    CoincidenceGroup current_group;

    for (size_t i = 0; i < all_events.size(); ++i)
    {
        if (current_group.events.empty()) 
        {
            current_group.events.push_back(all_events[i]);
        } 
        else 
        {
            long time_diff = std::abs(all_events[i].Timestamp - current_group.events.begin()->Timestamp);
            //std::cout << all_events[i].Timestamp << " "<< current_group.events.begin()->Timestamp << " "<< time_diff <<std::endl;

            if (time_diff <= deltaT_ns)
            {
                current_group.events.push_back(all_events[i]);
            } 
            else 
            {
                if (current_group.events.size() > 1) 
                {
                    coincidences.push_back(current_group);
                }
                current_group.events.clear();
                current_group.events.push_back(all_events[i]);
            }
        }
    }

    // Adiciona o último grupo
    if (current_group.events.size() > 1)
    {
        coincidences.push_back(current_group);
    }

    return coincidences;
}
