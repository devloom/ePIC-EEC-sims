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

void E3C()
{

	// Open the ROOT file
    TFile *file = TFile::Open("ep_collision.root", "READ");
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
	int  nConstituents, eventNum;
	double jPt, jPx, jPy, jPz, jE, jEta,jPhi; 
	std::vector<double>* dtr_Px = nullptr;
    std::vector<double>* dtr_Py = nullptr;
    std::vector<double>* dtr_Pz = nullptr;
    std::vector<double>* dtr_E = nullptr;
    // std::vector<double>* dtr_Phi ;
    // std::vector<double>* dtr_Eta ;
    std::vector<int>* dtr_PID = nullptr;

	tree->SetBranchAddress("nConstituents", &nConstituents); 
	tree->SetBranchAddress("dtr_Px" , &dtr_Px);
	tree->SetBranchAddress("dtr_Py" , &dtr_Py);
	tree->SetBranchAddress("dtr_Pz" , &dtr_Pz);
	tree->SetBranchAddress("dtr_E" , &dtr_E);
	tree->SetBranchAddress("dtr_PID" , &dtr_PID);
	tree->SetBranchAddress("eventNum" , &eventNum);
	//tree->SetBranchAddress("dtr_Eta" , &dtr_Eta);
	//tree->SetBranchAddress("dtr_Phi" , &dtr_Phi);

	tree->SetBranchAddress("jPt" , &jPt);
	tree->SetBranchAddress("jPx" , &jPx);
	tree->SetBranchAddress("jPy" , &jPy);
	tree->SetBranchAddress("jPz" , &jPz);
	tree->SetBranchAddress("jE" , &jE);

double xmax = 1;
	double xmin =0.0005;
	int bins=100; 
	double binwidth=(xmax-xmin)/bins;
	TH1F* EEC_hist[7];
	for (int i = 0; i < 7; ++i) {
        EEC_hist[i] = new TH1F(Form("EEC_hist_%d", i), Form("%d-%d GeV", i*5, (i+1)*5), bins, xmin, xmax);
    }

	int Numjets = tree->GetEntries();
	std::cout << Numjets << " entries" << std::endl;

	int pTarray[8]={5,10,15,20,25,30,35,40};
	int jetCounts[7] = {0};
	

	for (int ev = 0; ev < Numjets; ev++)
	{
		tree->GetEntry(ev);
        for (int pT = 0; pT<7; pT++){
				if (pTarray[pT]<jPt&&jPt<pTarray[pT+1]){
		jetCounts[pT]++;		
        if (nConstituents<3) continue;
		// TLorentzVector Lc, Jet, p, pi, K;
			for (int i = 0; i < nConstituents; i++)
			{    if (i >= dtr_Px->size() || i >= dtr_Py->size() || i >= dtr_Pz->size() || i >= dtr_E->size()){continue;}
					TLorentzVector dtr_i,dtr_j,dtr_k;
					dtr_i.SetPxPyPzE(dtr_Px->at(i), dtr_Py->at(i), dtr_Pz->at(i), dtr_E->at(i));

                for (int j = i+1; j < nConstituents; j++)
					{if (j >= dtr_Px->size() || j >= dtr_Py->size() || j >= dtr_Pz->size() || j >= dtr_E->size()){continue;}
						dtr_j.SetPxPyPzE(dtr_Px->at(j), dtr_Py->at(j), dtr_Pz->at(j), dtr_E->at(j));
						double delta_eta_ij=dtr_i.Eta()-dtr_j.Eta();
                    	double delta_phi_ij=dtr_i.Phi()-dtr_j.Phi();
						double delta_R_ij=sqrt(pow(delta_eta_ij,2)+pow(delta_phi_ij,2));
					for (int k = j+1; k < nConstituents; k++)
                    	{if (k >= dtr_Px->size() || k >= dtr_Py->size() || k >= dtr_Pz->size() || k >= dtr_E->size()){continue;}
				    		dtr_k.SetPxPyPzE(dtr_Px->at(k), dtr_Py->at(k), dtr_Pz->at(k), dtr_E->at(k));

                    double delta_eta_jk=dtr_j.Eta()-dtr_k.Eta();
                    double delta_eta_ik=dtr_i.Eta()-dtr_k.Eta();
					
                    double delta_phi_jk=dtr_j.Phi()-dtr_k.Phi();
                    double delta_phi_ik=dtr_i.Phi()-dtr_k.Phi();

                    
                    double delta_R_jk=sqrt(pow(delta_eta_jk,2)+pow(delta_phi_jk,2));
                    double delta_R_ik=sqrt(pow(delta_eta_ik,2)+pow(delta_phi_ik,2));

					double delta_R=std::max({delta_R_ij,delta_R_jk,delta_R_ik});
				    EEC_hist[pT]->Fill(delta_R,dtr_i.Energy()*dtr_j.Energy()*dtr_k.Energy()/(jE*jE*jE));
                    }
                }
		}
        
	
	}}}
	//  // histogram
    
	TCanvas* c1 = new TCanvas("c1", "Histogram Canvas", 1600, 1000); // 800 and 600 tell the program the size of the histogram
	c1->Divide(4,2);
	for (int i=0; i<7; ++i){
		c1->cd(i+1);
		gPad->SetLeftMargin(0.15);
        EEC_hist[i]->Scale(1. / (jetCounts[i]*binwidth));
		EEC_hist[i]->Draw("Pe"); // Draw the histogram
		EEC_hist[i]->SetTitle("3-point EEC"); // Title
		EEC_hist[i]->GetXaxis()->SetTitle("x_{L}"); // X-Axis title (Do it for the first histogram we plot)	
		EEC_hist[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		EEC_hist[i]->GetXaxis()->SetRangeUser(0, 10);
		EEC_hist[i]->SetMaximum(10); // Set y-axis maximum
    	EEC_hist[i]->SetMinimum(0.001); // Set y-axis maximum
		gPad->SetLogx();
		gPad->SetLogy();
		TLatex latex;
    	latex.SetNDC();
    	latex.SetTextSize(0.03);
    	latex.DrawLatex(0.45, 0.85, Form("%d<pT<%d GeV",pTarray[i],pTarray[i+1]));

		}
		c1->cd(8);
		TLatex latex;
    	latex.SetNDC();
    	latex.SetTextSize(0.03);
    	latex.DrawLatex(0.5, 0.85, "1000000 events");
    	latex.DrawLatex(0.5, 0.80, "pT hat cut = 5 GeV");
    	latex.DrawLatex(0.5, 0.75, "Eta cut = +-3.5GeV");
	

    // EEC_hist->GetYaxis()->SetRangeUser(1, 1000);
    for (int i=0; i<7; ++i){std::cout<<jetCounts[i]<<std::endl;}
	c1->SaveAs("plots/E3C.pdf"); // Save the histogram as a PDF file
	TFile *ofile = new TFile("E3C.root","RECREATE");
	for (int i = 0; i < 7; ++i) {
		EEC_hist[i]->Write();}
	ofile->Write();
	ofile->Close();
}