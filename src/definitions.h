/*
  N32B Macros Firmware v4.x.x
  MIT License

  Copyright (c) 2024 SHIK
*/

#ifndef N32B_DEFINITIONS
#define N32B_DEFINITIONS

#include <Arduino.h>
#include <vector>
#include <USB-MIDI.h>
#include <ezButton.h>
#include <Pot.h>
#include <display.h>

USING_NAMESPACE_MIDI;

#ifndef N32Bv3
constexpr uint8_t threshold_idle_to_motion = 4;
#else
constexpr uint8_t threshold_idle_to_motion = 2;
#endif
constexpr uint8_t threshold_motion_to_idle = 16;

const uint8_t firmwareVersion[] PROGMEM = {4, 5, 2};

extern MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport> MIDICoreUSB;
extern MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>> MIDICoreSerial;
extern N32B_DISPLAY n32b_display;
extern ezButton buttonA;
extern ezButton buttonB;

/* Pin setup */
#ifdef N32Bv3
enum PinIndices : uint8_t
{
  MUX_A_SIG = 8,
  MUX_B_SIG = 9,
  MIDI_TX_PIN = 1,
  MUX_S0 = 4,
  MUX_S1 = 5,
  MUX_S2 = 6,
  MUX_S3 = 7,
  LED_PIN = 17,
  SIN = 16,
  LAT = 10,
  SCLK = 15,
  BLANK = 14,
  BUTTON_A_PIN = A3,
  BUTTON_B_PIN = A2
};
#else
enum PinIndices : uint8_t
{
  MUX_A_SIG = 8,
  MUX_B_SIG = 9,
  MIDI_TX_PIN = 1,
  MUX_S0 = 2,
  MUX_S1 = 3,
  MUX_S2 = 4,
  MUX_S3 = 5,
  LED_PIN = 17,
  DIN = 16,
  CS = 10,
  CLK = 15,
  BUTTON_A_PIN = A3,
  BUTTON_B_PIN = A2
};
#endif

enum COMMANDS_INDEXS : uint8_t
{
  MANUFACTURER_INDEX = 1,
  COMMAND_INDEX,
  KNOB_INDEX, // Also used for other commands value
  MSB_INDEX,
  LSB_INDEX,
  CHANNEL_A_INDEX,
  CHANNEL_B_INDEX,
  OUTPUT_A_INDEX,
  OUTPUT_B_INDEX,
  MIN_A_INDEX,
  MAX_A_INDEX,
  MIN_B_INDEX,
  MAX_B_INDEX,
  PROPERTIES_INDEX,
  KNOB_MODE_INDEX
};

enum COMMANDS : uint8_t
{
  SET_KNOB_MODE = 1,         // Define knob mode (see KNOB_MODES)
  SAVE_PRESET = 2,           // Save the preset
  LOAD_PRESET = 3,           // Load a preset
  SEND_FIRMWARE_VERSION = 4, // Send the device firmware version
  SYNC_KNOBS = 5,            // Send active preset
  CHANGE_CHANNEL = 6,        // Changes the global MIDI channel
  START_SYSEX_MESSAGE = 7,   // Announce start of sysEx mesasge
  SET_THRU_MODE = 8,         // Set the midi THRU behavior
  END_OF_TRANSMISSION = 99   // Notify end of transmission
};

enum KNOB_MODES : uint8_t
{
  KNOB_MODE_DISABLE = 0,
  KNOB_MODE_STANDARD = 1,
  KNOB_MODE_MACRO = 2,
  KNOB_MODE_NRPN = 3,
  KNOB_MODE_RPN = 4,
  KNOB_MODE_HIRES = 5,
  KNOB_MODE_PROGRAM_CHANGE = 6,
  KNOB_MODE_MONO_AFTER_TOUCH = 7,
  KNOB_MODE_POLY_AFTER_TOUCH = 8
};

// General definitions
enum DEFINITIONS : uint8_t
{
  SHIK_MANUFACTURER_ID = 32,
  NUMBER_OF_KNOBS = 32,
  NUMBER_OF_PRESETS = 3
};

enum PROPERTIES : uint8_t
{
  INVERT_A_PROPERTY = 0,
  INVERT_B_PROPERTY = 1,
  USE_OWN_CHANNEL_A_PROPERTY = 2,
  USE_OWN_CHANNEL_B_PROPERTY = 3
};

enum CHANNEL_NAMES : uint8_t
{
  CHANNEL_A = 1,
  CHANNEL_B = 0
};

enum THRU_MODES : uint8_t
{
  THRU_OFF = 0,
  THRU_TRS_TRS = 1,
  THRU_TRS_USB = 2,
  THRU_USB_USB = 3,
  THRU_USB_TRS = 4,
  THRU_BOTH_DIRECTIONS = 5
};

enum OUTPUT_MODES : uint8_t
{
  OUTPUT_OFF = 0,
  OUTPUT_TRS,
  OUTPUT_USB,
  OUTPUT_BOTH
};

// Knob settings structure

struct Knob_t
{
  uint8_t MSB;
  uint8_t LSB;
  uint8_t MIN_A;
  uint8_t MIN_B;
  uint8_t MAX_A;
  uint8_t MAX_B;
  uint8_t OUTPUTS;  // MSB 2-bit for Macro A, LSB 2-bit for Macro B
  uint8_t CHANNELS; // MSB 4-bit for Channel A, LSB 4-bit for Channel B
  uint8_t PROPERTIES;
  // Using PROPERTIES to reduce storage size.
  // Bits are used as boolean values for inverts and use own channel:
  // 1 - Invert A
  // 2 - Invert B
  // 3 - Use own channel A
  // 4 - Use own channel B

  // Knob mode is defined with 4 bits and need to be shifted to the right to calculate it's value:
  // 5-8 - Mode value
};

// A preset struct is defining the device preset structure
struct Preset_t
{
  Knob_t knobInfo[32];
  uint8_t thruMode;
};

// A device struct is defining the device structure
struct Device_t
{
  Preset_t activePreset{0};
  Pot pots[NUMBER_OF_KNOBS];
  midi::Channel globalChannel{1};
  byte currentPresetIndex{0};
  bool isPresetMode{false};
};

/* Device setup data */
extern Device_t device;

/* Buttons variables */
extern const unsigned int reset_timeout; // Reset to factory preset timeout
extern const uint8_t SHORT_PRESS_TIME;   // Milliseconds
extern unsigned long pressedTime;
extern bool isPressingAButton;
extern bool isPressingBButton;

// byte index in EEPROM for the last used preset
extern uint8_t lastUsedPresetAddress;

#endif
