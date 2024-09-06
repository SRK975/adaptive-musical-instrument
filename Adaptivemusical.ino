#include <MD_MAX72xx.h>
#include <SPI.h>

#include <Wire.h>
#include <Adafruit_MPR121.h>


#include <SoftwareSerial.h>
#include <MIDI.h>

// MPR121 setup
Adafruit_MPR121 cap = Adafruit_MPR121();

// MAX7219 setup
#define MAX_DEVICES 1
#define DATA_PIN 11
#define CS_PIN 10
#define CLK_PIN 13
MD_MAX72XX mx = MD_MAX72XX(CS_PIN, MAX_DEVICES);

// MP3-TF module setup
SoftwareSerial mp3Serial(2, 3); // RX, TX for MP3 module

// MIDI setup
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  Serial.begin(115200);
  mp3Serial.begin(9600);

  // Initialize MPR121
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring!");
    while (1);
  }

  // Initialize MAX7219
  mx.begin();
  mx.clear();

  // Initialize MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // Test MP3 module
  mp3Serial.write(0x7E); // Send play command, as an example
}

void loop() {
  // Check touch input
  uint16_t touched = cap.touched();
  for (uint8_t i = 0; i < 12; i++) {
    if (touched & (1 << i)) {
      mx.clear();
      mx.setPoint(0, i, true); // Visual feedback
      playSound(i); // Play sound corresponding to touch pad
      sendMIDI(i);  // Send MIDI note
    }
  }
}

// Function to play sound based on touch input
void playSound(uint8_t index) {
  // Simple command to play track, depends on MP3-TF module command set
  mp3Serial.write(0x7E); // Replace with specific play commands
}

// Function to send MIDI note based on touch input
void sendMIDI(uint8_t note) {
  MIDI.sendNoteOn(note + 60, 127, 1); // Note On, velocity 127, channel 1
  delay(100);
  MIDI.sendNoteOff(note + 60, 127, 1); // Note Off
}