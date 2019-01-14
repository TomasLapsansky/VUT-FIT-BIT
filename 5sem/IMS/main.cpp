#include "main.hpp"
#include <iostream>
#include <vector>

using namespace std;

int endTime;

vector<KaktusPad*> kaktusy;

long finalnyProduktG = 0;

Store *dospelySamec;

Store *produkcneSamicky;
Store *usmrteneSamicky;
Store *vysuseneSamicky;
Store *samickyNaSpracovanie;
Store *finalnyProdukt;

Facility *zabijanieProdukcia;
Facility *susenieProdukcia;
Facility *cistenieProdukcia;
Facility *spracovanieProdukcia;

Facility *startOdpoctuZberu;

class Chrobak: public Process {

    Store *vajicka;
    Store *larva;
    Store *samec;
    Store *kukla;
    Store *samica;
    Store *dospelaSamica;

    public:
    Chrobak(Store *vajicka, Store *larva, Store *samec, Store *kukla, Store *samica, Store *dospelaSamica) {
        this->vajicka = vajicka;
        this->larva = larva;
        this->samec = samec;
        this->kukla = kukla;
        this->samica = samica;
        this->dospelaSamica = dospelaSamica;
    }

    void Behavior() {
        Enter(*(this->vajicka), 1);

        Wait(Uniform(37.78, 85.78));

        if(Random() <= 0.7) {   /* 70% sanca umrtia */
            Leave(*vajicka, 1);
            Cancel();
        }

        Leave(*vajicka, 1);

        Enter(*larva, 1);

        Wait(Exponential(60*24*2));

        if(Random() <= 0.56) {   /* 56% sanca umrtia */
            Leave(*larva, 1);
            Cancel();
        }

        Leave(*larva, 1);

        if(Random() <= 0.5) {   /* 50% sanca ze som samec */
            /* Som samec */
            Enter(*samec, 1);

            Wait(Uniform(60*24*20-1, 60*24*20+1));

            if(Random() <= 0.07) {   /* 7% sanca umrtia */
                Leave(*samec, 1);
                Cancel();
            }

            Leave(*samec, 1);
            Enter(*kukla, 1);

            Wait(Uniform(60*24*60-3, 60*24*60+3));

            Leave(*kukla, 1);
            Enter(*dospelySamec, 1);

            (new Mnozenie(true))->Activate();
            (new OdpocetZberu)->Activate();

            Wait(Uniform(60*24*3, 60*24*5));


        } else {
            /* Som samica */
            Enter(*samica,1);

            Wait(Uniform(60*24*45-2.25, 60*24*45+2.25));

            if(Random() <= 0.07) {   /* 7% sanca umrtia */
                Leave(*samica, 1);
                Cancel();
            }

            Leave(*samica, 1);
            Enter(*dospelaSamica, 1);
        }
    }
};

class KaktusPad: public Process {

    public:
    KaktusPad() {
        kaktusy.push_back(this);
    }

    Store *vajicka;
    Store *larva;
    Store *samec;
    Store *kukla;
    Store *samica;

    Store *dospelaSamica;
    Store *generacnaSamica;
    Store *zapotek;
    Store *oplodnenaSamica;

    void initStores() {
        vajicka = new Store("Vajicka", 10000);
        larva = new Store("Larva", 10000);
        samec = new Store("Samec", 10000);
        kukla = new Store("Kukla", 10000);
        samica = new Store("Samica", 10000);
        dospelaSamica = new Store("Dospela samica", 10000);
        generacnaSamica = new Store("Generacna samica", 100);   //rezerva
        zapotek = new Store("Zapotek", 100);                    //rezerva
        oplodnenaSamica = new Store("Oplodnena samica", 100);   //rezerva
    }

    void Behavior() {
        initStores();
        int pocetVajicok = Uniform(180, 220) * 20;
        for(int i = 0; i < pocetVajicok; i++) {
            (new Chrobak(vajicka, larva, samec, kukla, samica, dospelaSamica))->Activate();
        }

        Wait(endTime);
    }
};

class Spracovanie: public Process{

    void Behavior() {
        Seize(*zabijanieProdukcia, 0);

        while(!(produkcneSamicky->Empty())) {
            Leave(*produkcneSamicky, 1);
            Enter(*usmrteneSamicky, 1);
        }

        Release(*zabijanieProdukcia);
        
        Seize(*susenieProdukcia, 0);

        while(!(usmrteneSamicky->Empty())) {
            Leave(*usmrteneSamicky, 1);
            Enter(*vysuseneSamicky, 1);
        }

        Release(*susenieProdukcia);
        
        Seize(*cistenieProdukcia, 0);

        while(!(vysuseneSamicky->Empty())) {
            Leave(*vysuseneSamicky, 1);
            Enter(*samickyNaSpracovanie, 1);
        }

        Release(*cistenieProdukcia);
        
        Seize(*spracovanieProdukcia, 0);

        while(!(samickyNaSpracovanie->Empty())) {
            Leave(*samickyNaSpracovanie, 1);
            Enter(*finalnyProdukt, 7142);
            finalnyProduktG += 7142;
        }

        Release(*spracovanieProdukcia);
    }
};

class SpracovanieLinka: public Process{

    void Behavior() {

        Seize(*zabijanieProdukcia, 1);

        (new Spracovanie)->Activate();

        Wait(Uniform(5, 6));

        Release(*zabijanieProdukcia);

        Seize(*susenieProdukcia, 1);

        Wait(Uniform(60*24*2, 60*24*3));

        Release(*susenieProdukcia);

        Seize(*cistenieProdukcia, 1);

        Wait(Uniform(4, 5));

        Release(*cistenieProdukcia);

        Seize(*spracovanieProdukcia, 1);

        Wait(60*24*2);

        Release(*spracovanieProdukcia);
    }
};

