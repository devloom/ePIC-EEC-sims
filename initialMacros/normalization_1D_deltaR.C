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
#include <TLegend.h>

void normalization_1D_deltaR()
{

	// Open the ROOT file
    TFile *file_unsmeared = TFile::Open("ep_collision.root", "READ");
    if (!file_unsmeared || file_unsmeared->IsZombie()) 
	{
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

	// Get the tree
    TTree *tree_unsmeared = nullptr;
    file_unsmeared->GetObject("jets", tree_unsmeared); 
    if (!tree_unsmeared) 
	{
        std::cerr << "Error: Could not find tree." << std::endl;
        file_unsmeared->Close();
        return;
    }

	// Branches

	// Declare Stuff
	int  nConstituents_unsmeared, eventNum_unsmeared;
	double jPt_unsmeared, jPx_unsmeared, jPy_unsmeared, jPz_unsmeared, jE_unsmeared, jEta_unsmeared,jPhi_unsmeared; 
	std::vector<double>* dtr_Px_unsmeared = nullptr;
    std::vector<double>* dtr_Py_unsmeared = nullptr;
    std::vector<double>* dtr_Pz_unsmeared = nullptr;
    std::vector<double>* dtr_E_unsmeared = nullptr;
    // std::vector<double>* dtr_Phi ;
    // std::vector<double>* dtr_Eta ;
    std::vector<int>* dtr_PID_unsmeared = nullptr;

	tree_unsmeared->SetBranchAddress("nConstituents", &nConstituents_unsmeared); 
	tree_unsmeared->SetBranchAddress("dtr_Px" , &dtr_Px_unsmeared);
	tree_unsmeared->SetBranchAddress("dtr_Py" , &dtr_Py_unsmeared);
	tree_unsmeared->SetBranchAddress("dtr_Pz" , &dtr_Pz_unsmeared);
	tree_unsmeared->SetBranchAddress("dtr_E" , &dtr_E_unsmeared);
	tree_unsmeared->SetBranchAddress("dtr_PID" , &dtr_PID_unsmeared);
	tree_unsmeared->SetBranchAddress("eventNum" , &eventNum_unsmeared);
	//tree->SetBranchAddress("dtr_Eta" , &dtr_Eta);
	//tree->SetBranchAddress("dtr_Phi" , &dtr_Phi);

	tree_unsmeared->SetBranchAddress("jPt" , &jPt_unsmeared);
	tree_unsmeared->SetBranchAddress("jPx" , &jPx_unsmeared);
	tree_unsmeared->SetBranchAddress("jPy" , &jPy_unsmeared);
	tree_unsmeared->SetBranchAddress("jPz" , &jPz_unsmeared);
	tree_unsmeared->SetBranchAddress("jE" , &jE_unsmeared);

	double xmax = 3;
	double xmin =0;
	int bins_low=500;
	int bins_high=10; 
	double binwidth_low=(xmax-xmin)/bins_low;
	double binwidth_high=(xmax-xmin)/bins_high;

	TH1F* EEC_hist_unsmeared[7];
	for (int i = 0; i < 4; ++i) {
        EEC_hist_unsmeared[i] = new TH1F(Form("EEC_hist_%d", i), Form("%d-%d GeV", i*5, (i+1)*5), 100, 0, 3);
    }
	for (int i = 4; i < 7; ++i) {
        EEC_hist_unsmeared[i] = new TH1F(Form("EEC_hist_%d", i), Form("%d-%d GeV", i*5, (i+1)*5), 100, 0, 3);
    }
	int Numjets_unsmeared = tree_unsmeared->GetEntries();
	std::cout << Numjets_unsmeared << " entries" << std::endl;

	int pTarray_unsmeared[8]={5,10,15,20,25,30,35,40};
	int jetCounts_unsmeared[7] = {0};
	
			for (int ev = 0; ev < Numjets_unsmeared; ev++)
			{tree_unsmeared->GetEntry(ev);
				for (int pT = 0; pT<7; pT++){
				if (pTarray_unsmeared[pT]<jPt_unsmeared&&jPt_unsmeared<pTarray_unsmeared[pT+1]){

                jetCounts_unsmeared[pT]++;
				
				if (nConstituents_unsmeared==1) continue;
				// TLorentzVector Lc, Jet, p, pi, K;
					for (int i = 0; i < nConstituents_unsmeared; i++)
					{    if (i >= dtr_Px_unsmeared->size() || i >= dtr_Py_unsmeared->size() || i >= dtr_Pz_unsmeared->size() || i >= dtr_E_unsmeared->size()){continue;}
							TLorentzVector dtr_i_unsmeared,dtr_j_unsmeared;
							dtr_i_unsmeared.SetPxPyPzE(dtr_Px_unsmeared->at(i), dtr_Py_unsmeared->at(i), dtr_Pz_unsmeared->at(i), dtr_E_unsmeared->at(i));

						for (int j = 0; j < nConstituents_unsmeared; j++)
						{	if (j >= dtr_Px_unsmeared->size() || j >= dtr_Py_unsmeared->size() || j >= dtr_Pz_unsmeared->size() || j >= dtr_E_unsmeared->size()){continue;}
							
							dtr_j_unsmeared.SetPxPyPzE(dtr_Px_unsmeared->at(j), dtr_Py_unsmeared->at(j), dtr_Pz_unsmeared->at(j), dtr_E_unsmeared->at(j));
							
							double delta_eta_unsmeared=dtr_i_unsmeared.Eta()-dtr_j_unsmeared.Eta();
							double delta_phi_unsmeared=dtr_i_unsmeared.Phi()-dtr_j_unsmeared.Phi();
							double delta_R_unsmeared=sqrt(pow(delta_eta_unsmeared,2)+pow(delta_phi_unsmeared,2));
							EEC_hist_unsmeared[pT]->Fill(delta_R_unsmeared);
							
						}
					}
				
			
			}}}
	//  // histogram
    

    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    // Open the ROOT file
    TFile *file_smeared = TFile::Open("0_2_smeared_cluster_into_jets.root", "READ");
    if (!file_smeared || file_smeared->IsZombie()) 
	{
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

	// Get the tree
    TTree *tree_smeared = nullptr;
    file_smeared->GetObject("jets", tree_smeared); 
    if (!tree_smeared) 
	{
        std::cerr << "Error: Could not find tree." << std::endl;
        file_smeared->Close();
        return;
    }
	// Branches

	// Declare Stuff
	int  nConstituents_smeared, eventNum_smeared;
	double jPt_smeared, jPx_smeared, jPy_smeared, jPz_smeared, jE_smeared, jeta_smeared,jPhi_smeared; 
	std::vector<double>* dtr_Px_smeared = nullptr;
    std::vector<double>* dtr_Py_smeared = nullptr;
    std::vector<double>* dtr_Pz_smeared = nullptr;
    std::vector<double>* dtr_E_smeared = nullptr;
    std::vector<int>* dtr_PID_smeared = nullptr;

	tree_smeared->SetBranchAddress("nConstituents", &nConstituents_smeared); 
	tree_smeared->SetBranchAddress("dtr_Px" , &dtr_Px_smeared);
	tree_smeared->SetBranchAddress("dtr_Py" , &dtr_Py_smeared);
	tree_smeared->SetBranchAddress("dtr_Pz" , &dtr_Pz_smeared);
	tree_smeared->SetBranchAddress("dtr_E" , &dtr_E_smeared);
	tree_smeared->SetBranchAddress("dtr_PID" , &dtr_PID_smeared);
	tree_smeared->SetBranchAddress("eventNum" , &eventNum_smeared);
    tree_smeared->SetBranchAddress("jPt" , &jPt_smeared);
	tree_smeared->SetBranchAddress("jE" , &jE_smeared);
	tree_smeared->SetBranchAddress("jeta" , &jeta_smeared);

	

	TH1F* EEC_hist_smeared[7];
	for (int i = 0; i < 4; ++i) {
        EEC_hist_smeared[i] = new TH1F(Form("EEC_hist_%d", i), Form("%d-%d GeV", i*5, (i+1)*5),100, 0, 3);
    }
	for (int i = 4; i < 7; ++i) {
        EEC_hist_smeared[i] = new TH1F(Form("EEC_hist_%d", i), Form("%d-%d GeV", i*5, (i+1)*5), 100, 0, 3);
    }

	int Numjets_smeared = tree_smeared->GetEntries();
	std::cout << Numjets_smeared << " entries" << std::endl;

	int pTarray_smeared[8]={5,10,15,20,25,30,35,40};
	int jetCounts_smeared[7] = {0};
	
	
			for (int ev = 0; ev < Numjets_smeared; ev++) //event loop
				{tree_smeared->GetEntry(ev);
				for (int pT = 0; pT<7; pT++){   //jet loop
					if (pTarray_smeared[pT]<jPt_smeared&&jPt_smeared<pTarray_smeared[pT+1]){
				
               		jetCounts_smeared[pT]++;
					int count=0;
				
					if (nConstituents_smeared==1){continue;} // excluding jets with only 1 particle
					for (int i = 0; i < nConstituents_smeared; i++)
						{if (dtr_PID_smeared->at(i) == 0) {
							count++;}}
					if (count==nConstituents_smeared)
						{continue;}  //excluding jets where all constituent particles have PID=0

					for (int i = 0; i < nConstituents_smeared; i++)
					{  
						if (i >= dtr_Px_smeared->size() || i >= dtr_Py_smeared->size() || i >= dtr_Pz_smeared->size() || i >= dtr_E_smeared->size()){continue;}
							TLorentzVector dtr_i_smeared,dtr_j_smeared;
							dtr_i_smeared.SetPxPyPzE(dtr_Px_smeared->at(i), dtr_Py_smeared->at(i), dtr_Pz_smeared->at(i), dtr_E_smeared->at(i));

						for (int j = i+1; j < nConstituents_smeared; j++)
						{	if (j >= dtr_Px_smeared->size() || j >= dtr_Py_smeared->size() || j >= dtr_Pz_smeared->size() || j >= dtr_E_smeared->size()){continue;}
							
							dtr_j_smeared.SetPxPyPzE(dtr_Px_smeared->at(j), dtr_Py_smeared->at(j), dtr_Pz_smeared->at(j), dtr_E_smeared->at(j));
							
							double delta_eta_smeared=dtr_i_smeared.Eta()-dtr_j_smeared.Eta();
							double delta_phi_smeared=fabs(dtr_i_smeared.Phi()-dtr_j_smeared.Phi());
							if (delta_phi_smeared > 3.1415926){delta_phi_smeared-=(2*3.1415926);}
							double delta_R_smeared=sqrt(pow(delta_eta_smeared,2)+pow(delta_phi_smeared,2));
                            
							if (jE_smeared != 0) {
							EEC_hist_smeared[pT]->Fill(delta_R_smeared);} 
							else {
								  std::cerr << "delta_R_smeared= " << delta_R_smeared << std::endl;
								  std::cerr << "dtr_i_smeared.Energy= " << dtr_i_smeared.Energy() << std::endl;
								  std::cerr << "dtr_j_smeared.Energy= " << dtr_j_smeared.Energy() << std::endl;
								  std::cerr << "jE_smeared= " << jE_smeared << std::endl;
						          std::cerr << "Invalid data detected for bin 0, event " << ev << std::endl;}

						}
					}
				
			
			}}}
//---------------------------------------------------------------------------------------------------------------
     TFile *file_truth = TFile::Open("truth_jets.root", "READ");
    if (!file_truth || file_truth->IsZombie()) 
	{
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

	// Get the tree
    TTree *tree_truth = nullptr;
    file_truth->GetObject("jets", tree_truth); 
    if (!tree_truth) 
	{
        std::cerr << "Error: Could not find tree." << std::endl;
        file_truth->Close();
        return;
    }
	// Branches

	// Declare Stuff
	int  nConstituents_truth, eventNum_truth;
	double jPt_truth, jPx_truth, jPy_truth, jPz_truth, jE_truth, jeta_truth,jPhi_truth; 
	std::vector<double>* dtr_Px_truth = nullptr;
    std::vector<double>* dtr_Py_truth = nullptr;
    std::vector<double>* dtr_Pz_truth = nullptr;
    std::vector<double>* dtr_E_truth = nullptr;
    std::vector<int>* dtr_PID_truth = nullptr;

	tree_truth->SetBranchAddress("nConstituents", &nConstituents_truth); 
	tree_truth->SetBranchAddress("dtr_Px" , &dtr_Px_truth);
	tree_truth->SetBranchAddress("dtr_Py" , &dtr_Py_truth);
	tree_truth->SetBranchAddress("dtr_Pz" , &dtr_Pz_truth);
	tree_truth->SetBranchAddress("dtr_E" , &dtr_E_truth);
	tree_truth->SetBranchAddress("dtr_PID" , &dtr_PID_truth);
	tree_truth->SetBranchAddress("eventNum" , &eventNum_truth);
    tree_truth->SetBranchAddress("jPt" , &jPt_truth);
	tree_truth->SetBranchAddress("jE" , &jE_truth);
	tree_truth->SetBranchAddress("jeta" , &jeta_truth);

	

	TH1F* EEC_hist_truth[7];
	for (int i = 0; i < 4; ++i) {
        EEC_hist_truth[i] = new TH1F(Form("EEC_hist_%d", i), Form("%d-%d GeV", i*5, (i+1)*5),100, 0, 3);
    }
	for (int i = 4; i < 7; ++i) {
        EEC_hist_truth[i] = new TH1F(Form("EEC_hist_%d", i), Form("%d-%d GeV", i*5, (i+1)*5),100, 0, 3);
    }

	int Numjets_truth = tree_truth->GetEntries();
	std::cout << Numjets_truth << " entries" << std::endl;

	int pTarray_truth[8]={5,10,15,20,25,30,35,40};
	int jetCounts_truth[7] = {0};
	
	
			for (int ev = 0; ev < Numjets_truth; ev++)
				{tree_truth->GetEntry(ev);
				for (int pT = 0; pT<7; pT++){   //jet loop
					if (pTarray_truth[pT]<jPt_truth&&jPt_truth<pTarray_truth[pT+1]){
				
               		jetCounts_truth[pT]++;
					int count=0;
				
					if (nConstituents_truth==1){continue;} // excluding jets with only 1 particle
					for (int i = 0; i < nConstituents_truth; i++)
						{if (dtr_PID_truth->at(i) == 11 ||dtr_PID_truth->at(i) == 22) {
							count++;}}
					if (count==nConstituents_truth)
						{continue;}  //excluding jets where all constituent particles have PID=11 or 22

					for (int i = 0; i < nConstituents_truth; i++)
					{  
						
						if (i >= dtr_Px_truth->size() || i >= dtr_Py_truth->size() || i >= dtr_Pz_truth->size() || i >= dtr_E_truth->size()){continue;}
							TLorentzVector dtr_i_truth,dtr_j_truth;
							dtr_i_truth.SetPxPyPzE(dtr_Px_truth->at(i), dtr_Py_truth->at(i), dtr_Pz_truth->at(i), dtr_E_truth->at(i));

						for (int j = i+1; j < nConstituents_truth; j++)
						{	if (j >= dtr_Px_truth->size() || j >= dtr_Py_truth->size() || j >= dtr_Pz_truth->size() || j >= dtr_E_truth->size()){continue;}
							
							dtr_j_truth.SetPxPyPzE(dtr_Px_truth->at(j), dtr_Py_truth->at(j), dtr_Pz_truth->at(j), dtr_E_truth->at(j));
							
							double delta_eta_truth=dtr_i_truth.Eta()-dtr_j_truth.Eta();
							double delta_phi_truth=fabs(dtr_i_truth.Phi()-dtr_j_truth.Phi());
							if (delta_phi_truth > 3.1415926){delta_phi_truth-=(2*3.1415926);}
							double delta_R_truth=sqrt(pow(delta_eta_truth,2)+pow(delta_phi_truth,2));
                            
							
							EEC_hist_truth[pT]->Fill(delta_R_truth);

						}
					}
				
			
			}}}
	// //  // histogram
    // // cout<<" Before Tcanvas it ran "<< endl;
   
	TCanvas* c1 = new TCanvas("c1", "Histogram Canvas", 1600, 1000); // 800 and 600 tell the program the size of the histogram
	c1->Divide(4,2);
	for (int i=0; i<4; ++i){
		c1->cd(i+1);
		gPad->SetLeftMargin(0.15);
        // EEC_hist_unsmeared[i]->Scale(1. / (jetCounts_unsmeared[i]*binwidth));
		// EEC_hist_unsmeared[i]->Draw("Pe"); // Draw the histogram
        // EEC_hist_unsmeared[i]->SetLineColor(kBlue); 
        EEC_hist_smeared[i]->Scale(1. / (jetCounts_smeared[i]*binwidth_low));
		EEC_hist_smeared[i]->Draw("Pe"); // Draw the histogram
        EEC_hist_smeared[i]->SetLineColor(kRed); 
        EEC_hist_truth[i]->Scale(1. / (jetCounts_truth[i]*binwidth_low));
		EEC_hist_truth[i]->Draw("Pe SAME"); // Draw the histogram
        EEC_hist_truth[i]->SetLineColor(kGreen); 
		EEC_hist_smeared[i]->SetTitle("deltaR"); // Title
		EEC_hist_smeared[i]->GetXaxis()->SetTitle("deltaR"); // X-Axis title (Do it for the first histogram we plot)	
		// EEC_hist_smeared[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		// EEC_hist_smeared[i]->GetXaxis()->SetRangeUser(0, 10);
		// EEC_hist_smeared[i]->SetMaximum(10); // Set y-axis maximum
    	// EEC_hist_smeared[i]->SetMinimum(0.001); // Set y-axis maximum
		std::cout <<1. / (jetCounts_smeared[i]*binwidth_low)<< std::endl;
		// gPad->SetLogx();
		gPad->SetLogy();

		TLatex latex;
    	latex.SetNDC();
    	latex.SetTextSize(0.03);
    	latex.DrawLatex(0.45, 0.85, Form("%d<pT<%d GeV",pTarray_smeared[i],pTarray_smeared[i+1]));
        TLegend *legend = new TLegend(0.25, 0.25, 0.47, 0.35);  // Position the legend
        // legend->AddEntry(EEC_hist_unsmeared[i], "summer_truth", "l");
        legend->AddEntry(EEC_hist_smeared[i], "smeared", "l");
        legend->AddEntry(EEC_hist_truth[i], "truth", "l");
        legend->SetTextSize(0.03);
        // legend->Draw();
		}
		for (int i=4; i<6; ++i){
		c1->cd(i+1);
		gPad->SetLeftMargin(0.15);
        // EEC_hist_unsmeared[i]->Scale(1. / (jetCounts_unsmeared[i]*binwidth));
		// EEC_hist_unsmeared[i]->Draw("Pe"); // Draw the histogram
        // EEC_hist_unsmeared[i]->SetLineColor(kBlue); 
        EEC_hist_smeared[i]->Scale(1. / (jetCounts_smeared[i]*binwidth_high));
		EEC_hist_smeared[i]->Draw("Pe"); // Draw the histogram
        EEC_hist_smeared[i]->SetLineColor(kRed); 
        EEC_hist_truth[i]->Scale(1. / (jetCounts_truth[i]*binwidth_high));
		EEC_hist_truth[i]->Draw("Pe SAME"); // Draw the histogram
        EEC_hist_truth[i]->SetLineColor(kGreen); 
		EEC_hist_smeared[i]->SetTitle("deltaR"); // Title
		EEC_hist_smeared[i]->GetXaxis()->SetTitle("deltaR"); // X-Axis title (Do it for the first histogram we plot)	
		// EEC_hist_smeared[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{dx_{L}}");
		// EEC_hist_smeared[i]->GetXaxis()->SetRangeUser(0, 10);
		// EEC_hist_smeared[i]->SetMaximum(10); // Set y-axis maximum
    	// EEC_hist_smeared[i]->SetMinimum(0.001); // Set y-axis maximum
		std::cout <<1. / (jetCounts_smeared[i]*binwidth_high)<< std::endl;
		// gPad->SetLogx();
		gPad->SetLogy();

		TLatex latex;
    	latex.SetNDC();
    	latex.SetTextSize(0.03);
    	latex.DrawLatex(0.45, 0.85, Form("%d<pT<%d GeV",pTarray_smeared[i],pTarray_smeared[i+1]));
        TLegend *legend = new TLegend(0.25, 0.25, 0.47, 0.35);  // Position the legend
        // legend->AddEntry(EEC_hist_unsmeared[i], "summer_truth", "l");
        legend->AddEntry(EEC_hist_smeared[i], "smeared", "l");
        legend->AddEntry(EEC_hist_truth[i], "truth", "l");
        legend->SetTextSize(0.03);
        // legend->Draw();
		}
		c1->cd(8);
		TLatex latex;
    	latex.SetNDC();
    	latex.SetTextSize(0.03);
    	latex.DrawLatex(0.5, 0.85, "1000000 events");
    	latex.DrawLatex(0.5, 0.80, "pT hat cut = 5 GeV");
    	latex.DrawLatex(0.5, 0.75, "Eta cut = +-3.5GeV");
		latex.DrawLatex(0.5, 0.70, "jet radius= 1.0");

		// latex.DrawLatex(0.5, 0.60, Form("there are %d summer truth jets",Numjets_unsmeared));
		latex.DrawLatex(0.5, 0.50, Form("there are %d truth jets",Numjets_truth));
		latex.DrawLatex(0.5, 0.55, Form("there are %d smeared jets",Numjets_smeared));

        c1->cd();
        c1->Update();


    // EEC_hist->GetYaxis()->SetRangeUser(1, 1000);

	
	c1->SaveAs("plots/normalization_1D_deltaR.pdf"); // Save the histogram as a PDF file

	//for (int i = 0; i < 7; ++i) {
		//EEC_hist_smeared[i]->Write();
		//delete EEC_hist_smeared[i];
		//}
	
    std::cout << "Final integral for pT bin 0: " << EEC_hist_smeared[0]->Integral() << std::endl;
	
    
	
}
