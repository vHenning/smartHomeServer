#include <iostream>

#include "LED/LEDController.h"

int main (int, char**)
{
    // LEDController controller("192.168.0.116");
    LEDController controller("192.168.0.115");

    int red = 0x00;
    int green = 0x00;
    int blue = 0x00;
    float dim = 1.0;

    while(true)
    {
        std::cout << "Red (0-255)   ";
        std::cin >> red;
        std::cout << "Green (0-255) ";
        std::cin >> green;
        std::cout << "Blue (0-255)  ";
        std::cin >> blue;
        std::cout << "Dim (0.0-1.0) ";
        std::cin >> dim;

        controller.setColor(red, green, blue);
        controller.setDim(dim);
    }

    return 0;
}
