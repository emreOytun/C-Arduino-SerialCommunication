# C-Arduino-Serial-Communication
In this project, user gets Arduino do some tasks provided in menu. 

The objective of this project is to learn how to make a serial communication with devices such as Arduino and computer(C).

# Technical Issue about Getting Output :

When Arduino sends output to computer, it is essential to support this communication by a trigger system in which Arduino first sends a character to computer before sending actual output when C program waits for this character to come before starting getting actual output. In that way, it's more secure to get output from Arduino, because it could be an unwanted disturbance if there's not such system. This part of the code :

https://github.com/showtime58/C-Arduino-Serial-Communication/blob/098cbdd0f89d66d93ec9978787ab31868366c977/emreC.c#L157-L163