class Zber: public Process{

    void Behavior() {

        static int cnt = 0;
        cnt++;

        cout << "---------------------Zber " << cnt << " ---------------------" << endl;
        for(unsigned long i = 0; i < kaktusy.size(); i++) {
            Wait(1/2);
            int count = 20;
            while(!(kaktusy[i]->dospelaSamica->Empty())) {
                if(count != 0) {
                    Leave(*(kaktusy[i]->dospelaSamica), 1);
                    Enter(*(kaktusy[i]->generacnaSamica), 1);
                    count--;
                } else {
                    Leave(*(kaktusy[i]->dospelaSamica), 1);
                    Enter(*produkcneSamicky, 1);
                }
            }

            Wait(1/2);
            while(!(kaktusy[i]->generacnaSamica->Empty())) {
                Leave(*(kaktusy[i]->generacnaSamica), 1);
                Enter(*(kaktusy[i]->zapotek), 1);

                (new Mnozenie(true, kaktusy[i]->zapotek, kaktusy[i]))->Activate();
            }
        }

        (*produkcneSamicky).Output();

        (new SpracovanieLinka)->Activate();

        //(new Zber)->Activate(Time+80*24*60);
    }

};

//class OdpocetZberu: public Process {

    void OdpocetZberu::Behavior() {
        if(!startOdpoctuZberu->Busy()) {
            Seize(*startOdpoctuZberu, 1);
            cout << "--- Odpocet noveho zberu ---" << endl;
            /* Reinicializacia samiciek */
            produkcneSamicky = new Store("Pocet samiciek na produkciu", kaktusy.size()*10000);
            Wait(60*24*80);
            cout << "--- Koniec odpoctu zberu ---" << endl;
            (new Zber)->Activate();
            Release(*startOdpoctuZberu);
        } else {
            Cancel();
        }
    }
//};

class OplodnenaSamica: public Process {
    public:

    KaktusPad *pad;

    OplodnenaSamica(KaktusPad *pad) {
        this->pad = pad;
    }

    void Behavior() {
        if(!(pad->oplodnenaSamica->Empty())) {
            Leave(*(pad->oplodnenaSamica), 1);
            Wait(60*24*3);
            int pocetVajec = Uniform(180, 220);
            for(int i = 0; i < pocetVajec; i++) {
                (new Chrobak(pad->vajicka, pad->larva, pad->samec, pad->kukla, pad->samica, pad->dospelaSamica))->Activate();
            }
        }
    }
};

//class Mnozenie: public Process{

    //public:

    //bool samec = false;
    //Store *zapotek = NULL;
    //KaktusPad *pad = NULL;

    Mnozenie::Mnozenie(bool samica, Store *zapotek, KaktusPad *pad) {
        this->samec = !samica;
        this->zapotek = zapotek;
        this->pad = pad;
    }

    Mnozenie::Mnozenie(bool samec) {
        this->samec = samec;
    }

    void Mnozenie::Behavior() {
        if(samec) {
            for(unsigned long i = 0; i < kaktusy.size(); i++) {
                if(!kaktusy[i]->zapotek->Empty()) {
                    Leave(*dospelySamec, 1);
                    Leave(*(kaktusy[i]->zapotek), 1);
                    Wait(1/2);
                    Enter(*(kaktusy[i]->oplodnenaSamica), 1);
                    (new OplodnenaSamica(kaktusy[i]))->Activate();
                    return;
                }
            }
        } else {
            if(!dospelySamec->Empty()) {
                Leave(*dospelySamec, 1);
                Leave(*zapotek, 1);
                Wait(1/2);
                Enter(*(pad->oplodnenaSamica), 1);
                (new OplodnenaSamica(pad))->Activate();
            }
        }
    }
//};

void initSettings(int roky) {
    dospelySamec = new Store("Dospely samec", 10000);
    produkcneSamicky = new Store("Pocet samiciek na produkciu", kaktusy.size()*10000);
    usmrteneSamicky = new Store("Usmrtene samicky", kaktusy.size()*10000);
    vysuseneSamicky = new Store("Vysusene samicky", kaktusy.size()*10000);
    samickyNaSpracovanie = new Store("Samicky na spracovanie", kaktusy.size()*10000);
    finalnyProdukt = new Store("Finalny produkt (10^-6 g)", kaktusy.size()*10000*7142*roky);

    zabijanieProdukcia = new Facility("Zabijanie samic (produkcia)");
    susenieProdukcia = new Facility("Sesenie samic (produkcia)");
    cistenieProdukcia = new Facility("Cistenie samic od prebytocnych necistot (produkcia)");
    spracovanieProdukcia = new Facility("Spracovanie do finalneho produktu (produkcia)");

    startOdpoctuZberu = new Facility("Odstartovany odpocet zberu (80 dni)");
}

int main(int argc, char const *argv[])
{
    if(argc != 3) {
        cerr << "wrong number of arguments" << endl;
        return 1;
    }

    /* upravenie kalendara na queue, pre urychlenie pre mnozstvo procesov */
    SetCalendar("cq");

    int pocetKaktusy = strtol(argv[1], NULL, 10);
    int roky = strtol(argv[2], NULL, 10);

    endTime = 60*24*365*roky;
    RandomSeed(time(NULL));
    Init(0,endTime);

    for(int i = 0; i < pocetKaktusy; i++) {
        (new KaktusPad)->Activate();
    }

    (new Zber)->Activate(80*24*60);

    initSettings(roky);

    Run();

    //(*dospelySamec).Output();
    (*finalnyProdukt).Output();

    return 0;
}
