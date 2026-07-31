#include "../include/HistoManager.h"


R__LOAD_LIBRARY(../lib/libEnergyCorrelator.so)

void eA_EEC()
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

	int pTarray[7]={5,10,15,20,30,35,40};

    TFile *file_truth_ep = TFile::Open("/home/miyagu/ePIC-EEC-sims/macros/great_lakes_data/truth_EEC_ep_high_pt_genshd_1_merged.root", "READ");
    TFile *file_truth_eAu = TFile::Open("/home/miyagu/ePIC-EEC-sims/macros/great_lakes_data/truth_EEC_eAu_high_pt_genshd_1_merged.root", "READ");
	TFile *file_smeared_ep = TFile::Open("/home/miyagu/ePIC-EEC-sims/macros/great_lakes_data/smeared_EEC_ep_high_pt_genshd_1_merged.root", "READ");
    TFile *file_smeared_eAu = TFile::Open("/home/miyagu/ePIC-EEC-sims/macros/great_lakes_data/smeared_EEC_eAu_high_pt_genshd_1_merged.root", "READ");

	// TFile *file_truth_ep = TFile::Open("test_outfile_truth_ep_GenShd_1_R_1_n_05_5M.root", "READ");
    // TFile *file_truth_eAu = TFile::Open("test_outfile_truth_eAu_GenShd_1_R_1_n_05_5M.root", "READ");
	// TFile *file_smeared_ep = TFile::Open("test_outfile_smeared_ep_GenShd_1_R_1_n_05_5M.root", "READ");
    // TFile *file_smeared_eAu = TFile::Open("test_outfile_smeared_eAu_GenShd_1_R_1_n_05_5M.root", "READ");



	TCanvas* cE2C = new TCanvas("cE2C", "Histogram Canvas",1500, 1000); // 800 and 600 tell the program the size of the histogram
	cE2C->Divide(4,2);
    
	TCanvas* cE3C = new TCanvas("cE3C", "Histogram Canvas", 1500, 1000); // 800 and 600 tell the program the size of the histogram
	cE3C->Divide(4,2);

	TCanvas* cRatio = new TCanvas("cRatio", "(eAu-ep)/ep Ratio", 1500, 1000);
    cRatio->Divide(4,2);

	

	TH1I *jetcountsE2C_truth_ep    = nullptr, *jetcountsE3C_truth_ep    = nullptr;
	TH1I *jetcountsE2C_truth_eAu   = nullptr, *jetcountsE3C_truth_eAu   = nullptr;
	TH1I *jetcountsE2C_smeared_ep  = nullptr, *jetcountsE3C_smeared_ep  = nullptr;
	TH1I *jetcountsE2C_smeared_eAu = nullptr, *jetcountsE3C_smeared_eAu = nullptr;

	file_truth_ep   ->GetObject("jetCounts_E2C", jetcountsE2C_truth_ep);
	file_truth_ep   ->GetObject("jetCounts_E3C", jetcountsE3C_truth_ep);

	file_truth_eAu  ->GetObject("jetCounts_E2C", jetcountsE2C_truth_eAu);
	file_truth_eAu  ->GetObject("jetCounts_E3C", jetcountsE3C_truth_eAu);

	file_smeared_ep ->GetObject("jetCounts_E2C", jetcountsE2C_smeared_ep);
	file_smeared_ep ->GetObject("jetCounts_E3C", jetcountsE3C_smeared_ep);

	file_smeared_eAu->GetObject("jetCounts_E2C", jetcountsE2C_smeared_eAu);
	file_smeared_eAu->GetObject("jetCounts_E3C", jetcountsE3C_smeared_eAu);

	

	
    std::vector<TH1D*> EEC_hist_truth_ep(5);
    std::vector<TH1D*> EEC_hist_truth_eAu(5);
	std::vector<TH1D*> EEC_hist_smeared_ep(5);
    std::vector<TH1D*> EEC_hist_smeared_eAu(5);

	std::vector<TH1D*> E3C_hist_truth_ep(5);
    std::vector<TH1D*> E3C_hist_truth_eAu(5);
	std::vector<TH1D*> E3C_hist_smeared_ep(5);
    std::vector<TH1D*> E3C_hist_smeared_eAu(5);

	


	for (int i = 3; i < 4; ++i) 
	{       
		          
		// Get E2Cs
        std::string hist_name_truth_ep_E2C = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E2C = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
		std::string hist_name_smeared_ep_E2C = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_smeared_eAu_E2C = Form("eec_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);

        EEC_hist_truth_ep[i] = (TH1D*)file_truth_ep->Get(hist_name_truth_ep_E2C.c_str());
        EEC_hist_truth_eAu[i] = (TH1D*)file_truth_eAu->Get(hist_name_truth_eAu_E2C.c_str());
		EEC_hist_smeared_ep[i] = (TH1D*)file_smeared_ep->Get(hist_name_smeared_ep_E2C.c_str());
        EEC_hist_smeared_eAu[i] = (TH1D*)file_smeared_eAu->Get(hist_name_smeared_eAu_E2C.c_str());

		// Get E3Cs
		std::string hist_name_truth_ep_E3C = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_truth_eAu_E3C = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
		std::string hist_name_smeared_ep_E3C = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);
        std::string hist_name_smeared_eAu_E3C = Form("E3C_theta_pt_bin_%d_%d", pTarray[i], pTarray[i+1]);

        E3C_hist_truth_ep[i] = (TH1D*)file_truth_ep->Get(hist_name_truth_ep_E3C.c_str());
        E3C_hist_truth_eAu[i] = (TH1D*)file_truth_eAu->Get(hist_name_truth_eAu_E3C.c_str());
		E3C_hist_smeared_ep[i] = (TH1D*)file_smeared_ep->Get(hist_name_smeared_ep_E3C.c_str());
        E3C_hist_smeared_eAu[i] = (TH1D*)file_smeared_eAu->Get(hist_name_smeared_eAu_E3C.c_str());


		// E2C plots
		cE2C->cd();
		gPad->SetLeftMargin(0.15);
		double binwidth;
        if (i<4){binwidth=binwidth_low;}
		if (i>3){binwidth=binwidth_high;}


        EEC_hist_truth_ep[i]->Scale(1. / (jetcountsE2C_truth_ep->GetBinContent(i+1)),"width");
        EEC_hist_truth_eAu[i]->Scale(1. / (jetcountsE2C_truth_eAu->GetBinContent(i+1)),"width");
		EEC_hist_smeared_ep[i]->Scale(1. / (jetcountsE2C_smeared_ep->GetBinContent(i+1)),"width");
        EEC_hist_smeared_eAu[i]->Scale(1. / (jetcountsE2C_smeared_eAu->GetBinContent(i+1)),"width");
		std::cout << jetcountsE2C_truth_ep->GetBinContent(4) << std::endl;
		std::cout << jetcountsE2C_truth_eAu->GetBinContent(4) << std::endl;
		std::cout << jetcountsE2C_smeared_ep->GetBinContent(4) << std::endl;
		std::cout << jetcountsE2C_smeared_eAu->GetBinContent(4) << std::endl;
		// EEC_hist_truth_ep[i]->Rebin(2);
		// EEC_hist_truth_ep[i]->Scale(0.5);
		EEC_hist_truth_ep[i]->GetXaxis()->SetLimits(0.05, 1);
		// EEC_hist_truth_ep[i]->SetMaximum(1000.); // Set y-axis maximum
    	// EEC_hist_truth_ep[i]->SetMinimum(0.03); // Set y-axis maximum
		EEC_hist_truth_ep[i]->SetLineColor(kRed); 
		EEC_hist_truth_ep[i]->SetMarkerSize(1);
		EEC_hist_truth_ep[i]->SetMarkerStyle(21);
		EEC_hist_truth_ep[i]->SetMarkerColor(kRed);

		THStack *hsE2C = new THStack(Form("hsE2C_%d", i), "");

		// EEC_hist_truth_ep[i]->Draw("pe"); // Draw the histogram

		// EEC_hist_truth_ep[i]->GetXaxis()->SetTitle("#deltaR");
		// EEC_hist_truth_ep[i]->GetXaxis()->SetTitle("#theta");
		// EEC_hist_truth_ep[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{d#theta}");
		// EEC_hist_truth_eAu[i]->Rebin(2);
		// EEC_hist_truth_eAu[i]->Scale(0.5);
        EEC_hist_truth_eAu[i]->GetXaxis()->SetLimits(0.05, 1);
		// EEC_hist_truth_eAu[i]->SetMaximum(1000.); // Set y-axis maximum
    	// EEC_hist_truth_eAu[i]->SetMinimum(0.03); // Set y-axis maximum
        EEC_hist_truth_eAu[i]->SetLineColor(kBlue); 
		EEC_hist_truth_eAu[i]->SetMarkerSize(1);
		EEC_hist_truth_eAu[i]->SetMarkerStyle(20);
		EEC_hist_truth_eAu[i]->SetMarkerColor(kBlue);

		// EEC_hist_truth_eAu[i]->Draw("pe SAME"); // Draw the histogram

		// hsE2C->Add(EEC_hist_truth_ep[i]);
		hsE2C->Add(EEC_hist_truth_eAu[i]);
		
		// EEC_hist_truth_eAu[i]->GetXaxis()->SetTitle("#theta");
		// EEC_hist_truth_eAu[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{d#theta}");
		// hsE2C->Draw("NOSTACK P");   // P = draw markers (important)
		// hsE2C->GetXaxis()->SetTitle("#theta");
		// hsE2C->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d#sigma^{[2]}}{d#theta}");

		EEC_hist_smeared_ep[i]->GetXaxis()->SetLimits(0.05, 1);
		// EEC_hist_smeared_ep[i]->SetMaximum(1000.); // Set y-axis maximum
    	// EEC_hist_smeared_ep[i]->SetMinimum(0.03); // Set y-axis maximum
		EEC_hist_smeared_ep[i]->SetLineColor(kGreen); 
		EEC_hist_smeared_ep[i]->SetMarkerSize(1);
		EEC_hist_smeared_ep[i]->SetMarkerStyle(21);
		EEC_hist_smeared_ep[i]->SetMarkerColor(kGreen);
		// EEC_hist_smeared_ep[i]->Draw("pe"); // Draw the histogram
		// hsE2C->Add(EEC_hist_smeared_ep[i]);
		hsE2C->Add(EEC_hist_smeared_eAu[i]);
		// EEC_hist_smeared_ep[i]->GetXaxis()->SetTitle("#deltaR");
		// EEC_hist_smeared_ep[i]->GetXaxis()->SetTitle("#theta");
		// EEC_hist_smeared_ep[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{d#theta}");
		// EEC_hist_smeared_eAu[i]->Rebin(2);
		// EEC_hist_smeared_eAu[i]->Scale(0.5);
        EEC_hist_smeared_eAu[i]->GetXaxis()->SetLimits(0.05, 1);
		// EEC_hist_smeared_eAu[i]->SetMaximum(1000.); // Set y-axis maximum
    	// EEC_hist_smeared_eAu[i]->SetMinimum(0.03); // Set y-axis maximum
        EEC_hist_smeared_eAu[i]->SetLineColor(kMagenta); 
		EEC_hist_smeared_eAu[i]->SetMarkerSize(1);
		EEC_hist_smeared_eAu[i]->SetMarkerStyle(20);
		EEC_hist_smeared_eAu[i]->SetMarkerColor(kMagenta);
		// EEC_hist_smeared_eAu[i]->Draw("pe SAME"); // Draw the histogram
		// EEC_hist_smeared_eAu[i]->GetXaxis()->SetTitle("#theta");
		// EEC_hist_smeared_eAu[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[2]}}{d#theta}");
		hsE2C->Draw("NOSTACK P");   // P = draw markers (important)
		hsE2C->GetXaxis()->SetTitle("#theta");
		hsE2C->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d#sigma^{[2]}}{d#theta}");

		gPad->Update();
		hsE2C->GetXaxis()->SetRangeUser(0.05, 1);
		gPad->Modified();
		gPad->Update();

		

		
		TLatex ptLabel;
		ptLabel.SetNDC();
		ptLabel.SetTextSize(0.04);
		ptLabel.DrawLatex(0.2, 0.65, "400M/100M events");
		ptLabel.DrawLatex(0.2, 0.6, "18x110 GeV beam");
		ptLabel.DrawLatex(0.2, 0.55, " Q^{2}: 700-10000 GeV^{2}");
		ptLabel.DrawLatex(0.2, 0.5, "GenShd=1");
		ptLabel.DrawLatex(0.2, 0.45, "1 < #eta < 3.5");
		ptLabel.DrawLatex(0.2, 0.4, "jet R=1.0");
		ptLabel.DrawLatex(0.2, 0.35, Form("p_{T}^{jet}: %d - %d GeV/c", pTarray[i], pTarray[i+1]));
        ptLabel.DrawLatex(0.2, 0.3, "n = 0.5");
		
		TLegend *legendE2C = new TLegend(0.55, 0.2, 0.92, 0.35);  // Position the legend
		legendE2C->SetBorderSize(0); 
		legendE2C->SetFillStyle(0);
		legendE2C->SetTextSize(0.05);
		// legendE2C->AddEntry(EEC_hist_truth_ep[i], "truth_ep", "p");
        legendE2C->AddEntry(EEC_hist_truth_eAu[i], "truth_eAu", "p");
		// legendE2C->AddEntry(EEC_hist_smeared_ep[i], "smeared_ep", "p");
        legendE2C->AddEntry(EEC_hist_smeared_eAu[i], "smeared_eAu", "p");
		

		legendE2C->Draw();
		
		gPad->SetLogx();
		gPad->SetLogy();

		


		// E3C plots
		cE3C->cd();
		gPad->SetLeftMargin(0.15);

        if (i<4){binwidth=binwidth_low;}
		if (i>3){binwidth=binwidth_high;}


        E3C_hist_truth_ep[i]->Scale(1. / (jetcountsE3C_truth_ep->GetBinContent(i+1)),"width");
        E3C_hist_truth_eAu[i]->Scale(1. / (jetcountsE3C_truth_eAu->GetBinContent(i+1)),"width");
		E3C_hist_smeared_ep[i]->Scale(1. / (jetcountsE3C_smeared_ep->GetBinContent(i+1)),"width");
        E3C_hist_smeared_eAu[i]->Scale(1. / (jetcountsE3C_smeared_eAu->GetBinContent(i+1)),"width");
		std::cout << jetcountsE3C_truth_ep->GetBinContent(4) << std::endl;
		std::cout << jetcountsE3C_truth_eAu->GetBinContent(4) << std::endl;
		std::cout << jetcountsE3C_smeared_ep->GetBinContent(4) << std::endl;
		std::cout << jetcountsE3C_smeared_eAu->GetBinContent(4) << std::endl;

		E3C_hist_truth_ep[i]->GetXaxis()->SetLimits(0.05, 1);
		// E3C_hist_truth_ep[i]->SetMaximum(1000.); // Set y-axis maximum/
    	// E3C_hist_truth_ep[i]->SetMinimum(0.03); // Set y-axis maximum
		E3C_hist_truth_ep[i]->SetLineColor(kRed); 
		E3C_hist_truth_ep[i]->SetMarkerSize(1);
		E3C_hist_truth_ep[i]->SetMarkerStyle(21);
		E3C_hist_truth_ep[i]->SetMarkerColor(kRed);
		
		THStack *hsE3C = new THStack(Form("hsE3C_%d", i), "");
		hsE3C->Add(E3C_hist_truth_ep[i]);
		hsE3C->Add(E3C_hist_truth_eAu[i]);

		// E3C_hist_truth_ep[i]->Draw("pe"); // Draw the histogram
		// // E3C_hist_truth_ep[i]->GetXaxis()->SetTitle("#deltaR");
		// E3C_hist_truth_ep[i]->GetXaxis()->SetTitle("#theta");
		// E3C_hist_truth_ep[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{d#theta}");

        E3C_hist_truth_eAu[i]->GetXaxis()->SetLimits(0.05, 1);
		// E3C_hist_truth_eAu[i]->SetMaximum(1000.); // Set y-axis maximum
    	// E3C_hist_truth_eAu[i]->SetMinimum(0.03); // Set y-axis maximum
        E3C_hist_truth_eAu[i]->SetLineColor(kBlue); 
		E3C_hist_truth_eAu[i]->SetMarkerSize(1);
		E3C_hist_truth_eAu[i]->SetMarkerStyle(20);
		E3C_hist_truth_eAu[i]->SetMarkerColor(kBlue);
		// E3C_hist_truth_eAu[i]->Draw("pe SAME"); // Draw the histogram
		// E3C_hist_truth_eAu[i]->GetXaxis()->SetTitle("#theta");
		// E3C_hist_truth_eAu[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{d#theta}");

		E3C_hist_smeared_ep[i]->GetXaxis()->SetLimits(0.05, 1);
		// E3C_hist_smeared_ep[i]->SetMaximum(1000.); // Set y-axis maximum
    	// E3C_hist_smeared_ep[i]->SetMinimum(0.03); // Set y-axis maximum
		E3C_hist_smeared_ep[i]->SetLineColor(kGreen); 
		E3C_hist_smeared_ep[i]->SetMarkerSize(1);
		E3C_hist_smeared_ep[i]->SetMarkerStyle(21);
		E3C_hist_smeared_ep[i]->SetMarkerColor(kGreen);
		// E3C_hist_smeared_ep[i]->Draw("pe"); // Draw the histogram
		// E3C_hist_smeared_ep[i]->GetXaxis()->SetTitle("#deltaR");
		// E3C_hist_smeared_ep[i]->GetXaxis()->SetTitle("#theta");
		// E3C_hist_smeared_ep[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{d#theta}");

        E3C_hist_smeared_eAu[i]->GetXaxis()->SetLimits(0.05, 1);
		// E3C_hist_smeared_eAu[i]->SetMaximum(1000.); // Set y-axis maximum
    	// E3C_hist_smeared_eAu[i]->SetMinimum(0.03); // Set y-axis maximum
        E3C_hist_smeared_eAu[i]->SetLineColor(kMagenta); 
		E3C_hist_smeared_eAu[i]->SetMarkerSize(1);
		E3C_hist_smeared_eAu[i]->SetMarkerStyle(20);
		E3C_hist_smeared_eAu[i]->SetMarkerColor(kMagenta);
		// E3C_hist_smeared_eAu[i]->Draw("pe SAME"); // Draw the histogram
		// E3C_hist_smeared_eAu[i]->GetXaxis()->SetTitle("#theta");
		// E3C_hist_smeared_eAu[i]->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d #sigma^{[3]}}{d#theta}");
		hsE3C->Draw("NOSTACK P");   // P = draw markers (important)
		hsE3C->GetXaxis()->SetTitle("#theta");
		hsE3C->GetYaxis()->SetTitle("#frac{1}{N_{jet}} #frac{d#sigma^{[3]}}{d#theta}");

		gPad->Update();
		hsE3C->GetXaxis()->SetRangeUser(0.05, 1);
		gPad->Modified();
		gPad->Update();


		ptLabel.SetNDC();
		ptLabel.SetTextSize(0.04);
		ptLabel.DrawLatex(0.2, 0.65, "400M/100M events");
		ptLabel.DrawLatex(0.2, 0.6, "18x110 GeV beam");
		ptLabel.DrawLatex(0.2, 0.55, " Q^{2}: 700-10000 GeV^{2}");
		ptLabel.DrawLatex(0.2, 0.5, "GenShd=1");
		ptLabel.DrawLatex(0.2, 0.45, "1 < #eta < 3.5");
		ptLabel.DrawLatex(0.2, 0.4, "jet R=1.0");
		ptLabel.DrawLatex(0.2, 0.35, Form("p_{T}^{jet}: %d - %d GeV/c", pTarray[i], pTarray[i+1]));
        ptLabel.DrawLatex(0.2, 0.3, "n = 0.5");
		
		TLegend *legendE3C = new TLegend(0.55, 0.2, 0.92, 0.35);  // Position the legend
		legendE3C->SetBorderSize(0); 
		legendE3C->SetFillStyle(0);
		legendE3C->SetTextSize(0.05);
		legendE3C->AddEntry(EEC_hist_truth_ep[i], "truth_ep", "p");
        legendE3C->AddEntry(EEC_hist_truth_eAu[i], "truth_eAu", "p");
		// legendE3C->AddEntry(E3C_hist_smeared_ep[i], "smeared_ep", "p");
        // legendE3C->AddEntry(E3C_hist_smeared_eAu[i], "smeared_eAu", "p");

		

		legendE3C->Draw();
		
		gPad->SetLogx();
		gPad->SetLogy();

/////////////////////////////////////////////////////////////////////////////////////////////////

	
		cRatio->cd();

		// EEC_hist_truth_eAu[i]->Sumw2();
		// EEC_hist_truth_ep[i]->Sumw2();
		// EEC_hist_smeared_eAu[i]->Sumw2();
		// EEC_hist_smeared_ep[i]->Sumw2();

		TH1D* ratio_truth_ep = (TH1D*)EEC_hist_truth_eAu[i]->Clone("ratio_truth_ep");
		ratio_truth_ep->Add(EEC_hist_truth_ep[i], -1);  // eAu - ep
		ratio_truth_ep->Divide(EEC_hist_truth_ep[i]);   // (eAu - ep)/ep

		ratio_truth_ep->SetMarkerStyle(20);
		ratio_truth_ep->SetMarkerSize(1);
		ratio_truth_ep->SetMarkerColor(kRed);
		ratio_truth_ep->SetLineColor(kRed); 

		ratio_truth_ep->Draw("pe"); // Draw truth ratio with points and error bars
		TLine *line = new TLine(0.05, 0, 1, 0);
		line->SetLineStyle(2);
		line->Draw("same");
		ratio_truth_ep->GetXaxis()->SetRangeUser(0.05, 1);
		// ratio_truth_ep->GetYaxis()->SetRangeUser(-0.025,0.07);
		ratio_truth_ep->GetYaxis()->SetRangeUser(-0.04,0.14);
		// ratio_truth_ep->GetYaxis()->SetRangeUser(0.5,1.35);
		ratio_truth_ep->GetXaxis()->SetTitle("#theta");
		ratio_truth_ep->GetYaxis()->SetTitle("(eAu-ep)/ep Ratio");
		// ratio_truth_ep->GetYaxis()->SetTitle("eAu/ep Ratio");
		ratio_truth_ep->SetTitle(Form("ratio"));
		
		
		TH1D* ratio_smeared_ep = (TH1D*)EEC_hist_smeared_eAu[i]->Clone("ratio_smeared_ep");
		ratio_smeared_ep->Add(EEC_hist_smeared_ep[i], -1);  // eAu - ep
		ratio_smeared_ep->Divide(EEC_hist_smeared_ep[i]);   // (eAu - ep)/ep
		ratio_smeared_ep->SetMarkerStyle(21);
		ratio_smeared_ep->SetMarkerSize(1);
		ratio_smeared_ep->SetMarkerColor(kBlue);
		ratio_smeared_ep->SetLineColor(kBlue); 

		ratio_smeared_ep->Draw("pe SAME"); // Draw smeared ratio with points and error bars
		line = new TLine(0.05, 0, 1, 0);
		line->SetLineStyle(2);
		line->Draw("same");
		ratio_smeared_ep->GetXaxis()->SetRangeUser(0.05, 1);
		// ratio_smeared_ep->GetYaxis()->SetRangeUser(-0.025,0.07);
		ratio_smeared_ep->GetYaxis()->SetRangeUser(-0.04,0.14);
		// ratio_smeared_ep->GetYaxis()->SetRangeUser(0.5,1.35);
		ratio_smeared_ep->GetXaxis()->SetTitle("#theta");
		ratio_smeared_ep->GetYaxis()->SetTitle("(eAu-ep)/ep Ratio");
		// ratio_smeared_ep->GetYaxis()->SetTitle("eAu/ep Ratio");
		ratio_smeared_ep->SetTitle(Form("ratio"));
		
		// THStack *hsRatio = new THStack(Form("hsRatio_%d", i), "");

		// hsRatio->Add(ratio_truth_ep);
		// hsRatio->Add(ratio_smeared_ep);
		// hsRatio->Draw("NOSTACK");
		// TH1* hAxis = hsRatio->GetHistogram();
		// hAxis->GetXaxis()->SetLimits(0.05, 1);
		// hAxis->GetXaxis()->SetTitle("#theta");
		// hAxis->GetYaxis()->SetTitle("(eAu - ep) / ep");
		// hAxis->GetYaxis()->SetTitle("eAu/ep");
		

		// TLine *line = new TLine(0.05, 1, 1, 1);
		// line->SetLineStyle(2);
		// line->Draw("same");

		TLegend *legendRatio = new TLegend(0.55, 0.1, 0.85, 0.2);
		// TLegend *legendRatio = new TLegend(0.55, 0.4, 0.85, 0.5);
		legendRatio->SetBorderSize(0);
		legendRatio->SetFillStyle(0);
		legendRatio->SetTextSize(0.05);
		legendRatio->AddEntry(ratio_truth_ep, "truth (eAu-ep)/ep", "p");
		legendRatio->AddEntry(ratio_smeared_ep, "smeared (eAu-ep)/ep", "p");
		legendRatio->Draw();

		ptLabel.SetTextSize(0.03);
		ptLabel.DrawLatex(0.15, 0.85, "400M/100M events");
		ptLabel.DrawLatex(0.15, 0.8, "18x110 GeV beam");
		ptLabel.DrawLatex(0.15, 0.75, " Q^{2}: 700-10000 GeV^{2}");
		ptLabel.DrawLatex(0.15, 0.7, "GenShd=1");
		ptLabel.DrawLatex(0.15, 0.65, "1 < #eta < 3.5");
		ptLabel.DrawLatex(0.15, 0.6, "jet R=1.0");
		ptLabel.DrawLatex(0.15, 0.55, Form("p_{T}^{jet}: %d - %d GeV/c", pTarray[i], pTarray[i+1]));
        ptLabel.DrawLatex(0.15, 0.5, "n = 0.5");
		

		
		gPad->SetLogx();
		gPad->Modified();
		gPad->Update();


	}

	

	cE2C->SaveAs("../../simulations/plots/E2C_eA.pdf"); // Save the histogram as a PDF file

	cE3C->SaveAs("../../simulations/plots/E3C_eA.pdf"); // Save the histogram as a PDF file

	cRatio->SaveAs("../../simulations/plots/EEC_ratio_eA.pdf");
}