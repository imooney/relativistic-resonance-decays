//  eventgen.C (relativistic resonance decay)
//  Created by Isaac Mooney on 4/12/17.

#include "../Event.h"
#include "TROOT.h"
#include "TTree.h"					//Added by Sam, write file
#include "TFile.h"					//Added by Sam, write file
#include "TStyle.h"
#include <iostream>
#include "TRandom3.h"
#include <vector>
#include "TGenPhaseSpace.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

//Globals
const Double_t Ebeam        = 200.0;        //GeV
const Double_t mProton      = 0.9382720813; //GeV
const Double_t mPion        = 0.13957018;   //GeV

const Double_t PI           = 4.0*atan(1);

const Double_t num_events   = 100000;

//Things to do:
//      1) Background particles!
//      2) Check about energies (or masses?) of daughters not exceeding parent's.
//          Revision: check that daughters + background doesn't exceed beam energy (and maybe also the above check)
//      3) Figure out why beam energy doesn't matter????? Think it's because it needs to be spread around to background too
//      4) Gaussian smearing of final pTs.
//      5) Generalize away from mid-rapidity?

Event* Decay(vector<Double_t> sampledret, Int_t evNum) {
    //STARTING WITH PROTON-PION DECAY - GENERALIZE LATER
    //Lab frame
    //cout << "Mass of Lambda: " << sampledret[3] << '\n';
    Double_t momLambda = sampledret[2];// = pT, since at mid-rapidity
    Double_t ELambda = sqrt(pow(momLambda,2) + pow(sampledret[3],2));
    //should we assume it's only along this direction?
    Double_t pxL = momLambda;
    Double_t pyL = 0;
    Double_t pzL = 0;
    Track * tracklambda = new Track(pxL, pyL, pzL, ELambda, 0);
    
    Double_t Ep = (pow(sampledret[3],2) + pow(mProton,2) - pow(mPion,2)) / (2.0*sampledret[3]);
    Double_t Epi = (pow(sampledret[3],2) - pow(mProton,2) + pow(mPion,2)) / (2.0*sampledret[3]);
    Double_t mom = sqrt(pow(Ep,2) - pow(mProton,2));
    
    Double_t phi = sampledret[0];
    Double_t theta = sampledret[1];
    
    Double_t px = mom*sin(theta)*cos(phi);
    Double_t py = mom*sin(theta)*sin(phi);
    Double_t pz = mom*cos(theta);
    
    Track * trackprot = new Track(px, py, pz, Ep, 1);
    Track * trackpion = new Track(-px, -py, -pz, Epi, -2);
    
    Event * event = new Event();
    event->SetEventNumber(evNum);
    event->AddTrack(trackprot);
    event->AddTrack(trackpion);
    //event->Print();
    //cout << "Before boost: " << '\n';
    //tracklambda->Print();
    //cout << "Decays into: " << '\n';
    //trackprot->Print();
    //trackpion->Print();
    
    trackprot->Boost(pxL/ELambda,pyL/ELambda,pzL/ELambda);
    trackpion->Boost(pxL/ELambda,pyL/ELambda,pzL/ELambda);
    //Momentum & energy of the boosted tracks sum to the lambda momentum & energy. Good check.
    
    //cout << "After boost: " << '\n';
    //trackprot->Print();
    //trackpion->Print();
    
    return event;
}

//pulls numbers from the sample distributions for each variable
vector<Double_t> sample(const unsigned index, TTree* samples) {
    //initialize more variables to be sampled if more distributions are added to sampledists.root
    Double_t philoc = -1, thetaloc = -1, ptloc = -1, massloc = -1;
    samples->SetBranchAddress("phi",&philoc);
    samples->SetBranchAddress("theta",&thetaloc);
    samples->SetBranchAddress("pt", &ptloc);
    samples->SetBranchAddress("mass", &massloc);
    //index corresponds to current event number, so kinematics change for each event
    samples->GetEntry(index);
    //add these numbers to the kinematics vector
    vector<Double_t> sampled = {philoc, thetaloc, ptloc, massloc};
    return sampled;
}

