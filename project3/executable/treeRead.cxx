// reads in the TTree 

#include "TH1F.h"
#include <TFile.h>
#include <TTree.h>
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

using namespace std;

void treeRead()
{	
	TH1F *hnseg = new TH1F("hnseg",
			"Num. of seg. for selected tracks",4096,0,8192);

	// Connect file generated in $ROOTSYS/test
	TFile fileIn("Resonance.root"); //Resonance.root is name of TTree 
	TTree *theTree = nullptr;
	TTreeReader theReader("T",&fileIn);
	TTreeReaderValue<Event> eventRV(theReader, "event");
	TTreeReaderValue<Int_T> nTracksRV(theReader, "fNtrack");

	while(theReader.Next())
	{
		if(*nTracksRV < 587) //587 is max number of tracks?
		{
			continue; //Check if we don't have too many tracks
		}
		auto event = eventRV.Get(); //Read complete accepted event in memory
		
		hnseg->Fill(event->GetNseg()); //Fill histogram w/ number of segments
	}

	hnseg->Draw();
}

// loop on a tree
// read a sub-branc only (the branch containing the number of tracks)
// read all branches, when selection is positive
// histogram the results of a member function of the event object

