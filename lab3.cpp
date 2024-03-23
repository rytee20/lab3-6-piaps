#include <iostream>
#include "Airplane.h"

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "RUS");

    Airplane* airplane = createAirplane();

    if (airplane->isReady()) {
        if (airplane->excessWeight()) {
            airplane->laggageRemoval();
        }
    }
}