#include <TApplication.h> 

#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TROOT.h> 

#include <my_data.hh>

using namespace std;

void plot_histograms(int ch=41,int print=0) 
{
    // Abre o arquivo
    TFile *f = TFile::Open("data.root");
    if (!f || f->IsZombie()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return;
    }

    // Acessa a árvore
    TTree *tree = (TTree*)f->Get("my_tree");
    if (!tree)
    {
        std::cerr << "Árvore 'my_tree' não encontrada!" << std::endl;
        return;
    }

    my_data* data = nullptr;
    tree->SetBranchAddress("data", &data);

    // Cria histogramas para range = 0 e range = 1
    auto h_amp_0 = new TH1F("h_amp_0", "Amplitude;Amplitude;Normalized Entries", 150, -100, 8500);
    auto h_amp_1 = new TH1F("h_amp_1", "Amplitude;Amplitude;Normalized Entries", 150, -100, 8500);

    auto h_int_0 = new TH1F("h_int_0", "Integral;Integral;Normalized Entries", 150, -100, 100000);
    auto h_int_1 = new TH1F("h_int_1", "Integral;Integral;Normalized Entries", 150, -100, 100000);

    auto h_num_0 = new TH1F("h_num_0", "Number;Number;Normalized Entries", 150, -5, 45);
    auto h_num_1 = new TH1F("h_num_1", "Number;Number;Normalized Entries", 150, -5, 45);

    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; i++)
    {
        tree->GetEntry(i);
        if(data->Channel == ch)
        {
            if (data->range == 0)
            {
                if(data->t0 != -1000 && data->zero_crossing != -1000)
                {
                    h_amp_0->Fill(data->amplitude);
                    h_int_0->Fill(data->integral);
                    h_num_0->Fill(data->number);
                }
        
            } 
            else if (data->range == 1) 
            {
                if(data->t0 != -1000 && data->zero_crossing != -1000)
                {
                    h_amp_1->Fill(data->amplitude);
                    h_int_1->Fill(data->integral);
                    h_num_1->Fill(data->number);
                }
            }
        }
    }

    // Normalização
    h_amp_0->Scale(1.0 / h_amp_0->Integral());
    h_amp_1->Scale(1.0 / h_amp_1->Integral());

    h_int_0->Scale(1.0 / h_int_0->Integral());
    h_int_1->Scale(1.0 / h_int_1->Integral());

    h_num_0->Scale(1.0 / h_num_0->Integral());
    h_num_1->Scale(1.0 / h_num_1->Integral());

    // Cores diferentes
    h_amp_0->SetLineColor(kBlue);
    h_amp_1->SetLineColor(kRed);
    h_int_0->SetLineColor(kBlue);
    h_int_1->SetLineColor(kRed);
    h_num_0->SetLineColor(kBlue);
    h_num_1->SetLineColor(kRed);

    TCanvas *c = new TCanvas("c", "Comparation", 1200, 800);
    c->Divide(2,2);

    c->cd(1);
    h_amp_0->Draw("HIST");
    h_amp_1->Draw("HIST SAME");
    auto leg1 = new TLegend(0.7,0.75,0.9,0.9);
    leg1->AddEntry(h_amp_0, "range = beam ON", "l");
    leg1->AddEntry(h_amp_1, "range = beam OFF", "l");
    leg1->Draw();

    c->cd(2);
    h_int_0->Draw("HIST");
    h_int_1->Draw("HIST SAME");
    auto leg2 = new TLegend(0.7,0.75,0.9,0.9);
    leg2->AddEntry(h_int_0, "range = beam ON", "l");
    leg2->AddEntry(h_int_1, "range = beam OFF", "l");
    leg2->Draw();

    c->cd(3);
    h_num_0->Draw("HIST");
    h_num_1->Draw("HIST SAME");
    auto leg3 = new TLegend(0.7,0.75,0.3,0.9);
    leg3->AddEntry(h_num_0, "range = beam ON", "l");
    leg3->AddEntry(h_num_1, "range = beam OFF", "l");
    leg3->Draw();

    c->cd(4);
    gPad->Draw();

    c->Update();

    if (print == 0)
    {
        // Mostra o canvas na tela (isso é padrão em ROOT interativo)
        
        c->Draw();
    }
    else if (print == 1)
    {
        // Salva o canvas em PNG, sem mostrar na tela
        std::string filename = "./output/histograms_" + std::to_string(ch) + ".png";
        c->SaveAs(filename.c_str());
      
        // Deletar histogramas e canvas
        delete h_amp_0; delete h_amp_1;
        delete h_int_0; delete h_int_1;
        delete h_num_0; delete h_num_1;
        delete c;
        // Fecha o arquivo ROOT e limpa memória
        f->Close();
        delete f;
    }   
}