void background(vector<Double_t> sampledret, Event *eventtest, Double_t Elambda) {
    Double_t Etot   = Ebeam - Elambda;// - sampledret[2];
    //cout << Elambda << endl;
    Double_t Epart  = -1, Mpart = -1, Ppart = -1, px = -1, py = -1, pz = -1;
    Double_t phi = -1, theta = -1;
    srand (time(NULL));
    Double_t Etest = Elambda;
    while (Epart < Etot /*&& eventtest->GetNumberOfTracks() < 5*/ && Etest < Etot) {
        Mpart = fmod(rand(), Etot);
        Epart = fmod(rand(), Etot - Mpart) + Mpart;
        if (Mpart < 0.1) { continue;}
        Ppart = sqrt(pow(Epart, 2) - pow(Mpart, 2));
        
        phi = fmod(rand(), 2*PI);
        theta = fmod(rand(), PI);
        
        px = Ppart*sin(theta)*cos(phi);
        py = Ppart*sin(theta)*sin(phi);
        pz = Ppart*cos(theta);
        
        eventtest->AddTrack(px, py, pz, Epart, -99999);
        
        //Etot -= Epart;
        //cout << "Etrack: " << eventtest->GetTrack(eventtest->GetNumberOfTracks()-1)->E() << endl;
        //cout << "Etot: " << Etot << endl;
        Etest += Epart;
    }
    //cout << "TEST: " << Etest << endl;
    //cout << "TEST: " << eventtest->GetNumberOfTracks() << endl;
    return;
}

int main() {
    //open sample distributions file and get tree; create file & tree of events
    TFile *f = new TFile("Resonance.root","recreate");
    TTree *t1 = new TTree("t1","Test 1");
    TFile *samp = new TFile("sampledists.root");
    TTree *samples = (TTree*)samp->Get("treerand");
    
    //create an empty event, and put it on a branch
    Event * eventtest = new Event();
    Double_t phifinal = -1, thetafinal = -1, ptfinal = -1, massfinal = -1, mtest = -1, etest = -1;
    t1->Branch("Event",&eventtest);     t1->Branch("phi", &phifinal);
    t1->Branch("theta", &thetafinal);   t1->Branch("pt", &ptfinal);
    t1->Branch("mass", &massfinal);
    t1->Branch("mtest", &mtest); t1->Branch("etest", &etest);
    //initialize vector for sampled kinematics
    vector<Double_t> sampledret = {-1, -1, -1, -1};

    //event loop
    for (unsigned i = 0; i < num_events; ++ i) {
        //fill the vector
        sampledret = sample(i, samples);
        TGenPhaseSpace(); //FIGURE OUT HOW TO IMPLEMENT
        phifinal = sampledret[0];   thetafinal = sampledret[1];
        ptfinal = sampledret[2];    massfinal = sampledret[3];
        
        //create a decay, using the given sampled kinematics
        eventtest->SetEventNumber(i);
        eventtest = Decay(sampledret, i);
        Double_t Elambda = eventtest->GetTrack(0)->E() + eventtest->GetTrack(1)->E();
        //create background
        background(sampledret, eventtest, Elambda);
        for (unsigned j = 2; j < eventtest->GetNumberOfTracks(); ++ j) {
            mtest = eventtest->GetTrack(j)->M();
            etest = eventtest->GetTrack(j)->E();
            t1->Fill();
        }
        //fill the tree with this event
        t1->Fill();
    }
    
    delete eventtest; eventtest = NULL;
    
    //close the file of distributions
    samp->Close();
    //write the tree of events to file and close it
    t1->GetCurrentFile()->Write();
    f->Close();
    
    cout << "Tree written to and file closed" << '\n';
    
    return 0;
}
