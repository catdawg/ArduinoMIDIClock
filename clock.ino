

#define PIN_LED_PLAYING 6
#define PIN_LED_TEMPO 7

#define PIN_PLAY_INPUT 2
#define PIN_CONTINUE_INPUT 3
#define PIN_TAP_INPUT 2
#define PIN_TEMPO_POT 1

#define BPM_POT_DIVISOR 11 // needs to be a divisor of 1023 so that each pot position equals an integer bpm value
#define LOWEST_BPM 50 // highest bpm will be equal to 50 + (1023 / BPM_POT_DIVISOR)

// Midi types taken from the MIDI.h library
enum MidiType: uint8_t
{
    Clock                 = 0xF8,
    Start                 = 0xFA,
    Stop                  = 0xFC,
};

unsigned long last_clock_time_micros;
unsigned int ticks;
unsigned int bpm; // from 50 to 250

bool started;
int start_button_state;

void setup()
{
  pinMode(PIN_LED_PLAYING, OUTPUT);
  pinMode(PIN_LED_TEMPO, OUTPUT);
  pinMode(PIN_PLAY_INPUT, INPUT_PULLUP);
  digitalWrite(PIN_LED_PLAYING, HIGH);
  digitalWrite(PIN_LED_TEMPO, HIGH);
  
  // MIDI baudrate
  Serial.begin(31250);
  
  Serial.write(MidiType::Stop);
  Serial.write(MidiType::Clock);
  last_clock_time_micros = micros();
  started = false;
  start_button_state = HIGH;
  ticks = 0;
}

void loop()
{
  bool new_tick = false;

  // Start button
  {
    int new_start_button_state = digitalRead(PIN_PLAY_INPUT);

    if (start_button_state == HIGH && new_start_button_state == LOW) {
      
      if (started) {
        Serial.write(MidiType::Stop);
        digitalWrite(PIN_LED_PLAYING, HIGH);
        started = false;
      } else {
        Serial.write(MidiType::Start);
        digitalWrite(PIN_LED_PLAYING, LOW);
        started = true;
        ticks = 0;
        new_tick = true;
      }
    }
    start_button_state = new_start_button_state;
  }

  // Get bpm from the pot
  {
    unsigned long pot_value = 0x3ff - analogRead(PIN_TEMPO_POT); // result is 10 bits (max 1023), also we reverse it here
    
    unsigned long bpm_diff = (pot_value * (1023 / BPM_POT_DIVISOR)) / 1023;
    bpm = LOWEST_BPM + bpm_diff;
  }

  // handle sending clocks
  {
    unsigned long midi_clocks_per_minute = bpm * 24; // per MIDI specification, we send 24 clocks per quarter note
    unsigned long micro_interval_between_midi_clocks = (60 * 1000000) / midi_clocks_per_minute; // 60 * 1000000 is equal to 60 seconds in microseconds

    long next_clock = last_clock_time_micros + micro_interval_between_midi_clocks;

    unsigned long current_time = micros();
    long time_diff = next_clock - current_time;

    if (time_diff < 0) {
      Serial.write(MidiType::Clock);
      last_clock_time_micros = current_time + time_diff; // accounts for the imperfection, so the next clock will be earlier if this one was later
      ++ticks;
      new_tick = true;
    }
  }

  // handle tempo light
  {
    if (new_tick) {
      if (ticks % 24 > 6)
      {
        digitalWrite(PIN_LED_TEMPO, HIGH);
      }
      else 
      {
        digitalWrite(PIN_LED_TEMPO, LOW);
      }
    }
  }
}
