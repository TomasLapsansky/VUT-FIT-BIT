#include <simlib.h>

class Chrobak;
class KaktusPad;
class Zber;
class SpracovanieLinka;
class Spracovanie;
class Mnozenie: public Process {
    public:

    bool samec = false;
    Store *zapotek = NULL;
    KaktusPad *pad = NULL;

    Mnozenie(bool samica, Store *zapotek, KaktusPad *pad);
    Mnozenie(bool samec);
    void Behavior();
};

class OdpocetZberu: public Process {
    void Behavior();
};

void initSettings(int roky);