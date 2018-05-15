#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "arduPiLoRa.h"

int e;
char my_packet[100];
float temp=0;
float humid=0;
char tempstr[50];
char humidstr[50];
int tempdes;
int humiddes;

void setup()
{
  if((tempdes = open("/home/pi/Demo/Raspberry-Web,MQTT/MQTT/linux/Publish/temp", O_WRONLY)) < 0){
      printf("fail to call open()\n");
      exit(1);
  }

  if((humiddes = open("/home/pi/Demo/Raspberry-Web,MQTT/MQTT/linux/Publish/humid", O_WRONLY)) < 0){
      printf("fail to call open()\n");
      exit(1);
  }
  
  // Print a start message
  printf("SX1272 module and Raspberry Pi: receive packets without ACK\n");
  
  // Power ON the module
  e = sx1272.ON();
  printf("Setting power ON: state %d\n", e);
  
  // Set transmission mode
  e |= sx1272.setMode(1);
  printf("Setting Mode: state %d\n", e);
  
  // Set header
  e |= sx1272.setHeaderON();
  printf("Setting Header ON: state %d\n", e);
  
  // Select frequency channel
  e |= sx1272.setChannel(CH_10_868);
  printf("Setting Channel: state %d\n", e);
  
  // Set CRC
  e |= sx1272.setCRC_ON();
  printf("Setting CRC ON: state %d\n", e);
  
  // Select output power (Max, High or Low)
  e |= sx1272.setPower('L');
  printf("Setting Power: state %d\n", e);
  
  // Set the node address
  e |= sx1272.setNodeAddress(8);
  printf("Setting Node address: state %d\n", e);
  
  // Print a success message
  if (e == 0)
    printf("SX1272 successfully configured\n");
  else
    printf("SX1272 initialization failed\n");

  delay(1000);
}

void loop(void)
{
  // Receive message
  e = sx1272.receivePacketTimeout(10000);
  if ( e == 0 )
  {
    printf("Receive packet, state %d\n",e);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
  }
  else {
    printf("Receive packet, state %d\n",e);
  }
  temp=(my_packet[0]-48)*100+(my_packet[1]-48)*10+(my_packet[3]-48);
  humid=(my_packet[5]-48)*100+(my_packet[6]-48)*10+(my_packet[8]-48);
  printf("temp: %.1f , humid : %.1f\n", temp/10, humid/10);

  sprintf(tempstr, "%.1f", temp/10);

  sprintf(humidstr, "%.1f", humid/10);

  if(write(tempdes, tempstr, 50)==-1){
    printf("fail to call write()\n");
    exit(1);
  }

  if(write(humiddes, humidstr, 50)==-1){
    printf("fail to call write()\n");
    exit(1);
  }
}

int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}