void plot_histograms_total(int print=0) 
{
    // Abre o arquivo
    TFile *f = TFile::Open("data.root");
    if (!f || f->IsZombie()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return;
    }

    // Acessa a árvore
    TTree *tree = (TTree*)f->Get("my_tree");
    if (!tree)
    {
        std::cerr << "Árvore 'my_tree' não encontrada!" << std::endl;
        return;
    }

    my_data* data = nullptr;
    tree->SetBranchAddress("data", &data);

    // Cria histogramas para range = 0 e range = 1
    auto h_amp_0 = new TH1F("h_amp_0", "Amplitude;Amplitude;Normalized Entries", 150, -100, 8500*10);
    auto h_amp_1 = new TH1F("h_amp_1", "Amplitude;Amplitude;Normalized Entries", 150, -100, 8500*10);

    auto h_int_0 = new TH1F("h_int_0", "Integral;Integral;Normalized Entries", 150, -100, 100000*10);
    auto h_int_1 = new TH1F("h_int_1", "Integral;Integral;Normalized Entries", 150, -100, 100000*10);

    auto h_num_0 = new TH1F("h_num_0", "Number;Number;Normalized Entries", 150, -5, 45);
    auto h_num_1 = new TH1F("h_num_1", "Number;Number;Normalized Entries", 150, -5, 45);

    Long64_t nentries = tree->GetEntries();
    
    int event = 0;
    int old_event = 0;

    float total_amplitude=0.0;
    float total_integral=0.0;
    int range=0;
    int number = 0;

    for (Long64_t i = 0; i < nentries; i++)
    {
        tree->GetEntry(i);
        int event = data->event;
        if(event != old_event)
        {   
            
            if(range == 0)
            {
                
                h_amp_0->Fill(total_amplitude);
                h_int_0->Fill(total_integral);
                h_num_0->Fill(number);
            }
            else if (range == 1)
            {
                
                h_amp_1->Fill(total_amplitude);
                h_int_1->Fill(total_integral);
                h_num_1->Fill(number);
            }    
            
            total_amplitude=0.0;
            total_integral=0.0;
            old_event = event;
            if(data->t0 != -1000 && data->zero_crossing != -1000)
            {
                range=data->range;
                number=data->number;
                total_amplitude+=data->amplitude;
                total_integral+=data->integral;
            }

        }
        else
        {
            if(data->t0 != -1000 && data->zero_crossing != -1000)
            {
                //std::cout<<range<<std::endl;
                range=data->range;
                number=data->number;
                total_amplitude+=data->amplitude;
                total_integral+=data->integral;
            }
        }
        
    }

    if (range == 0)
    {
        h_amp_0->Fill(total_amplitude);
        h_int_0->Fill(total_integral);
        h_num_0->Fill(number);
    } 
    else if (range == 1) 
    {
        h_amp_1->Fill(total_amplitude);
        h_int_1->Fill(total_integral);
        h_num_1->Fill(number);
    }

    // Normalização
    h_amp_0->Scale(1.0 / h_amp_0->Integral());
    h_amp_1->Scale(1.0 / h_amp_1->Integral());

    h_int_0->Scale(1.0 / h_int_0->Integral());
    h_int_1->Scale(1.0 / h_int_1->Integral());

    h_num_0->Scale(1.0 / h_num_0->Integral());
    h_num_1->Scale(1.0 / h_num_1->Integral());

    // Cores diferentes
    h_amp_0->SetLineColor(kBlue);
    h_amp_1->SetLineColor(kRed);
    h_int_0->SetLineColor(kBlue);
    h_int_1->SetLineColor(kRed);
    h_num_0->SetLineColor(kBlue);
    h_num_1->SetLineColor(kRed);

    TCanvas *c = new TCanvas("c", "Comparation", 1200, 800);
    c->Divide(2,2);

    c->cd(1);
    h_amp_0->Draw("HIST");
    h_amp_1->Draw("HIST SAME");
    auto leg1 = new TLegend(0.7,0.75,0.9,0.9);
    leg1->AddEntry(h_amp_0, "range = beam ON", "l");
    leg1->AddEntry(h_amp_1, "range = beam OFF", "l");
    leg1->Draw();

    c->cd(2);
    h_int_0->Draw("HIST");
    h_int_1->Draw("HIST SAME");
    auto leg2 = new TLegend(0.7,0.75,0.9,0.9);
    leg2->AddEntry(h_int_0, "range = beam ON", "l");
    leg2->AddEntry(h_int_1, "range = beam OFF", "l");
    leg2->Draw();

    c->cd(3);
    h_num_0->Draw("HIST");
    h_num_1->Draw("HIST SAME");
    auto leg3 = new TLegend(0.7,0.75,0.3,0.9);
    leg3->AddEntry(h_num_0, "range = beam ON", "l");
    leg3->AddEntry(h_num_1, "range = beam OFF", "l");
    leg3->Draw();

    c->cd(4);
    gPad->Draw();

    c->Update();

    if (print == 0)
    {
        // Mostra o canvas na tela (isso é padrão em ROOT interativo)
        
        c->Draw();
    }
    else if (print == 1)
    {
        // Salva o canvas em PNG, sem mostrar na tela
        std::string filename = "./output/histograms_total.png";
        c->SaveAs(filename.c_str());
      
        // Deletar histogramas e canvas
        delete h_amp_0; delete h_amp_1;
        delete h_int_0; delete h_int_1;
        delete h_num_0; delete h_num_1;
        delete c;
        // Fecha o arquivo ROOT e limpa memória
        f->Close();
        delete f;
    }   
}



int main(int argc, char** argv)
{
    TApplication theApp("App", &argc, argv);

    int ch=41;
    int print=0;

    if( argc > 1 )
    {
        ch=std::stoi(argv[1]);
    }

    if( argc > 2 )
    {
        print=std::stoi(argv[2]);
    }

    if(print == 1)
    {
        gROOT->SetBatch(kTRUE);
    }

    if(print==0 || (ch != -1 && print == 1))
    {
        if(ch != 1000)
        {
            plot_histograms(ch,print);
        }
        if(ch==1000)
        {
            plot_histograms_total(print);
        }
        
    }
    else if(print==1)
    {
        for(int i=40;i<80;i++)
        {
            plot_histograms(i,print);
        }
        plot_histograms_total(print);
    }
   

    theApp.Run();

    return 0;
}