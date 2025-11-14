# Wireless Display for Midnite Classic - No wiring needed!
This project, wirelessly connects to the Midnite Classic and displays "State of Charge", "Wbjr" Amps, & PV Watts. This was my first time coding, but it works!

The nice thing is, all you need to do to the Classic is to connect your Classic to the internet with a Ethernet cable and set up a static IP address for it. If you are already using the "Classic Monitor" app or something similar, you probably allready did this.

![Image of Project](ProjectIMG.jpg)

# How it works: 
If your Classic is connected to the internet via an Ethernet cable and you have already assigned it a static IP. This setup will: 
- 1 Connect to your wifi
- 2 then connect to the Classic over wifi/Ethernet 
- 3 then read the Modbus data from the Classic 
- 4 then display the data on the OLED screen and serial output.
- 5 Once the data is being displayed on the OLED, the bottom two lines will flash every 5 seconds to let you know that it isn't frozen. FYI, if it looses connection, it will stop flashing then after it reconnects by itself it will start updating and flashing again.

# How to set it up:
I used Arduino IDE to create the code and program the Wemos D1 board.
- Edit Line "30"  as seen below:

WiFi.begin("USERNAME", "PASSWORD"); // INCLUDE USER AND PASSWORD HERE*****
- Edit Line "17" to the Static IP for your Classic as seen below:

IPAddress remote(192,168,0,96); // Classic IP ***** CHANGE THIS TO YOURS****

- Download the libraries needed as seen at the top of the code. Obviously though, you may need different libraries if you are using different boards.



Feel free to email me at kg7zev@gmail.com with any questions. I plan on replacing the screen with a bigger 2.8" 240x320 OLED display. 


Also, while this board is connected to the Classic, you can not use any other software to connect to it such as the "Classic Monitor" app. However, the Midnite Solar "MyMidnite" website is unaffected. This doesn't bother me since I only use the app when setting up pannels or troubleshooting something and I really like having a remote display that I can mount anywhere!
