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

void weight_plot_peak_analysis()
{   
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

	


	int Numjets_smeared = tree_smeared->GetEntries();
	std::cout << Numjets_smeared << " entries" << std::endl;

	std::map<int, int> pidCounts_smeared;
    std::vector<int> trackedPIDs_smeared = {11,-11, 22, 211, -211, 311, -311, 321, -321, 111, -111, 221, -221, 331, -331,
                                    130, -130, 310, -310, 213, -213, 313, -313, 323, -323, 113, -113, 223, -223,
                                    333, -333, 2212, -2212, 2112, -2112};
	int nConstituentCounts_smeared[7] = {0};
    double dtrEnergyCounts_smeared[7] = {0};
    double jetEnergyCounts_smeared[7] = {0};
	
			for (int ev = 0; ev < Numjets_smeared; ev++) //event loop
				{tree_smeared->GetEntry(ev);
				// for (int pT = 0; pT<7; pT++){   //jet loop
				// 	if (pTarray_smeared[pT]<jPt_smeared&&jPt_smeared<pTarray_smeared[pT+1]){
				
               	// 	jetCounts_smeared[pT]++;
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
							
							// double delta_eta_smeared=dtr_i_smeared.Eta()-dtr_j_smeared.Eta();
							// double delta_phi_smeared=dtr_i_smeared.Phi()-dtr_j_smeared.Phi();
							// double delta_R_smeared=sqrt(pow(delta_eta_smeared,2)+pow(delta_phi_smeared,2));
                            
							if (jE_smeared == 0) {
							continue;} 

							double EEC_smeared = dtr_i_smeared.Energy() * dtr_j_smeared.Energy()/(jE_smeared * jE_smeared);
							
							// for (int n= 0; n<7; n++)
							// {
							// double lower_bound_smeared = 0.21+n*0.01;
							// double upper_bound_smeared = lower_bound_smeared+0.01;

							// if (EEC_smeared >= lower_bound_smeared && EEC_smeared < upper_bound_smeared)
							// 	{
							// 	nConstituentCounts_smeared[n] += nConstituents_smeared;
							// 	dtrEnergyCounts_smeared[n] += dtr_i_smeared.Energy() + dtr_j_smeared.Energy();
							// 	jetEnergyCounts_smeared[n] += jE_smeared;

							// 	// Count PIDs
							// 	for (int pid_smeared : trackedPIDs_smeared)
							// 		{for (int m = 0; m < nConstituents_smeared; m++)
							// 			{if (dtr_PID_smeared->at(m) == pid_smeared)
							// 				{pidCounts_smeared[pid_smeared]++;}}}}
							// 	std::cout<< "Interval_smeared [" <<0.21+n*0.01<<", "<<0.21+(n+1)*0.01<<"):"<< std::endl;
							// 	std::cout<< "nConstituents_smeared: "<<nConstituentCounts_smeared[n]<<std::endl;
							// 	std::cout<< "dtr_energies_smeared: "<<dtrEnergyCounts_smeared[n]<<std::endl;
							// 	std::cout<< "jet energies_smeared: "<<jetEnergyCounts_smeared[n]<<std::endl;
							// 	std::cout<< "PID counts_smeared:"<<std::endl;
							// 	for (const auto& pid_smeared:trackedPIDs_smeared)
							// 	{
							// 		if (pidCounts_smeared[pid_smeared]>0)
							// 		{
							// 			std::cout << "PID"<<pid_smeared<< ": "<<pidCounts_smeared[pid_smeared]<<std::endl;
							// 		}
							// 	}
							// 	std::cout<< "------------------- "<<std::endl;
							// 	pidCounts_smeared.clear();


											// }						
														
						// 	if (0.21<=dtr_i_smeared.Energy()*dtr_j_smeared.Energy()/(jE_smeared*jE_smeared)&&dtr_i_smeared.Energy()*dtr_j_smeared.Energy()/(jE_smeared*jE_smeared)<=0.28){
						// 		std::cerr << "eventNum_smeared= " << eventNum_smeared << std::endl;
						// 		std::cerr << "jPt " << pTarray_smeared[pT] << std::endl;
						// 		std::cerr << "E1E2/Ejet^2= " << (dtr_i_smeared.Energy()*dtr_j_smeared.Energy()/(jE_smeared*jE_smeared)) << std::endl;
						// 		std::cerr << "delta_R_smeared= " << delta_R_smeared << std::endl;
						// 		std::cerr << "dtr_i_smeared.Energy= " << dtr_i_smeared.Energy() << std::endl;
						// 		std::cerr << "dtr_j_smeared.Energy= " << dtr_j_smeared.Energy() << std::endl;
						// 		std::cerr << "jE_smeared= " << jE_smeared << std::endl;
						// 		std::cerr << "dtr_i_smeared.Px= " << dtr_i_smeared.Px() << std::endl;
						// 		std::cerr << "dtr_j_smeared.Px= " << dtr_j_smeared.Px() << std::endl;
						// 		std::cerr << "dtr_i_smeared.Py= " << dtr_i_smeared.Py() << std::endl;
						// 		std::cerr << "dtr_j_smeared.Py= " << dtr_j_smeared.Py() << std::endl;
						// 		std::cerr << "dtr_i_smeared.Pz= " << dtr_i_smeared.Pz() << std::endl;
						// 		std::cerr << "dtr_j_smeared.Pz= " << dtr_j_smeared.Pz() << std::endl;
						// 		std::cerr << "dtr_i_PID_smeared= " << dtr_PID_smeared->at(i) << std::endl;
						// 		std::cerr << "dtr_j_PID_smeared= " << dtr_PID_smeared->at(j) << std::endl;
						// }
						}
					}
			
			}
	
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

	
	int Numjets_truth = tree_truth->GetEntries();
	std::cout << Numjets_truth << " entries" << std::endl;

	std::map<int, int> pidCounts_truth;
    std::vector<int> trackedPIDs_truth = {11,-11, 22, 211, -211, 311, -311, 321, -321, 111, -111, 221, -221, 331, -331,
                                    130, -130, 310, -310, 213, -213, 313, -313, 323, -323, 113, -113, 223, -223,
                                    333, -333, 2212, -2212, 2112, -2112};
	int nConstituentCounts_truth[7] = {0};
    double dtrEnergyCounts_truth[7] = {0};
    double jetEnergyCounts_truth[7] = {0};
	
			for (int ev = 0; ev < Numjets_truth; ev++) //event loop
				{tree_truth->GetEntry(ev);
				// for (int pT = 0; pT<7; pT++){   //jet loop
				// 	if (pTarray_truth[pT]<jPt_truth&&jPt_truth<pTarray_truth[pT+1]){
				
               	// 	jetCounts_truth[pT]++;
					int count=0;
					
					
					if (nConstituents_truth==1){continue;} // excluding jets with only 1 particle
					for (int i = 0; i < nConstituents_truth; i++)
						{if (dtr_PID_truth->at(i) == 11 ||dtr_PID_truth->at(i) == 22 || fabs(dtr_PID_truth->at(i)) == 2212 ||fabs(dtr_PID_truth->at(i)) == 2112  ) {
							count++;}}
					if (count==nConstituents_truth)
						{continue;}  //excluding jets where all constituent particles have PID=11 or 22 or 2212 or 2112

					for (int i = 0; i < nConstituents_truth; i++)
					{  
						if (i >= dtr_Px_truth->size() || i >= dtr_Py_truth->size() || i >= dtr_Pz_truth->size() || i >= dtr_E_truth->size()){continue;}
							TLorentzVector dtr_i_truth,dtr_j_truth;
							dtr_i_truth.SetPxPyPzE(dtr_Px_truth->at(i), dtr_Py_truth->at(i), dtr_Pz_truth->at(i), dtr_E_truth->at(i));

						for (int j = i+1; j < nConstituents_truth; j++)
						{	if (j >= dtr_Px_truth->size() || j >= dtr_Py_truth->size() || j >= dtr_Pz_truth->size() || j >= dtr_E_truth->size()){continue;}
							
							dtr_j_truth.SetPxPyPzE(dtr_Px_truth->at(j), dtr_Py_truth->at(j), dtr_Pz_truth->at(j), dtr_E_truth->at(j));
							

							// double delta_eta_truth=dtr_i_truth.Eta()-dtr_j_truth.Eta();
							// double delta_phi_truth=dtr_i_truth.Phi()-dtr_j_truth.Phi();
							// double delta_R_truth=sqrt(pow(delta_eta_truth,2)+pow(delta_phi_truth,2));
                            
							if (jE_truth == 0) {
							continue;} 
							double EEC_truth=dtr_i_truth.Energy() * dtr_j_truth.Energy()/(jE_truth * jE_truth);
							// for (int n= 0; n<7; n++)
							// {
							// double lower_bound_truth = 0.21+n*0.01;
							// double upper_bound_truth= lower_bound_truth+0.01;

							// if (EEC_truth >= lower_bound_truth && EEC_truth < upper_bound_truth)
							// 	{
							// 	nConstituentCounts_truth[n] += nConstituents_truth;
							// 	dtrEnergyCounts_truth[n] += dtr_i_truth.Energy() + dtr_j_truth.Energy();
							// 	jetEnergyCounts_truth[n] += jE_truth;

							// 	// Count PIDs
							// 	for (int pid_truth : trackedPIDs_truth)
							// 		{for (int m = 0; m < nConstituents_truth; m++)
							// 			{if (dtr_PID_truth->at(m) == pid_truth)
							// 				{pidCounts_truth[pid_truth]++;}}}}							
							// 	std::cout<< "Interval_smeared [" <<0.21+n*0.01<<", "<<0.21+(n+1)*0.01<<"):"<< std::endl;
							// 	std::cout<< "nConstituents_smeared: "<<nConstituentCounts_smeared[n]<<std::endl;
							// 	std::cout<< "dtr_energies_smeared: "<<dtrEnergyCounts_smeared[n]<<std::endl;
							// 	std::cout<< "jet energies_smeared: "<<jetEnergyCounts_smeared[n]<<std::endl;
							// 	std::cout<< "PID counts_smeared:"<<std::endl;
							// 	for (const auto& pid_smeared:trackedPIDs_smeared)
							// 	{
							// 		if (pidCounts_smeared[pid_smeared]>0)
							// 		{
							// 			std::cout << "PID"<<pid_smeared<< ": "<<pidCounts_smeared[pid_smeared]<<std::endl;
							// 		}
							// 	}
							// 	std::cout<< "------------------- "<<std::endl;
							// 	pidCounts_smeared.clear();}
							if (jPt_truth>5 && jPt_truth<10)
								{if (EEC_truth>0.24 && EEC_truth<0.25){
									std::cout << jE_truth << " " << dtr_PID_truth->at(i) <<  " " << dtr_i_truth.Energy() << " " << dtr_PID_truth->at(j)<< " " << dtr_j_truth.Energy() <<std::endl;
									};}
						// 	if (0.21<=dtr_i_truth.Energy()*dtr_j_truth.Energy()/(jE_truth*jE_truth)&&dtr_i_truth.Energy()*dtr_j_truth.Energy()/(jE_truth*jE_truth)<=0.28){
						// 		std::cerr << "eventNum_truth= " << eventNum_truth << std::endl;
						// 		std::cerr << "jPt " << pTarray_truth[pT] << std::endl;
						// 		std::cerr << "E1E2/Ejet^2= " << (dtr_i_truth.Energy()*dtr_j_truth.Energy()/(jE_truth*jE_truth)) << std::endl;
						// 		std::cerr << "delta_R_truth= " << delta_R_truth << std::endl;
						// 		std::cerr << "dtr_i_truth.Energy= " << dtr_i_truth.Energy() << std::endl;
						// 		std::cerr << "dtr_j_truth.Energy= " << dtr_j_truth.Energy() << std::endl;
						// 		std::cerr << "jE_truth= " << jE_truth << std::endl;
						// 		std::cerr << "dtr_i_truth.Px= " << dtr_i_truth.Px() << std::endl;
						// 		std::cerr << "dtr_j_truth.Px= " << dtr_j_truth.Px() << std::endl;
						// 		std::cerr << "dtr_i_truth.Py= " << dtr_i_truth.Py() << std::endl;
						// 		std::cerr << "dtr_j_truth.Py= " << dtr_j_truth.Py() << std::endl;
						// 		std::cerr << "dtr_i_truth.Pz= " << dtr_i_truth.Pz() << std::endl;
						// 		std::cerr << "dtr_j_truth.Pz= " << dtr_j_truth.Pz() << std::endl;
						//		std::cerr << "dtr_i_PID_truth= " << dtr_PID_truth->at(i) << std::endl;
						//		std::cerr << "dtr_j_PID_truth= " << dtr_PID_truth->at(j) << std::endl;
						//}
						}
					}
			
			}
	
}
