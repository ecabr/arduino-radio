#include <SPI.h>
#include <Wire.h>
#include <Encoder.h>

unsigned int frequency = 0;
unsigned int step = 1;
Encoder encoder(2, 3);
int oldPosition = -999;

void setup() {
  Serial.begin(9600);

  // Init radio module
  Wire.begin();
  frequency = 875;
  delay(100);
  setFrequency(frequency);

  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  int newPosition = encoder.read();
  if (oldPosition != newPosition) {
    int delta = (newPosition - oldPosition) / 2;
    if (delta != 0) {
      if (delta > 0) {
      frequency += step;
      }
      else {
        frequency -= step;
      }
      setFrequency(frequency);
      oldPosition = newPosition;
    }
  }
}

void setFrequency(unsigned int frequency) {
  // Data to write
  unsigned int frequencyB = freqToWord(frequency);
  unsigned char frequencyH = highFreqWord(frequencyB);
  unsigned char frequencyL = lowFreqWord(frequencyB);

  // Write data
  Wire.beginTransmission(0x60);
  Wire.write(frequencyH);
  Wire.write(frequencyL);
  Wire.write(0xB0);
  Wire.write(0x10);
  Wire.write(0x00);
  Wire.endTransmission();

  Serial.println(frequency);
}

unsigned int freqToWord(unsigned int frequency) {
  unsigned int freqB = 4*(frequency*100000+225000)/32768;
  return freqB;
}

unsigned char highFreqWord(unsigned int freqB) {
  unsigned char freqH = freqB >> 8;
  return freqH;
}

unsigned char lowFreqWord(unsigned int freqB) {
  unsigned char freqL = freqB & 0xff;
  return freqL;
}
