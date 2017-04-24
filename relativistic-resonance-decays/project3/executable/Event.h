//  Event.h
//  Created by Isaac Mooney on 4/12/17.

#ifndef ROOT_Event
#define ROOT_Event

#include "TROOT.h"
#include "TObjArray.h"
#include "TLorentzVector.h"
#include "TObject.h"
#include "TBuffer.h"

class Track : public TLorentzVector {
    //setters, getters, and boost inherited from TLorentzVector
private:
    int pid; // p = +1 for proton, pi- = -2 (sign for charge, number for particle)
    
public:
    //constructors and destructor
    Track(); //default
    Track(double, double, double, double, int); //when only given its components
    
    virtual ~Track() {}//destructor
    
    //getters and setters
    void SetPid(int Pid) {pid = Pid;}
    int GetPid() {return pid;}
    
    void Print();
    
    ClassDef(Track, 1)
};

class Event : public TObject {
protected:
    int evNum; //event number, not number of events
    TObjArray *trackArray;
    
public:
    //constructors and destructor
    Event(); //default
    Event(int);
    
    virtual ~Event(); //destructor
    
    //getters and setters
    void SetEventNumber(int n) {evNum = n;}
    void AddTrack(Track * part) {trackArray->AddLast(part);}
    void AddTrack(double, double, double, double, int);
    Track* GetTrack(int test) {return (Track*) trackArray->At(test);}
    int GetEventNumber() {return evNum;}
    int GetNumberOfTracks() {return (int) trackArray->GetEntries();}
    
    void Print();
    
    ClassDef(Event, 1)
};

#endif /* ROOT_Event */
