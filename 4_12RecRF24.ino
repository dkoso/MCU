

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#include <SPI.h>
#include "RF24.h"


RF24 radio(9, 10);

const byte address[6] = "00001";

struct dataStruct {
  char text[25];
  int packets = 0;
  bool EOT = false;
} myData;

int counter = 0;

char buf[183];
char oledText1[26];
char oledText2[26];
char oledText3[26];
char oledText4[26];
char oledText5[26];
char oledText6[26];
char oledText7[26];

void rx();
void textArr();
void disp();

void setup(void) {
	
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  
  u8g2.begin();
  u8g2.setFlipMode(0);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(8);
  radio.setCRCLength(RF24_CRC_8);
  radio.startListening();
  
  Serial.println("Receiving...");
  
}

void loop(void) {
	
	rx();
	
	textArr();
	
	disp();
	
}


void rx(void) {
	
	
	if (radio.available()) {

    radio.read(&myData, sizeof(myData));
	
	
    for (int i = 0; i < sizeof(myData.text); i++)
	{
		buf[i] = myData.text[i];
		digitalWrite(4, HIGH);
	}
	
	Serial.print(buf);
    counter++;
	

    if (myData.EOT == true)
    {
      if (!myData.packets == counter)
      {
        Serial.println(" FAIL! Packets Missing!");
      }
      Serial.println("");
      counter = 0;
	  digitalWrite(4, LOW);
	  myData.EOT = false;
    }
	
  }
	
 }


 void textArr(void) {
		
			if (myData.packets == 1)
			{
				for (int i = 0; i < 25; i++)
				{
					oledText1[i] = buf[i];
				}
				memset(&oledText2[0], 0, sizeof(oledText1));
				memset(&oledText3[0], 0, sizeof(oledText1));
				memset(&oledText4[0], 0, sizeof(oledText1));
				memset(&oledText5[0], 0, sizeof(oledText1));
				memset(&oledText6[0], 0, sizeof(oledText1));
				memset(&oledText7[0], 0, sizeof(oledText1));
			}
			else if (myData.packets == 2)
			{
				for (int i = 0; i < 25; i++)
				{
					oledText2[i] = buf[i];
				}
				memset(&oledText3[0], 0, sizeof(oledText1));
				memset(&oledText4[0], 0, sizeof(oledText1));
				memset(&oledText5[0], 0, sizeof(oledText1));
				memset(&oledText6[0], 0, sizeof(oledText1));
				memset(&oledText7[0], 0, sizeof(oledText1));
			}
			else if (myData.packets == 3)
			{
				for (int i = 0; i < 25; i++)
				{
					oledText3[i] = buf[i];
				}
				memset(&oledText4[0], 0, sizeof(oledText1));
				memset(&oledText5[0], 0, sizeof(oledText1));
				memset(&oledText6[0], 0, sizeof(oledText1));
				memset(&oledText7[0], 0, sizeof(oledText1));
			}
			else if (myData.packets == 4)
			{
				for (int i = 0; i < 25; i++)
				{
					oledText4[i] = buf[i];
				}
				memset(&oledText5[0], 0, sizeof(oledText1));
				memset(&oledText6[0], 0, sizeof(oledText1));
				memset(&oledText7[0], 0, sizeof(oledText1));
			}
			else if (myData.packets == 5)
			{
				for (int i = 0; i < 25; i++)
				{
					oledText5[i] = buf[i];
				}
				memset(&oledText6[0], 0, sizeof(oledText1));
				memset(&oledText7[0], 0, sizeof(oledText1));
			}
			else if (myData.packets == 6)
			{
				for (int i = 0; i < 25; i++)
				{
					oledText6[i] = buf[i];
				}
				memset(&oledText7[0], 0, sizeof(oledText1));
			}
			else if (myData.packets == 7)
			{
				for (int i = 0; i < 25; i++)
				{
					oledText7[i] = buf[i];
				}
			}
					
 }	
 

 
void disp(void) {
	
  u8g2.setFont(u8g2_font_5x8_mf);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 9);
    u8g2.print(oledText1);
	u8g2.setCursor(0, 18);
    u8g2.print(oledText2);
	u8g2.setCursor(0, 27);
    u8g2.print(oledText3);
	u8g2.setCursor(0, 36);
    u8g2.print(oledText4);
	u8g2.setCursor(0, 45);
    u8g2.print(oledText5);
	u8g2.setCursor(0, 54);
    u8g2.print(oledText6);
	u8g2.setCursor(0, 63);
    u8g2.print(oledText7);
  } while ( u8g2.nextPage() );
  //delay(1000);	
}
	
