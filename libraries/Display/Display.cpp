#ifndef DISPLAY_CPP
#define DISPLAY_CPP

#include "Display.h"


bool initDisplayB = true;
bool displayUpdateB = false;


// ==================================================================
/*
  Callback di inizalizzazione del display.
  setup() di arduino
*/
// ==================================================================
static displayClass displayData;
static Adafruit_SSD1306 display(displayData.width, displayData.height, &Wire, displayData.pinResetOled);

static timer showLogoTimer(0);


static textClass firstRow;
static textClass secondRow;
static textClass thirdRow;
static textClass fourthRow;

Display VEADisplay;


void  Display::initDisplay()
{
    Wire.begin();


    if (!display.begin(SSD1306_SWITCHCAPVCC, displayData.addressOled, false))
    {

        // Serial.println(F("SSD1306 allocation failed"));
    }else{
        display.setTextColor(SSD1306_WHITE); // Draw white text
        /*!
        \brief display the VEA's logo
        */
        display.drawBitmap(0, 0, logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
        display.display();
        showLogoTimer.reset();
    }
    
    
}

/*!
   \brief write text to the screen, do not update the screen
   \param alignment alignment of the text
   \param row number of the row (screen divided by 4)
   \param fontWidth the width of the font (to calculate the cursor position)
   \param fontHeight the height of the font (to calculate the background of the text position)
   \param text the text to write
   \param inverted if the text is inverted the text will be black with a white
          background, otherwise it will be white with black background
   \param textSize the size of the text (min 1)
   \param column the start position of cursor if alignment is "Manual"
    \param setFont if true set the font to FreeMono9pt7b, otherwise use the current font
   \todo for the text size > 1 the backgound will be bigger, now is small
*/
void Display::writeOnDisplay(textAlign alignment, uint8_t row, uint8_t fontWidth,
                    uint8_t fontHeight, String text,
                    bool inverted = false, uint8_t textSize = 1,
                    uint8_t column = 0,bool setFont = true, int xCorrection = 0, int yCorrection = 0)
{
    display.setTextSize(textSize); // Normal 1:1 pixel scale
    if (setFont){
        display.setFont(&FreeMono9pt7b);
    }
    // calculate offsets for text
    int x = 0;
    switch (alignment)
    {
    case Center:
        x = (displayData.width - text.length() * fontWidth) / 2.;
        x += displayData.offsetWidth+xCorrection;
        break;
    case Right:
        x = (displayData.width - text.length() * fontWidth);
        x += displayData.offsetWidth+xCorrection;
        break;
    case Left:
        x = 0;
        break;
    default:
        x = column;
        break;
    }

    int y = displayData.height / 4. * row - floor((displayData.height / 4. - fontHeight) / 2.)+yCorrection;
    //debug print
    // if (row == 4){
    //     // Serial.print("x: ");
    //     // Serial.print(x);
    //     // Serial.print(" y: ");
    //     Serial.println(y);

    // }

    
    // calculate offsets for rectangles
    int baseRecty = displayData.height / 4. * (row - 1) + 1;
    int width = text.length() * fontWidth;
    // draw rectangles
    if (inverted)
    {
        display.fillRect(x, baseRecty, width, displayData.height / 4. - 1, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
    }
    else
    {
        display.fillRect(x, baseRecty, width, displayData.height / 4. - 1, SSD1306_BLACK);
        display.setTextColor(SSD1306_WHITE);
    }

    // draw text
    display.setCursor(x, y);
    display.write(text.c_str()); // Write
}

/*!
   \brief display one input on the screen
*/
void Display::displayInput(uint8_t pin, bool value)
{
    writeOnDisplay(Manual, 1, displayData.fontWidth, displayData.fontHeight, String(pin),
                   value, 1, 35 + displayData.fontWidth * pin);
}

/*!
   \brief display one output on the screen
*/
void Display::displayOutput(uint8_t pin, bool value)
{
    writeOnDisplay(Manual, 4, displayData.fontWidth, displayData.fontHeight, String(pin),
                   value, 1, 35 + displayData.fontWidth * pin);
}



void Display::manageDisplay()
{
    // reset display if isn't in first SHOW_LOGO_TEMP mSec
    if (showLogoTimer.readMilliseconds() > displayData.showLogoTemp)
    {
        display.setFont(&FreeSans8pt7b);
        // Serial.println("clearing");
        display.clearDisplay();

        if (firstRow.text != "")
        {
            writeOnDisplay(Center, 1, displayData.fontWidth, displayData.fontHeight, firstRow.text.c_str(), firstRow.inverted,firstRow.textSize,firstRow.column,true,firstRow.xCorrection,firstRow.yCorrection);
        }else{
            writeOnDisplay(Left, 1, displayData.fontWidth, displayData.fontHeight, "IN");
            for (size_t i = 0; i < number_of_input_pins; i++)
            {
                displayInput(i, digitalRead(digital_in_pin[i]));
            }
        }
        if (fourthRow.text != ""){
            writeOnDisplay(Center, 4, displayData.fontWidth, displayData.fontHeight, fourthRow.text.c_str(), fourthRow.inverted,fourthRow.textSize,fourthRow.column,true,fourthRow.xCorrection,fourthRow.yCorrection);
        }else{
            writeOnDisplay(Left, 4, displayData.fontWidth, displayData.fontHeight, "OUT");
            for (size_t i = 0; i < number_of_output_pins; i++)
            {
                displayOutput(i, digitalRead(digital_out_pin[i]));
            }
        }


        if (secondRow.text != "")
        {
            writeOnDisplay(Center, 2, displayData.fontWidth, displayData.fontHeight, secondRow.text.c_str(), secondRow.inverted,secondRow.textSize,secondRow.column,true,secondRow.xCorrection,secondRow.yCorrection);
        }else{
            writeOnDisplay(Center, 2, displayData.fontWidth, displayData.fontHeight, "IRUINO", false, 1, 0, false, 5, 0);
        }
        if (thirdRow.text != "")
        {
            writeOnDisplay(Center, 3, displayData.fontWidth, displayData.fontHeight, thirdRow.text.c_str(), thirdRow.inverted,thirdRow.textSize,thirdRow.column,true,thirdRow.xCorrection,thirdRow.yCorrection);
        }else{
            float analog0 = analogRead(A0) * 10.0 / 4095.0;
            float analog1 = analogRead(A1) * 10.0 / 4095.0;
            char analogString0[6];  // Allocate a buffer for the string
            char analogString1[6];  // Allocate a buffer for the string
            sprintf(analogString0, "A%02d.%01d", (int)(analog0), (int)(analog0 * 10) % 10);
            sprintf(analogString1, "B%02d.%01d", (int)(analog1), (int)(analog1 * 10) % 10);  // Format the string with the format "XX.Y"
            writeOnDisplay(Left, 3, displayData.fontWidth, displayData.fontHeight, analogString0, false, 1, 0, false, 0, 0);
            writeOnDisplay(Right, 3, displayData.fontWidth, displayData.fontHeight, analogString1, false, 1, 0, false, 7, 0);
        }
        
        //update screen
        display.display();
    }

}

static bool displayInitialized = false;

void Display::DisplayLoop(){
    if (!displayInitialized){
        initDisplay();
        displayInitialized = true;
    }
    manageDisplay();
}
void  Display::clearRow(uint8_t row)
{
    display.fillRect(0, displayData.height / 4. * (row - 1) + 1, displayData.width, displayData.height / 4. - 1, SSD1306_BLACK);
}

void Display::setFirstRow(std::string text, textAlign alignment, bool inverted, uint8_t textSize, uint8_t column, int xCorrection, int yCorrection){
    firstRow.text = text;
    firstRow.alignment = alignment;
    firstRow.inverted = inverted;
    firstRow.textSize = textSize;
    firstRow.column = column;
    firstRow.xCorrection = xCorrection;
    firstRow.yCorrection = yCorrection;
}
void Display::setSecondRow(std::string text, textAlign alignment, bool inverted, uint8_t textSize, uint8_t column, int xCorrection, int yCorrection){
    secondRow.text = text;
    secondRow.alignment = alignment;
    secondRow.inverted = inverted;
    secondRow.textSize = textSize;
    secondRow.column = column;
    secondRow.xCorrection = xCorrection;
    secondRow.yCorrection = yCorrection;
}
void Display::setThirdRow(std::string text, textAlign alignment, bool inverted, uint8_t textSize, uint8_t column, int xCorrection, int yCorrection){
    thirdRow.text = text;
    thirdRow.alignment = alignment;
    thirdRow.inverted = inverted;
    thirdRow.textSize = textSize;
    thirdRow.column = column;
    thirdRow.xCorrection = xCorrection;
    thirdRow.yCorrection = yCorrection;
}
void Display::setFourthRow(std::string text, textAlign alignment, bool inverted, uint8_t textSize, uint8_t column, int xCorrection, int yCorrection){
    fourthRow.text = text;
    fourthRow.alignment = alignment;
    fourthRow.inverted = inverted;
    fourthRow.textSize = textSize;
    fourthRow.column = column;
    fourthRow.xCorrection = xCorrection;
    fourthRow.yCorrection = yCorrection;
}
void Display::setFirstRow(String text, textAlign alignment, bool inverted, uint8_t textSize, uint8_t column, int xCorrection, int yCorrection){
    setFirstRow(std::string(text.c_str()),alignment,inverted,textSize,column,xCorrection,yCorrection);
}
void Display::setSecondRow(String text, textAlign alignment, bool inverted, uint8_t textSize, uint8_t column, int xCorrection, int yCorrection){
    setSecondRow(std::string(text.c_str()),alignment,inverted,textSize,column,xCorrection,yCorrection);
}
void Display::setThirdRow(String text, textAlign alignment, bool inverted, uint8_t textSize, uint8_t column, int xCorrection, int yCorrection){
    setThirdRow(std::string(text.c_str()),alignment,inverted,textSize,column,xCorrection,yCorrection);
}
void Display::setFourthRow(String text, textAlign alignment, bool inverted, uint8_t textSize, uint8_t column, int xCorrection, int yCorrection){
    setFourthRow(std::string(text.c_str()),alignment,inverted,textSize,column,xCorrection,yCorrection);
}

#endif
