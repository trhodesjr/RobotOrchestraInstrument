#include <Servo.h>

/************** Global Variables **************/
enum Modes {
  ALLON,
  ALLOFF,
  PLAY
};

enum PlayableNotes {
  C,  // Midi #72
  D,  // Midi #74
  E,  // Midi #76
  G,  // Midi #79
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
  mode = PLAY;                 // Set the mode of the Arduino (All notes on, All notes off, Play incoming notes)

  on_position[C] = 95;         // Note on position
  on_position[D] = 95;
  on_position[E] = 85;
  on_position[G] = 90;

  off_position[C] = 90;        // Note off position
  off_position[D] = 88;
  off_position[E] = 93;
  off_position[G] = 95;

  note_pin[C] = 11;            // Pin the servo is connected to on Arduino
  note_pin[D] = 5;
  note_pin[E] = 6;
  note_pin[G] = 9;
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
  allNotesOff();
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
  delay(100);        // change this as needed
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
  if (byte_count % 2 == 0) {
    for (int b = 0; b < byte_count / 2; b++) { // read no
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
  if (note_index < 0) // invalid note index
    return;
  else if (vel > 0)
    note_position[note_index] = on_position[note_index];
  else
    note_position[note_index] = off_position[note_index];
}

void testConnection(byte note) {
  delay(750);
  Serial.write(note);  
}

// getNoteIndex() returns the note array index for a given midi note
byte getNoteIndex(byte note) {
  switch (note) {
    case 99:
      testConnection(note);// Used to test connection to conductor
      return -1;
    case 72:
      return C;
    case 74:
      return D;
    case 76:
      return E;
    case 79:
      return G;
    default:      // midi note not playable
      return -1;  
  }
}
