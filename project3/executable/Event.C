//  Event.C
//  Created by Isaac Mooney on 4/14/17.

#include "TROOT.h"
#include "Event.h"
#include <iostream>

using namespace std;

ClassImp(Track)
ClassImp(Event)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BEGIN TRACK~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


//default constructor (should probably pick a pid to default to instead of 0
Track::Track():TLorentzVector(0,0,0,0) {
    //gSystem->Load("libTrack.so"); <- for interpreting
    pid = 0;
}

//constructor with values
Track::Track(double p1, double p2, double p3, double E, int Pid):TLorentzVector(p1,p2,p3,E) {
    pid = Pid;
}

void Track::Print() {
    cout << "Track 4 momentum: ( " << Px() << ", " << Py() << ", " <<
        Pz() << ", " << E() << " )\n";
    cout << "Track pid: " << pid << '\n';
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END TRACK~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BEGIN EVENT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


//default constructor
Event::Event() {
    //gSystem->Load("../Lib/libEvent.so"); <- for interpreting
    evNum = -1; //not sure what we should default the event number to
    trackArray = new TObjArray();
    trackArray->SetOwner(kTRUE); //need to give ROOT object ownership on construction
}

//constructor with given event number
Event::Event(int Num) {
    evNum = Num;
    trackArray = new TObjArray(100);
    trackArray->SetOwner(kTRUE);
}

//destructor
Event::~Event() {
    trackArray->Delete();
    delete trackArray;
}

void Event::AddTrack(double p1, double p2, double p3, double E, int Pid) {
    Track * add = new Track(p1, p2, p3, E, Pid);
    trackArray->AddLast(add);
}

void Event::Print() {
    cout << "Event number " << GetEventNumber() << '\n';
    cout << "Number of tracks in the event: " << GetNumberOfTracks() << '\n';;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END EVENT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
