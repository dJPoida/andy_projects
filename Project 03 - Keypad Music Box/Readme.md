# Keypad Music Box

This little music box should be a bit of fun to play with.

It utilised the 4x4 matrix button keypad you got in your kit and also uses a light sensor for pitch bending!

## How does it work?

* Once the Arduino is powered up, you can press the buttons on the keypad to play a song.
* Hover your finger over the LDR (Light Detecting Resistor or "Light Sensor") to bend the pitch of the current note.

There are 2 octaves in the key of C Major

| C | D | E | F |
| G | A | B | C |
| C | D | E | F |
| G | A | B | C |

## How is it built?
The schematic can be found in the _/Circuit Diagrams_ folder.

## Notes
* There are 2 resistors used in this project.
    * The resistor used with the piezo buzzer is 220ohms
    * The resistor used on the LDR is 10kohm
* The cables to connect the keypad with the Arduino came in the kit as a single rainbow coloured ribbon. You only need to connect 8 of the available 10 cables to make the project work.
