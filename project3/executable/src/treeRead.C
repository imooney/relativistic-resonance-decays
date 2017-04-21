// reads in the TTree 

#include "TROOT.h"
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "../Event.h"

#include<iostream>

using namespace std;

int main()
{	
	

	TH1F *hnseg = new TH1F("hnseg",
			"Num. of seg. for selected tracks",4096,0,8192);

	// Connect file generated in $ROOTSYS/test
	TFile *f = new TFile("Resonance.root"); //Resonance.root is name of TTree 
	TTree *t1 = (TTree*)f->Get("t1");
	
	Event *currEvent=0;
	
	t1->SetBranchAddress("Event",&currEvent);
	t1->GetEntry(0);
	currEvent->Print();

	cout<<currEvent->GetTrack(0)->E()<<endl;
	cout<<currEvent->GetTrack(1)->E()<<endl;


	int nEvents = t1->GetEntries();
	cout<<nEvents<<endl;
	
	for(int j=0;j<nEvents;j++)
	{
		int nTracks = currEvent->GetNumberOfTracks();
		cout<<nTracks<<endl;
		for(int i=0; i<nTracks; i++)
		{
			currEvent->GetTrack(i)->Print();
		}
	}
	

	if (gROOT->IsBatch()) {return 0;}
	//new TBrowser();
	//t1->StartViewer();

	//hnseg->Draw();
	
	//delete f; //delete hnseg;

	return 0;
}

// loop on a tree
// read a sub-branc only (the branch containing the number of tracks)
// read all branches, when selection is positive
// histogram the results of a member function of the event object

