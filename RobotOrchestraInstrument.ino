#include <Servo.h>
Servo note;  // servo used for playing note

// you will need to experiment with these numbers to find the right servo position for pressing/releasing a note
int note_press = 70;     // servo position for "pressing" note
int note_release = 110;  // servo position for "releasing" note

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  note.attach(2); // attach the Servo to the digital pin the servo is connected to
  while (!Serial) {
    ;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {     // if serial data is available
    byte note = Serial.read();      // read one byte from buffer, this is the note byte
    byte vel = Serial.read();       // read another byte from buffer, this is the velocity byte
    switch (note) {                 // jump to the procedure for the note received
      case 60:                      // 60 is the midi value for C4 (middle C on a piano)
        if (vel > 0) {              // if velocity is greater than 0, play this note
          note.write(note_press);
          delay(250);               // wait a moment before releasing the note
          note.write(note_release); // release the note
        }
        else                        // if velocity is not greater than 0, release the note
          note.write(note_release);
        break;
        
      default:  // if there is not a case for the note received, do nothing
        break;
    }
  }
}
