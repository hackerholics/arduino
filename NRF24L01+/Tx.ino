#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define trigPin 6
#define echoPin 7

RF24 radio(9, 10);
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

struct sensor
{
  int id = 0;
  long distance = 0;
  char text[50] = "Recieved";
};

typedef struct sensor Sensor;
Sensor data;

void setup(void)
{
  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/pingpair/\n\r");
  printf("ROLE: %s\n\r", "S");

  radio.begin();
  radio.setRetries(15, 15);
  radio.setPayloadSize(32);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
  radio.printDetails();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop(void)
{

  // First, stop listening so we can talk.
  radio.stopListening();
  radio.write(&data, sizeof(data));
  Serial.print("\nPackage:");
  Serial.print(data.id);
  Serial.print("\n");
  Serial.println(data.distance);
  //  Serial.println(data.text);
  data.id = data.id + 1;
  // data.distance = data.distance + 0.1;
  // delay(lat);


  long duration;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  data.distance = (duration / 2) / 29.1;


}
