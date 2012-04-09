To communicate from the PC, you can use for example putty (http://www.putty.org/).

- In connection type, choose serial
- In serial line, write the port where you connected the board (in windows, search for device manager -> Ports, its named COMx, where x is a number)
- The speed is the baud rate, in our case 9600
- Go to the Serial options, and select --> Data bits: 1, Stop bits: 1, Parity: None, Flow control: None
- Go back to the session tab, in Saved Sessions, write something there and click Save.
- Now, just double-click on the saved session to start.