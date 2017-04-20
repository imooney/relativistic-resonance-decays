//  rrd.C (relativistic resonance decay)
//  Created by Isaac Mooney on 4/12/17.

#include "../Event.h"
#include "TROOT.h"
#include "TTree.h"					//Added by Sam, write file
#include "TFile.h"					//Added by Sam, write file	
#include "TStyle.h"
#include <iostream>

using namespace std;

int main() {
    
    cout << "test" << endl;
    
    Track * tracktest = new Track();
    Track * trackbetter = new Track(1.1,2.3,3.2,4.5,1);
    tracktest->Print();
    trackbetter->Print();
    Event * eventtest = new Event();
    eventtest->Print();
    eventtest->AddTrack(tracktest);
    eventtest->AddTrack(trackbetter);
    eventtest->Print();
    

//////////////////////////////////////////////////////////
/////////////////  WRITE FILE  ///////////////////////////
//////////////////////////////////////////////////////////

   TFile *f=new TFile("Resonance.root","recreate");
   TTree *t1=new TTree("t1","Test 1");

   Track *forsave = new Track();
   t1->Branch("Event",&forsave,"r1/F");					//saving tracks in only 1 branch

// FILL THE TREE
  cout<<"------Filling tree-----"<<endl;
   for (Int_t i=0; i< eventtest->GetNumberOfTracks(); i++) {		// VARIABLE i, is number of tracks  
      forsave = eventtest->GetTrack(i);					//Saving track by track on temporal branch
      t1->Fill();							//Saving on tree
      forsave->Print();							//monitoring track, we can erase this line later.
   }

   t1->Write();
   f->Write();
   f->Close();

//////////////////////////////////////////////////////////

    delete eventtest; eventtest = NULL;

    return 0;
}

