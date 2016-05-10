#include <Servo.h>

/************** Global Variables **************/
enum Modes {
  ALLON,
  ALLOFF,
  PLAY
};

enum PlayableNotes {
  A,  // Midi #69
  G,  // Midi #67
  F,  // Midi #65
  E,  // Midi #64
  NUMNOTES  // add playable notes above this line
};

Servo notes[NUMNOTES];          // array of servos used for playing notes
byte note_pin[NUMNOTES];        // pins servos are connected to on Arduino
byte note_position[NUMNOTES];   // note state positions
byte on_position[NUMNOTES];     // on state positions
byte off_position[NUMNOTES];    // off state positions
byte mode;                      // Intrument mode (On, Off, Play)
byte note_index;                // Note index in array
byte byte_count;                // Number of incoming bytes
byte midi_note;                 // Midi note number
byte midi_vel;                  // Midi velocity number (greater than 0 is on, else off)

/************** Configuration Parameters **************/
void initialize() {
  mode = PLAY;                  // Set the mode of the Arduino (All notes on, All notes off, Play incoming notes)

  on_position[A] = 80;          // Note on position
  on_position[G] = 80;
  on_position[F] = 80;
  on_position[E] = 80;

  off_position[A] = 100;        // Note off position
  off_position[G] = 100;
  off_position[F] = 100;
  off_position[E] = 100;

  note_pin[A] = 2;              // pin the servo is connected to on Arduino
  note_pin[G] = 3;
  note_pin[F] = 4;
  note_pin[E] = 5;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initialize();                           // Initialize configuration parameters
  for (int note = 0; note < NUMNOTES; note++)
    notes[note].attach(note_pin[note]);   // attach the Servo to the digital pin the servo is connected to
  while (!Serial) {
    ;
  }
}

void loop() {
  switch (mode) {
    case ALLON:
      allNotesOn();     // moves all servos to on position
      break;

    case ALLOFF:
      allNotesOff();    // moves all servos to off position
      break;

    case PLAY:
      play();           // plays incoming notes
      break;

    default:
      break;
  }
}

// play() is used to play notes it receives from the main computer
void play() {
  pressNotes();
  delay(20);        // change this as needed
  allNotesOff();    // if your instruement can sustain notes, remove this and previous line
}

// allNotesOn() is used to move all servos to the note on position
void allNotesOn() {
  for (int note = 0; note < NUMNOTES; note++)
    note_position[note] = on_position[note];  // set note position to on
  playNotes();
}

// allNotesOff() is used to move all servos to the note off position
void allNotesOff() {
  for (int note = 0; note < NUMNOTES; note++)
    note_position[note] = off_position[note];// set note position to off
  playNotes();
}

// playNotes() is used to move the servos to the note position
void playNotes() {
  for (int note = 0; note < NUMNOTES; note++)
    notes[note].write(note_position[note]);
}

// pressNotes() is used to set + move the servos specified by the serial messages
void pressNotes() {
  byte_count = Serial.available();              // check if message available
  if (byte_count > 0) {
    for (int b = 0; b < byte_count / 2; b++) {  // read note
      midi_note = Serial.read();
      midi_vel = Serial.read();
      setNotePosition(midi_note, midi_vel);     // set note position
    }
    playNotes();                                // play note  
  }
}

// setNotePosition() is used to set note position specified by input parameters
void setNotePosition(byte note, byte vel) {
  note_index = getNoteIndex(note);
  if (note_index > 0) {
    if (vel > 0)
      note_position[note_index] = on_position[note_index];
    else
      note_position[note_index] = off_position[note_index];
  }
}

// getNoteIndex() returns the note array index for a given midi note
byte getNoteIndex(byte note) {
  switch (note) {
    case 69:
      return A;
    case 67:
      return G;
    case 65:
      return F;
    case 64:
      return E;
    default:
      return -1;
  }
}
