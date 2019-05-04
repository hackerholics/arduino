// importing libs
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

int LED = 2;

RF24 radio(9, 10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

struct sensor
{
  int id = 0;
  long distance = 0;
  // char text[50] = "empty";
};

typedef struct sensor Sensor;
Sensor data;

void setup(void)
{


  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/pingpair/\n\r");
  printf("ROLE: %s\n\r", "R");

  radio.begin();
  radio.setRetries(15, 15);
  radio.setPayloadSize(32);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();
  radio.printDetails();

  pinMode(LED, OUTPUT);
}

void loop(void)
{

  if (radio.available())
  {
    radio.read(&data, sizeof(data));
    Serial.print("\nPackage:");
    Serial.print(data.id);
    Serial.print("\n");
    Serial.println(data.distance);
    //  Serial.println(data.text);

    if (data.distance < 10) {
      digitalWrite(LED, LOW);
      Serial.println("Warning");
    }
    else {
      digitalWrite(LED, HIGH);
      Serial.println("Safe");
    }
  }
}
