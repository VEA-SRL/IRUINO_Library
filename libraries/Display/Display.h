#ifndef DISPLAY_H
#define DISPLAY_H



#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//fonts
#include "Fonts/FreeMono9pt7b.h"
#include "Fonts/FreeMono8pt7b.h"
#include "Fonts/FreeSans8pt7b.h"


//vea lib
#include "logo.h"
#include "timers.h"
#include "ESP32Thread.h"



struct displayClass
{
public:
    uint8_t addressOled = 0x3C; // See datasheet for Address
    double width = 128; // OLED display width, in pixels
    double height = 64; // OLED display height, in pixels
    uint8_t pinResetOled = -1; // Reset pin # (or -1 if sharing Arduino reset pin)

    double offsetWidth = 0;  // offset for display
    double offsetHeight = 0; // offset for display

    int showLogoTemp = 3000; // mSec of showing logo

    std::string codProd = ""; // codice prodotto

    uint8_t fontWidthPt = 9; // font width in points
    uint8_t fontHeightPt = 7; // font height in points
    double fontWidth = ceil(fontWidthPt * 1.33); // font width in pixels
    double fontHeight = ceil(fontHeightPt * 1.33); // font height in pixels
};




enum textAlign
{
    Left,
    Right,
    Center,
    Manual
};

class textClass{
    public :
        textClass(std::string text, textAlign alignment, bool inverted, uint8_t textSize, uint8_t row, uint8_t column, int xCorrection, int yCorrection);
        textClass() {}
        ~textClass(){}
        std::string text = "";
        textAlign alignment = Left;
        bool inverted = false;
        uint8_t textSize= 1;
        uint8_t column = 1;
        int xCorrection= 0;
        int yCorrection= 0;
};

class Display{
    private:
        Thread displayThread;

        static void displayOutput(uint8_t pin, bool value);
        static void displayInput(uint8_t pin, bool value);
        static void writeOnDisplay(textAlign alignment, uint8_t row, uint8_t fontWidth,
                            uint8_t fontHeight, String text,
                            bool inverted, uint8_t textSize,
                            uint8_t column, bool setFont, int xCorrection, int yCorrection);
        static void initDisplay();
        static void DisplayLoop();
        static void manageDisplay();
        static void clearRow(uint8_t row);

    public:
        //constructor
        /*the core on which the thread will run is the other one of the one on which
            the main loop runs, the main loop run over the core 1, so the thread will
             run over the core 1, defined in main.cpp with priority 1 
        */
        Display():displayThread(DisplayLoop, true, 0, 2, "displayThread", 0)
                  {displayThread.run();}
        //destructor
        ~Display(){}

        //displayClass display;
        void setFirstRow(std::string text, textAlign alignment = Left, bool inverted = false, uint8_t textSize = 1, uint8_t column = 1, int xCorrection = 0, int yCorrection = 0);
        void setSecondRow(std::string text, textAlign alignment = Left, bool inverted = false, uint8_t textSize = 1, uint8_t column = 1, int xCorrection = 0, int yCorrection = 0);
        void setThirdRow(std::string text, textAlign alignment = Left, bool inverted = false, uint8_t textSize = 1, uint8_t column = 1, int xCorrection = 0, int yCorrection = 0);
        void setFourthRow(std::string text, textAlign alignment = Left, bool inverted = false, uint8_t textSize = 1, uint8_t column = 1, int xCorrection = 0, int yCorrection = 0);
        void setFirstRow(String text, textAlign alignment = Left, bool inverted = false, uint8_t textSize = 1, uint8_t column = 1, int xCorrection = 0, int yCorrection = 0);
        void setSecondRow(String text, textAlign alignment = Left, bool inverted = false, uint8_t textSize = 1, uint8_t column = 1, int xCorrection = 0, int yCorrection = 0);
        void setThirdRow(String text, textAlign alignment = Left, bool inverted = false, uint8_t textSize = 1, uint8_t column = 1, int xCorrection = 0, int yCorrection = 0);
        void setFourthRow(String text, textAlign alignment = Left, bool inverted = false, uint8_t textSize = 1, uint8_t column = 1, int xCorrection = 0, int yCorrection = 0);

        

};

extern Display VEADisplay;


#endif // DISPLAY_H