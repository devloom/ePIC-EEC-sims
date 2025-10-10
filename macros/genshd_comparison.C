#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void genshd_comparison()
{	TLatex etaLabel;  // global for all pads
	etaLabel.SetNDC();
	etaLabel.SetTextSize(0.05);
	
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);  // <--- This hides the display of titles in plots

	double xmax = 1;
	double xmin =0.05;
	int bins_low=100;
	int bins_high=10; 
	double binwidth_low=(xmax-xmin)/bins_low;
	double binwidth_high=(xmax-xmin)/bins_high;

	int pTarray[7]={5,10,15,20,25,30,35};




    TFile *file_truth_ep = TFile::Open("test_outfile_truth_ep_GenShd_1_R_1_n_05_5M.root", "READ");
    TFile *file_truth_eAu_1 = TFile::Open("test_outfile_truth_eAu_GenShd_1_R_1_n_05_5M.root", "READ");
    TFile *file_truth_eAu_2 = TFile::Open("test_outfile_truth_eAu_GenShd_2_R_1_n_05_1M.root", "READ");
    TFile *file_truth_eAu_3 = TFile::Open("test_outfile_truth_eAu_GenShd_3_R_1_n_05_1M.root", "READ");
	



	TCanvas* cE2C = new TCanvas("cE2C", "Histogram Canvas",1500, 1000); // 800 and 600 tell the program the size of the histogram
	cE2C->Divide(4,2);
    
	// TCanvas* cE3C = new TCanvas("cE3C", "Histogram Canvas", 1500, 1000); // 800 and 600 tell the program the size of the histogram
	// cE3C->Divide(4,2);

	TCanvas* cRatio = new TCanvas("cRatio", "(eAu-ep)/ep Ratio", 1500, 1000);
    cRatio->Divide(4,2);

	

	TH1I *jetcounts_truth_ep = nullptr;
	file_truth_ep->GetObject("jetCounts",jetcounts_truth_ep);
	
    TH1I *jetcounts_truth_eAu_1 = nullptr;
	file_truth_eAu_1->GetObject("jetCounts",jetcounts_truth_eAu_1);
	
    TH1I *jetcounts_truth_eAu_2 = nullptr;
	file_truth_eAu_2->GetObject("jetCounts",jetcounts_truth_eAu_2);
    
    TH1I *jetcounts_truth_eAu_3 = nullptr;
	file_truth_eAu_3->GetObject("jetCounts",jetcounts_truth_eAu_3);

	
    std::vector<TH1D*> EEC_hist_truth_ep(5);
    std::vector<TH1D*> EEC_hist_truth_eAu_1(5);
    std::vector<TH1D*> EEC_hist_truth_eAu_2(5);
    std::vector<TH1D*> EEC_hist_truth_eAu_3(5);


	


	for (int i = 3; i < 4; ++i) 
	{       
		          
		// Get E2Cs
        std::string hist_name_truth_ep_E2C = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E2C_1 = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E2C_2 = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E2C_3 = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);

        EEC_hist_truth_ep[i] = (TH1D*)file_truth_ep->Get(hist_name_truth_ep_E2C.c_str());
        EEC_hist_truth_eAu_1[i] = (TH1D*)file_truth_eAu_1->Get(hist_name_truth_eAu_E2C_1.c_str());
        EEC_hist_truth_eAu_2[i] = (TH1D*)file_truth_eAu_2->Get(hist_name_truth_eAu_E2C_2.c_str());
        EEC_hist_truth_eAu_3[i] = (TH1D*)file_truth_eAu_3->Get(hist_name_truth_eAu_E2C_3.c_str());
        


		// E2C plots
		cE2C->cd();
		gPad->SetLeftMargin(0.15);
		double binwidth;
        if (i<4){binwidth=binwidth_low;}
		if (i>3){binwidth=binwidth_high;}


        EEC_hist_truth_ep[i]->Scale(1. / (jetcounts_truth_ep->GetBinContent(i+1)*binwidth));
        EEC_hist_truth_eAu_1[i]->Scale(1. / (jetcounts_truth_eAu_1->GetBinContent(i+1)*binwidth));
        EEC_hist_truth_eAu_2[i]->Scale(1. / (jetcounts_truth_eAu_2->GetBinContent(i+1)*binwidth));
        EEC_hist_truth_eAu_3[i]->Scale(1. / (jetcounts_truth_eAu_3->GetBinContent(i+1)*binwidth));


        std::cout << jetcounts_truth_ep->GetBinContent(4) << std::endl;
		std::cout << jetcounts_truth_eAu_1->GetBinContent(4) << std::endl;
		std::cout << jetcounts_truth_eAu_2->GetBinContent(4) << std::endl;
		std::cout << jetcounts_truth_eAu_3->GetBinContent(4) << std::endl;


		
		TLatex ptLabel;
		ptLabel.SetNDC();
        ptLabel.SetTextSize(0.05);

	
		cRatio->cd();

		TH1D* ratio_truth_ep_1 = (TH1D*)EEC_hist_truth_eAu_1[i]->Clone("ratio_truth_ep");
        ratio_truth_ep_1->Add(EEC_hist_truth_ep[i], -1);  // eAu - ep
		ratio_truth_ep_1->Divide(EEC_hist_truth_ep[i]);   // (eAu - ep)/ep
        TH1D* ratio_truth_ep_2 = (TH1D*)EEC_hist_truth_eAu_2[i]->Clone("ratio_truth_ep");
        ratio_truth_ep_2->Add(EEC_hist_truth_ep[i], -1);  // eAu - ep
		ratio_truth_ep_2->Divide(EEC_hist_truth_ep[i]);   // (eAu - ep)/ep
        TH1D* ratio_truth_ep_3 = (TH1D*)EEC_hist_truth_eAu_3[i]->Clone("ratio_truth_ep");
        ratio_truth_ep_3->Add(EEC_hist_truth_ep[i], -1);  // eAu - ep
		ratio_truth_ep_3->Divide(EEC_hist_truth_ep[i]);   // (eAu - ep)/ep

		

		ratio_truth_ep_1->SetMarkerStyle(20); 
        ratio_truth_ep_1->SetMarkerSize(1);
        ratio_truth_ep_2->SetMarkerStyle(21); 
        ratio_truth_ep_2->SetMarkerSize(1);
        ratio_truth_ep_3->SetMarkerStyle(22); 
        ratio_truth_ep_3->SetMarkerSize(1);

        ratio_truth_ep_1->SetMarkerColor(kBlue);  
        ratio_truth_ep_1->SetLineColor(kBlue);
        ratio_truth_ep_2->SetMarkerColor(kRed);   
        ratio_truth_ep_2->SetLineColor(kRed);
        ratio_truth_ep_3->SetMarkerColor(kGreen); 
        ratio_truth_ep_3->SetLineColor(kGreen);

		
		ratio_truth_ep_1->GetXaxis()->SetRangeUser(0.05, 1);
		ratio_truth_ep_1->GetYaxis()->SetRangeUser(-0.025,0.07);
		ratio_truth_ep_1->GetXaxis()->SetTitle("#theta");
		ratio_truth_ep_1->GetYaxis()->SetTitle("(eAu-ep)/ep Ratio");
		ratio_truth_ep_1->SetTitle(Form("ratio"));
        
        ratio_truth_ep_1->Draw("pe");
        ratio_truth_ep_2->Draw("pe SAME");
        ratio_truth_ep_3->Draw("pe SAME");

        TLine *line = new TLine(0, 0, 1, 0);
		line->SetLineStyle(2);
		line->Draw("same");

		ptLabel.DrawLatex(0.2, 0.75, Form("p_{T}^{jet}: %d - %d GeV", pTarray[i], pTarray[i+1]));
		ptLabel.DrawLatex(0.2, 0.7, "1 < #eta < 3.5");
        ptLabel.DrawLatex(0.2, 0.65, "n = 0.5");  // <-- added latex label


        TLegend *legendratio = new TLegend(0.65, 0.18, 1.02, 0.32);

        legendratio->SetBorderSize(0); 
        legendratio->SetFillStyle(0);
        legendratio->SetTextSize(0.045);
        legendratio->AddEntry(ratio_truth_ep_1, "GenShd = 1", "lp");
        legendratio->AddEntry(ratio_truth_ep_2, "GenShd = 2", "lp");
        legendratio->AddEntry(ratio_truth_ep_3, "GenShd = 3", "lp");
        legendratio->Draw();
		gPad->SetLogx();

	}

	

	// cE2C->SaveAs("../../simulations/plots/E2C_eA.pdf"); // Save the histogram as a PDF file

	// cE3C->SaveAs("../../simulations/plots/E3C_eA.pdf"); // Save the histogram as a PDF file

	cRatio->SaveAs("../../simulations/plots/EEC_ratio_eA.pdf");
}