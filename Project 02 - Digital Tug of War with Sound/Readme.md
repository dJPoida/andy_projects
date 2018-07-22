# Digital Tug of War (With Sound)

This is the same as Project 1 - Digital Tug of War but with the addition of sound!

## How does the game work?

* The Arduino starts up, plays a tune and begins flashing the center LED indicating the game is about to start.
* Once the center LED turns green, each player can begin tugging from their end by rapidly pressing each button in sequence (left / right / left etc...)
* After one player exceeds the other player by a pre-determined threshold (10 by default) the game is won by that player and a winning tune is played!
* To play again, reset the arduino using the reset button on the board.

## How is it built?
The schematic can be found in the _/Circuit Diagrams_ folder.

## Notes
* There are 7 resistors used in this project.
    * The two resistors on the LEDs are 220ohm
    * The four resistors on the buttons are 10kohm
    * The resistor on the Buzzer is 220ohm
* The RGB LED in this project is assumed to have onboard current limiting resistors.
