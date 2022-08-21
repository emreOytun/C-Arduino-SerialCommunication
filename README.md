# C-Arduino-Serial-Communication
In this project, user gets Arduino do some tasks provided in menu. 

The objective of this project is to learn how to make a serial communication with devices such as Arduino and computer(C).

 It provides user to send input to/get output from Arduino and make Arduino do some tasks from computer.

# Technical Issue about Getting Output :

When Arduino sends output to computer, it is essential to support this communication by a trigger system in which Arduino first sends a character to computer before sending actual output when C program waits for this character to come before starting getting actual output. In that way, it's more secure to get output from Arduino, because it could be an unwanted disturbance if there's not such system. This part of the code :

https://github.com/emreOytun/C-Arduino-Serial-Communication/blob/7dc62341d3536e94d3acacc1ebde142d9be34e66/emreC.c#L157-L163

# Demo video :

https://drive.google.com/file/d/1cEWCoBAPL_QZp1GTZVePG2LwUioK8kFF/view?classId=0d7cd9ee-af50-409f-b9f0-4d79a8853b9d&assignmentId=a65eefa7-c368-4353-99ec-1bfaf23c5d4d&submissionId=a287b3e0-40bc-27f5-7cda-a1d7374cf1f2
