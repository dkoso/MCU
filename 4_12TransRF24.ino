
#include <SPI.h>
#include "RF24.h"

RF24 radio(9, 10);

const byte address[6] = "00001";

char buf[256];
int availableBytes = 0;
int len = 0;
bool ptt = false;

int count = 0;
int i = 0;

struct dataStruct {
  char text[25];
  int packets = 0;
  bool EOT = false;
} myData;

void setup() {
  Serial.begin(57600);
  pinMode(4, OUTPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(8);
  radio.setCRCLength(RF24_CRC_8);
  radio.stopListening();
  Serial.println("Sending RF24...");
}

void(* resetFunc) (void) = 0; // Software reset for arduino 

void loop() {

  get();
  
  delay(250);
  
  send();
}

void clearArr() {

  memset(&myData.text[0], 0, sizeof(myData.text));
  Serial.println("clearArr() called");
  
}

void get() {

  if (Serial.available() > 0)
  {
    availableBytes = Serial.available();
    if (availableBytes <= 173)
    {
      for (int i = 0; i < availableBytes; i++)
      {
        buf[i] = Serial.read();
        buf[i + 1] = '\0';
        len++;
        Serial.print(buf[i]);
      }
      Serial.println("");
      Serial.print("Serial chars: ");
      Serial.println(availableBytes);
      ptt = true;
    }
    else
    {
      Serial.println("The messaage is limited to 173 characters");
      delay(1000);
      resetFunc();
    }
  }
}


void send() {
	
	while (ptt == true)	
	{
		if (len > 25)
		{
			delay(175);
			for (i = 0; i < 25; i++)
			{
				myData.text[i] = buf[i + (count * 25)];
			}
			myData.packets++;
			myData.EOT = false;
			radio.write(&myData, sizeof(myData));
			Serial.println("> 25");
			Serial.print("Length: ");
			Serial.println(len);
			Serial.print("Count: ");
			Serial.println(count);
			Serial.print("Packets: ");
			Serial.println(myData.packets);
			Serial.println("----------");
			len -= 25;
			count++;
			ptt = true;
		}
		else
		{
			for (i = 0; i < 25; i++)
			{
				myData.text[i] = buf[i + (count * 25)];
			}
			myData.packets++;
			myData.EOT = true;
			radio.write(&myData, sizeof(myData));
			Serial.println("< 25");
			Serial.print("Length: ");
			Serial.println(len);
			Serial.print("Count: ");
			Serial.println(count);
			Serial.print("Packets: ");
			Serial.println(myData.packets);
			Serial.println("----------");
			len = 0;
			count = 0;
			myData.packets = 0;
			ptt = false;
		}
	}
}

