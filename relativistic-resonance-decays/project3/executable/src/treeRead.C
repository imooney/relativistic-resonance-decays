// reads in the TTree 

#include "TROOT.h"
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "../Event.h"
#include <iostream>

using namespace std;

//Things to do:
//  1) Make histograms of information from events

int main() {
	TH1F *hnseg = new TH1F("hnseg", "Num. of seg. for selected tracks", 4096, 0, 8192);

	// Connect file generated in $ROOTSYS/test
	TFile *f = new TFile("Resonance.root"); //Resonance.root is name of TFile
	TTree *t1 = (TTree*)f->Get("t1");
	
	Event *currEvent = 0;
	t1->SetBranchAddress("Event", &currEvent);
    
    int nEvents = t1->GetEntries();
    cout << "Number of events: " << nEvents << '\n';
    
    for (unsigned i = 0; i < nEvents; ++ i) {
        t1->GetEntry(i);
        cout << "Event " << i << ": " << '\n';
        currEvent->Print();
        int nTracks = currEvent->GetNumberOfTracks();
        cout << "Number of tracks in event " << i << ": " << nTracks << '\n';
        for (unsigned j = 0; j < nTracks; ++ j) {
            cout << "Track " << j << " for event " << i << ": " << '\n';
            currEvent->GetTrack(j)->Print();
            cout << "Energy of track " << j << ": " << currEvent->GetTrack(j)->E() << '\n';
        }
    }
    
	if (gROOT->IsBatch()) {return 0;}
    
    f->Close();
	//new TBrowser();
	//t1->StartViewer();

	//hnseg->Draw();
	
	//delete f; //delete hnseg;

	return 0;
}

// loop on a tree
// read a sub-branch only (the branch containing the number of tracks)
// read all branches, when selection is positive
// histogram the results of a member function of the event object

