#include <stdio.h>     
#include <stdlib.h>	
#include <string.h>	
#include <fcntl.h>     /* File Control Defitions */
#include <termios.h>	/* POSIX Terminal Control Definitions */
#include <unistd.h>    /* UNIX Standart Function Definitions */
#include <sys/ioctl.h>

void displayMenu(){
  printf("\033[36;1m## WELCOME TO GTU ARDUINO LAB        ##\n");
  printf("## STUDENT NAME: EMRE OYTUN          ##\n");
  printf("## PLEASE SELECT FROM THE FOLLOWING  ##\n");
  printf("## MENU :                            ##\n");
  printf("(1) TURN ON LED ON ARDUINO\n");
  printf("(2) TURN OFF LED ON ARDUINO\n");
  printf("(3) FLASH ARDUINO LED 3 TIMES\n");
  printf("(4) SEND A NUMBER TO ARDUINO TO COMPUTE SQUARE BY ARDUINO\n");
  printf("(5) Button press counter (bonus item)\n");
  printf("(0) EXIT\n");
  printf("\033[31;1mPLEASE SELECT:\n\033[0m");
}

int serialport_read_until(int fd, char* buf, char until, int buf_max, int timeout){	/* Function for reading from Arduino until a specified character,which i got from library and modified */
    char b[1];  // read expects an array, so we give it a 1-byte array
    int i=0;
    do { 
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 1 * 1000 );  // wait 1 msec try again
            timeout--;
            if( timeout==0 ) return -2;
            continue;
        }
#ifdef SERIALPORTDEBUG  
        printf("serialport_read_until: i=%d, n=%d b='%c'\n",i,n,b[0]); // debug
#endif
        if(b[0] != ' ' || b[0] != '\n'){
        buf[i] = b[0]; 
        }
        i++;
    } while( b[0] != until && i < buf_max && timeout>0 );

    buf[i-1] = 0;  // null terminate the string in the place of until character
    return 0;
}

int start_serial_communication(const char* serialPort){ 	/* Function for starting serial communication with Arduino and setting communication protocols, which i got from library and modified */
	int port = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY); // Open the file descriptor in non-blocking mod
	if(port == -1){ 
	printf("Sorry, there's some problem with connection.\n");
	printf("Please check device path then try again...\n"); 
	getchar();
	getchar();
	return -1;
	}
	
	struct termios options; // Set up the control structure
	tcgetattr(port, &options); // Get currently set options for the tty
	//Set baud rate
	cfsetispeed(&options, B9600); // 9600 Baud
	cfsetospeed(&options, B9600);
	options.c_cflag |= (CLOCAL | CREAD);
	tcsetattr(port, TCSANOW, &options);
	//8 bit characters	
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
	options.c_cflag |= CS8;    /* Select 8 data bits */
	//No parity
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	
	options.c_cc[VMIN]  = 0;
    	options.c_cc[VTIME] = 0;
    	return port;
}

int counter(int port){
	while(1){
		char status;
	 	printf("Enter 'd' to display the counter // Enter 'q' to return the menu: ");
	 	scanf("%c",&status);
	 	if(status == '\n'){
	 		scanf("%c",&status);
	 	}
	 	write(port,&status,1);
	 	if(status == 'q'){
	 		return 0;
	 	}
	 	if(status == 'd'){
	 		char EofCharacter = 'f'; // for reading process
	 		char buf[20];
			sleep(1.5);
			serialport_read_until(port, buf, EofCharacter, 20, 2000);
			printf("%s",&buf[2]);
	 	}
    	}		
}

int main(){
	/* Start serial communication with Arduino */
	char *serialPort = "/dev/ttyUSB0"; /******************* WARNING : You should predetermine your port here *****************/
	int port;
	do{  	
	port = start_serial_communication(serialPort);
	}while(port<0);  // Loop until port opens
	printf("Connection is being established ...\n");
	sleep(1.5); // for Arduino to reboot
	
	
	
	int exit = 0;
	while(exit == 0){
		/* Display selection menu */
		displayMenu();
		

		/* Get selection */
		int input;
		fflush(stdin);		
		scanf("%d",&input);
		//char ch=getchar(); // for skipping one element in buffer
	
		if(input ==1){
		/* Send selection to Arduino to call related functions */
		 	char ch= 'a';
		 	write(port,&ch,1);
		}
		else if(input ==2){
			char ch= 'b';
			write(port,&ch,1);
		}
		else if(input ==3){
			char ch='c';
			write(port,&ch,1);
			sleep(3); // for blinking process
		}
		else if(input == 4){		
		/* Get the number whose square we'll find */
			tcflush(port, TCIFLUSH); // flush all serial buffer
			char input2[10];
			printf("Please enter a number: ");
			close(port); // first close serial communication to provide unwanted results 
			fflush(stdin);
			scanf("%s",input2); 
			
			/* Send selection to call square function in Arduino */
			start_serial_communication(serialPort);
			sleep(2); // for arduino to reboot
			char ch ='d';
			write(port,&ch,1);
			printf("Please wait, arduino is calculating the result!\n");
		
		/* Send number to Arduino */
			int len = strlen(input2); 
			write(port,&input2,len);
	 		
 		/* Read the result from Arduino */
 			char EofCharacter = 'f'; // for reading process
	 		int trigger = -1; // Trigger system to understand Arduino has finished calculations
	 		do{
	 			char status[1];
	 		trigger = serialport_read_until(port,status,EofCharacter,1,200);
	 		}while( trigger<0 );
	 		
	 		char buf[20];
	 		serialport_read_until(port, buf, EofCharacter, 20, 2000);
	 		printf("The result is: %s",&buf[1]);	 		
		}
		else if(input == 5){
			char ch = 'e';
			write(port,&ch,1);
			counter(port);			
		}	
		else if(input == 0){
			printf("###### Arduino is closing... ######\n");
			close(port);
			sleep(1.5);
			printf("###### Disconnected ! #####\n");
			exit = 1;
			
		}
		else{   /* Default */
			printf("You should enter a number between 0-5...\n");
			sleep(1);
		}	
	}
	return 0;
}
