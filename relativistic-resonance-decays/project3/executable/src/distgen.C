//  distgen.C
//  Created by Isaac Mooney on 4/27/17.

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TStyle.h"
#include <iostream>
#include "TRandom3.h"
#include "TMath.h"
#include <chrono>

using namespace std;

//Globals
const Double_t Ebeam        = 200.0;        //GeV
const Double_t Gamma        = 2.631e-10;    //seconds
const Double_t PI = 4.0*atan(1);

//Things to do:
//  1) make a Breit-Wigner distribution to sample from for mass of lambda
//  2) make a decaying exponential distribution to sample from for pT of lambda

int main() {
    //create file and tree for the distributions
    TFile *sampledists = new TFile("sampledists.root", "RECREATE");
    TTree *treerand = new TTree("treerand", "sample tree");
    
    //seeding pseudorandom number generator
    auto begin = chrono::high_resolution_clock::now();
    
    Double_t phi, theta;
    TRandom3* phi_rand = new TRandom3();
    TRandom3* theta_rand = new TRandom3();
    
    auto end = chrono::high_resolution_clock::now();
    
    phi_rand->SetSeed(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count());
    theta_rand->SetSeed(0);
    
    treerand->Branch("phi", &phi);
    treerand->Branch("theta", &theta);
    
    //fill distributions, write to tree
    for(int i = 0; i < 1000000; ++ i) {
        phi = phi_rand->Uniform(0, 2*PI);
        theta = theta_rand->Uniform(0, PI);
        treerand->Fill();
    }
    
    //delete the TRandom3s
    delete phi_rand; delete theta_rand; phi_rand = NULL; theta_rand = NULL;
    
    //write to file, close the file
    sampledists->Write();
    sampledists->Close();
    
    return 0;
}
