/// Author(s): Mingyang Gu (Shaun), Devon Loomis ///

#include <iostream>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include "TH1.h"
#include "TFile.h"
#include <vector>
#include <cmath>
#include <TMath.h>

void E3C_pp()
{
    // Open the ROOT file
    TFile *file = TFile::Open("pp_collision.root", "READ");
    if (!file || file->IsZombie()) 
    {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

    // Get the tree
    TTree *tree = nullptr;
    file->GetObject("jets", tree); 
    if (!tree) 
    {
        std::cerr << "Error: Could not find tree." << std::endl;
        file->Close();
        return;
    }

    // Branches

    // Declare Stuff
    int nConstituents_i, nConstituents_j, eventNum;
    double jPt_i, jEta_i, jPhi_i, jPt_j, jEta_j, jPhi_j, jE_i, jE_j; 
    std::vector<double>* dtr_Px_i = nullptr;
    std::vector<double>* dtr_Py_i = nullptr;
    std::vector<double>* dtr_Pz_i = nullptr;
    std::vector<double>* dtr_E_i = nullptr;
    std::vector<int>* dtr_PID_i = nullptr;
    std::vector<double>* dtr_Px_j = nullptr;
    std::vector<double>* dtr_Py_j = nullptr;
    std::vector<double>* dtr_Pz_j = nullptr;
    std::vector<double>* dtr_E_j = nullptr;
    std::vector<int>* dtr_PID_j = nullptr;

    tree->SetBranchAddress("nConstituents_i", &nConstituents_i); 
    tree->SetBranchAddress("dtr_Px_i", &dtr_Px_i);
    tree->SetBranchAddress("dtr_Py_i", &dtr_Py_i);
    tree->SetBranchAddress("dtr_Pz_i", &dtr_Pz_i);
    tree->SetBranchAddress("dtr_E_i", &dtr_E_i);
    tree->SetBranchAddress("dtr_PID_i", &dtr_PID_i);
    tree->SetBranchAddress("eventNum", &eventNum);

    tree->SetBranchAddress("nConstituents_j", &nConstituents_j); 
    tree->SetBranchAddress("dtr_Px_j", &dtr_Px_j);
    tree->SetBranchAddress("dtr_Py_j", &dtr_Py_j);
    tree->SetBranchAddress("dtr_Pz_j", &dtr_Pz_j);
    tree->SetBranchAddress("dtr_E_j", &dtr_E_j);
    tree->SetBranchAddress("dtr_PID_j", &dtr_PID_j);

    tree->SetBranchAddress("jPt_i", &jPt_i);
    tree->SetBranchAddress("jPt_j", &jPt_j);
    tree->SetBranchAddress("jE_i", &jE_i);
    tree->SetBranchAddress("jE_j", &jE_j);
    tree->SetBranchAddress("jPhi_i", &jPhi_i);
    tree->SetBranchAddress("jPhi_j", &jPhi_j);

    double xmax = 0.3;
    double xmin = 0.0002;
    int bins = 100; 
    double binwidth = (xmax - xmin) / bins;

    TH1F* EEC_hist[8];
    for (int x = 0; x < 8; ++x) {
        EEC_hist[x] = new TH1F(Form("EEC_hist_%d", x), Form("%d-%d GeV", x*5, (x+1)*5), bins, xmin, xmax);
    }

    int Numjets = tree->GetEntries();
    std::cout << Numjets << " entries" << std::endl;

    int pTarray[9] = {97, 220, 330, 468, 638, 846, 1101, 1410, 1784};
    int jetCounts[8] = {0};

    for (int ev = 3; ev < Numjets; ev++) {
        tree->GetEntry(ev);

        for (int pT = 0; pT < 8; pT++) {
            if (pTarray[pT] < jPt_i && jPt_i < pTarray[pT+1] ) {
                jetCounts[pT]++;
                if (nConstituents_i <3) continue;

                for (int a = 0; a < nConstituents_i; a++) {    
                    if (a >= dtr_Px_i->size() ||
                                a >= dtr_Py_i->size() ||
                                a >= dtr_Pz_i->size() ||
                                a >= dtr_E_i->size()) continue;
                                TLorentzVector dtr_a, dtr_b, dtr_c;
                            dtr_a.SetPxPyPzE(dtr_Px_i->at(a), dtr_Py_i->at(a), dtr_Pz_i->at(a), dtr_E_i->at(a)); 

                    for (int b =a+1; b < nConstituents_i; b++) {
                        if (b >= dtr_Px_i->size() 
                                || b >= dtr_Py_i->size()
                                || b >= dtr_Pz_i->size() 
                                || b >= dtr_E_i->size()) continue; 
                                dtr_b.SetPxPyPzE(dtr_Px_i->at(b), dtr_Py_i->at(b), dtr_Pz_i->at(b), dtr_E_i->at(b));
                                double delta_eta_i_ab = dtr_a.Eta() - dtr_b.Eta();
                                double delta_phi_i_ab = dtr_a.Phi() - dtr_b.Phi();
                                double delta_R_i_ab = sqrt(pow(delta_eta_i_ab, 2) + pow(delta_phi_i_ab, 2));

                        for (int c = b+1; c < nConstituents_i; c++) {
                            if (c >= dtr_Px_i->size() ||
                                c >= dtr_Py_i->size() ||
                                c >= dtr_Pz_i->size() ||
                                c >= dtr_E_i->size()) continue; 
                                dtr_c.SetPxPyPzE(dtr_Px_i->at(c), dtr_Py_i->at(c), dtr_Pz_i->at(c), dtr_E_i->at(c));

                            
                            double delta_eta_i_bc = dtr_b.Eta() - dtr_c.Eta();
                            double delta_eta_i_ac = dtr_a.Eta() - dtr_c.Eta();
                            
                            double delta_phi_i_bc = dtr_b.Phi() - dtr_c.Phi();
                            double delta_phi_i_ac = dtr_a.Phi() - dtr_c.Phi();

                            double delta_R_i_bc = sqrt(pow(delta_eta_i_bc, 2) + pow(delta_phi_i_bc, 2));
                            double delta_R_i_ac = sqrt(pow(delta_eta_i_ac, 2) + pow(delta_phi_i_ac, 2));
                            double delta_R_i = std::max({delta_R_i_ab, delta_R_i_bc, delta_R_i_ac});
                            if (jE_i != 0) {
                                EEC_hist[pT]->Fill(delta_R_i, dtr_a.Energy() * dtr_b.Energy() * dtr_c.Energy() / (jE_i * jE_i * jE_i));
                            }
                        }
                    }
                }
            }
            if ( pTarray[pT] < jPt_j && jPt_j < pTarray[pT+1]) {     
                jetCounts[pT]++;
                if (nConstituents_j <3) continue;

                for (int d = 0; d < nConstituents_j; d++) {    
                            if (d >= dtr_Px_j->size() ||
                                d >= dtr_Py_j->size() ||
                                d >= dtr_Pz_j->size() ||
                                d >= dtr_E_j->size()) continue; 
                                TLorentzVector dtr_d, dtr_e, dtr_f;
                            dtr_d.SetPxPyPzE(dtr_Px_j->at(d), dtr_Py_j->at(d), dtr_Pz_j->at(d), dtr_E_j->at(d));
                    
                    for (int e = d + 1; e < nConstituents_j; e++) {
                            if ( e >= dtr_Px_j->size() ||
                              e >= dtr_Py_j->size() || 
                              e >= dtr_Pz_j->size() 
                               || e >= dtr_E_j->size() ) continue;
                               dtr_e.SetPxPyPzE(dtr_Px_j->at(e), dtr_Py_j->at(e), dtr_Pz_j->at(e), dtr_E_j->at(e)); 
                               double delta_eta_j_de = dtr_d.Eta() - dtr_e.Eta();
                                double delta_phi_j_de = dtr_d.Phi() - dtr_e.Phi();
                                double delta_R_j_de = sqrt(pow(delta_eta_j_de, 2) + pow(delta_phi_j_de, 2));
                                
                        for (int f = e + 1; f < nConstituents_j; f++) {
                            if (f >= dtr_Px_j->size() ||
                                f >= dtr_Py_j->size() ||
                                f >= dtr_Pz_j->size() ||
                                f >= dtr_E_j->size()) continue; 
                                dtr_f.SetPxPyPzE(dtr_Px_j->at(f), dtr_Py_j->at(f), dtr_Pz_j->at(f), dtr_E_j->at(f));

                            
                            double delta_eta_j_ef = dtr_e.Eta() - dtr_f.Eta();
                            double delta_eta_j_df = dtr_d.Eta() - dtr_f.Eta();

                            double delta_phi_j_ef = dtr_e.Phi() - dtr_f.Phi();
                            double delta_phi_j_df = dtr_d.Phi() - dtr_f.Phi();
                            
                            double delta_R_j_ef = sqrt(pow(delta_eta_j_ef, 2) + pow(delta_phi_j_ef, 2));
                            double delta_R_j_df = sqrt(pow(delta_eta_j_df, 2) + pow(delta_phi_j_df, 2));
                            double delta_R_j = std::max({delta_R_j_de, delta_R_j_ef, delta_R_j_df});
                            if (jE_j != 0) {
                                EEC_hist[pT]->Fill(delta_R_j, dtr_d.Energy() * dtr_e.Energy() * dtr_f.Energy() / (jE_j * jE_j * jE_j));
                            }
                        }
                    }
                }
            }
        }
    }
    

    // Histogram
    TCanvas* c1 = new TCanvas("c1", "Histogram Canvas", 1600, 1000); // 800 and 600 tell the program the size of the histogram
    c1->Divide(4, 2);
    for (int x = 0; x < 8; ++x) {
        c1->cd(x + 1);
        gPad->SetLeftMargin(0.15);
        if (jetCounts[x] > 0) {
            EEC_hist[x]->Scale(1. / (jetCounts[x] * binwidth));
        } else {
            EEC_hist[x]->Scale(0);
        }
        EEC_hist[x]->Draw("Pe"); // Draw the histogram
        EEC_hist[x]->SetTitle("3-point EEC"); // Title
        EEC_hist[x]->GetXaxis()->SetTitle("x_{L}"); // X-Axis title (Do it for the first histogram we plot)    
        EEC_hist[x]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d#sigma^{[3]}}{dx_{L}}"); // Y-Axis title
        EEC_hist[x]->GetXaxis()->SetRangeUser(0, 10);
        EEC_hist[x]->SetMaximum(100); // Set y-axis maximum
        EEC_hist[x]->SetMinimum(0.05); // Set y-axis minimum
        gPad->SetLogx();
        gPad->SetLogy();
        TLatex latex;
        latex.SetNDC();
        latex.SetTextSize(0.03);
        latex.DrawLatex(0.45, 0.85, Form("%d<pT<%d GeV", pTarray[x], pTarray[x+1]));
    }
    c1->cd(9);
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.03);
    latex.DrawLatex(0.5, 0.75, "1000000 events");
    latex.DrawLatex(0.5, 0.70, "pT hat cut = 30 GeV");
    latex.DrawLatex(0.5, 0.65, "Eta cut = +-2.1GeV");

    // Save the histogram as a PDF file
    c1->SaveAs("plots/E3C_pp.pdf"); 

    // Save the histograms to a ROOT file
    TFile *ofile = new TFile("E3C_pp.root", "RECREATE");
    for (int x = 0; x < 8; ++x) {
        EEC_hist[x]->Write();
    }
    ofile->Write();
    ofile->Close();
}
