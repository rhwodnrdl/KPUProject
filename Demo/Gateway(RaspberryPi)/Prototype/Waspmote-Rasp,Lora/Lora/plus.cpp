#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
 
#include "arduPiLoRa.h"
 
int e, num, co2, lpg, fan, buzzer, light;
float temp, humid;
char my_packet[100];
char idenstr[1];
char tempstr[10], tempstr2[10], tempstr3[10];
char humidstr[10], humidstr2[10], humidstr3[10];
char co2str[10], co2str2[10], co2str3[10];
char lpgstr[10], lpgstr2[10], lpgstr3[10];
char fanstr[10], fanstr2[10], fanstr3[10];
char buzzerstr[10], buzzerstr2[10], buzzerstr3[10];
char lightstr[10], lightstr2[10], lightstr3[10];


int tempdes,tempdes2,tempdes3,Idendes=0;
int humiddes, humiddes2,humiddes3=0;
int co2des,co2des2,co2des3=0;
int lpgdes,lpgdes2,lpgdes3=0;
int fandes,fandes2,fandes3=0;
int buzzerdes,buzzerdes2,buzzerdes3=0;
int lightdes,lightdes2,lightdes3=0;

int desConFan, desConBuzzer, desIden, desConFan2, desConBuzzer2, desConLight, desConLight2, i;
int desSetTemp, desSetHumid, desSetCo2, desSetTemp2, desSetHumid2, desSetCo22;
int desSetTemp3, desSetHumid3, desSetCo23, desConFan3, desConBuzzer3, desConLight3;
int readSetTemp, readSetHumid, readSetCo2, readConFan, readConBuzzer, readConLight, readIden;
int readSetTemp2, readSetHumid2, readSetCo22, readConFan2, readConBuzzer2, readConLight2;
int readSetTemp3, readSetHumid3,readSetCo23,readConFan3,readConBuzzer3, readConLight3;
char settemp[17], sethumid[17], setco2[17], confan[15], conbuzzer[15], conlight[15];
char settemp2[17], sethumid2[17], setco22[17], confan2[15], conbuzzer2[15], conlight2[15];
char settemp3[17], sethumid3[17], setco23[17], confan3[15], conbuzzer3[15], conlight3[15] = ""; 
char resultpacket[7], des[0], a;

int node = 0;

void setup()
{
 if ((Idendes = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/iden", O_RDWR)) < 0) {
  printf("fail to call open()1\n");
  exit(1);
 }
 if ((tempdes = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_temp", O_RDWR)) < 0) {
  printf("fail to call open()1\n");
  exit(1);
 }
 if ((humiddes = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_humid", O_RDWR)) < 0) {
  exit(1);
 }
 if ((co2des = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_co2", O_RDWR)) < 0) {
  printf("fail to call open()3\n");
  exit(1);
 }
 if ((lpgdes = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_lpg", O_RDWR)) < 0) {
  printf("fail to call open()4\n");
  exit(1);
 }
 if ((fandes = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_fan", O_RDWR)) < 0) {
  printf("fail to call open()6\n");
  exit(1);
 }
 if ((buzzerdes = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_buzzer", O_RDWR)) < 0) {
  printf("fail to call open()7\n");
  exit(1);
 }
 if ((lightdes = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_light", O_RDWR)) < 0) {
  printf("fail to call open()7\n");
  exit(1);
 }
 
 if ((tempdes2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_temp", O_RDWR)) < 0) {
  printf("fail to call open()\n");
  exit(1);
 }
 if ((humiddes2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_humid", O_RDWR)) < 0) {
  printf("fail to call open()\n");
  exit(1);
 }
 if ((co2des2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_co2", O_RDWR)) < 0) {
  printf("fail to call open()\n");
  exit(1);
 }
 if((lpgdes2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_lpg", O_RDWR)) < 0){
 printf("fail to call open()\n");
 exit(1);
 }
 
 if ((fandes2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_fan", O_RDWR)) < 0) {
  printf("fail to call open()\n");
  exit(1);
 }
 if ((buzzerdes2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_buzzer", O_RDWR)) < 0) {
  printf("fail to call open()\n");
  exit(1);
 }
 if((lightdes2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_light", O_RDWR)) < 0){
 printf("fail to call open()\n");
 exit(1);
 }
  if((tempdes3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_temp", O_RDWR)) < 0){
      printf("fail to call open()\n");
      exit(1);
  }
  if((humiddes3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_humid", O_RDWR)) < 0){
      printf("fail to call open()\n");
      exit(1);
  }
  if((co2des3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_co2", O_RDWR)) < 0){
      printf("fail to call open()\n");
      exit(1);
  }
  if((lpgdes3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_lpg", O_RDWR)) < 0){
      printf("fail to call open()\n");
      exit(1);
  }

  if((fandes3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_fan", O_RDWR)) < 0){
      printf("fail to call open()\n");
      exit(1);
  }
  if((buzzerdes3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_buzzer", O_RDWR)) < 0){
      printf("fail to call open()\n");
      exit(1);
  }
  if((lightdes3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_light", O_RDWR)) < 0){
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
 e |= sx1272.setChannel(CH_00_900);
 printf("Setting Channel: state %d\n", e);



 // Set CRC
 e |= sx1272.setCRC_ON();
 printf("Setting CRC ON: state %d\n", e);
 
 // Select output power (Max, High or Low)
 e |= sx1272.setPower('M');
 printf("Setting Power: state %d\n", e);
 
 // Print a success message
 if (e == 0)
  printf("SX1272 successfully configured\n");
 else
  printf("SX1272 initialization failed\n");
 
 delay(1000);
 
}
 
 
void control_setup()
{
 
 if ((desIden = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/iden", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call iden2()\n");
  exit(1);
 }
 if ((desSetTemp = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/temp", O_RDWR | O_NONBLOCK))< 0) {
  printf("fail to call temp2()\n");
  exit(1);
 }
 if ((desSetHumid = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/humid", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call temp4()\n");
  exit(1);
 }
 if ((desSetCo2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/co2", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call temp6()\n");
  exit(1);
 }
 if ((desConFan = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/fan", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call temp12()\n");
  exit(1);
 }
 if ((desConBuzzer = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/buzzer", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call temp14()\n");
  exit(1);
 }
 if ((desConLight = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/light", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call temp14()\n");
  exit(1);
 }
 if ((desSetTemp2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/temp", O_RDWR | O_NONBLOCK))< 0) {
  printf("fail to call temp2()\n");
  exit(1);
 }
 if ((desSetHumid2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/humid", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call temp4()\n");
  exit(1);
 }
 if ((desSetCo22 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/co2", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call temp6()\n");
  exit(1);
 }
 if ((desConFan2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/fan", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call temp12()\n");
  exit(1);
 }
 if ((desConBuzzer2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/buzzer", O_RDWR | O_NONBLOCK)) < 0) {
  printf("fail to call temp14()\n");
  exit(1);
 }
 if((desConLight2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/light", O_RDWR | O_NONBLOCK)) < 0){
 printf("fail to call temp14()\n");
 exit(1);
 }
 if((desSetTemp3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/temp", O_RDWR | O_NONBLOCK)) < 0){
        	printf("fail to call temp2()\n");
        	exit(1);
	}
 if((desSetHumid3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/humid", O_RDWR | O_NONBLOCK)) < 0){
        	printf("fail to call temp4()\n");
        	exit(1);
	}
 if((desSetCo23 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/co2", O_RDWR | O_NONBLOCK)) < 0){
        	printf("fail to call temp6()\n");
        	exit(1);
 }
 if((desConFan3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/fan", O_RDWR | O_NONBLOCK)) < 0){
        	printf("fail to call temp12()\n");
        	exit(1);
 }
 if((desConBuzzer3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/buzzer", O_RDWR | O_NONBLOCK)) < 0){
        	printf("fail to call temp14()\n");
        	exit(1);
	}
if((desConLight3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/light", O_RDWR | O_NONBLOCK)) < 0){
        	printf("fail to call temp14()\n");
        	exit(1);
	}
}
 
void Rasp_to_AWS()//from test.cpp
{
  int f_node = (node % 3) + 2;
  char buf[2]=""; 

  if(f_node == 2) {
	buf[0] = '4';
  	e = sx1272.setNodeAddress(7);
  	//printf("Setting Node address 7\n");
  }

  if(f_node == 3) { 
	buf[0] = '5';
  	e = sx1272.setNodeAddress(8);
  	//printf("Setting Node address 8\n");
  }

  if(f_node == 4) {
	buf[0] = '6';
  	e = sx1272.setNodeAddress(9);
  	//printf("Setting Node address 9\n");
  }
  
  e = sx1272.sendPacketTimeout(f_node, buf);
  printf("===========send %d!==========\n",f_node-1);
  node++;

  e = sx1272.receivePacketTimeout(5000);
 

  if(e == 0) {
  printf("===========receive from %dboard==========\n",f_node-1);
 
   for (unsigned int i = 0; i < sx1272.packet_received.length; i++){
    my_packet[i] = (char)sx1272.packet_received.data[i];
   }

   printf("my packet : %s\n", my_packet);

   num = my_packet[0] - 48;
   temp = (my_packet[1] - 48) * 100 + (my_packet[2] - 48) * 10 + (my_packet[3] - 48);
   humid = (my_packet[4] - 48) * 100 + (my_packet[5] - 48) * 10 + (my_packet[6] - 48);
   co2 = (my_packet[7] - 48) * 1000 + (my_packet[8] - 48) * 100 + (my_packet[9] - 48) * 10 + my_packet[10];
   lpg = (my_packet[11] - 48) * 1000 + (my_packet[12] - 48) * 100 + (my_packet[13] - 48) * 10 + my_packet[14];
   fan = my_packet[15] - 48;
   buzzer = my_packet[16] - 48;
   light = my_packet[17] - 48;
 
   printf("num: %d\n", num);
   printf("temp: %.1lf\n", temp / 10);
   printf("humid: %.1lf\n", humid / 10);
   printf("co2: %d\n", co2);
   printf("lpg: %d\n", lpg);
   printf("fan: %d\n", fan);
   printf("Buzzer: %d\n", buzzer);
   printf("Light: %d\n", light);
  }	
  if (num == 1) {
 
   idenstr[0] = '1';
    sprintf(tempstr, "%.1lf", temp / 10);
    sprintf(humidstr, "%.1lf", humid / 10);
    sprintf(co2str, "%d", co2);
    sprintf(lpgstr, "%d", lpg);
    sprintf(fanstr, "%d", fan);
    sprintf(buzzerstr, "%d", buzzer);
    sprintf(lightstr, "%d", light);
 
    if (write(Idendes, idenstr, 1) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(tempdes, tempstr, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(humiddes, humidstr, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(co2des, co2str, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(lpgdes, lpgstr, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(fandes, fanstr, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(buzzerdes, buzzerstr, 10) == -1) {
     printf("fail to c all write()\n");
     exit(1);
    }
    if (write(lightdes, lightstr, 10) == -1) {
     printf("fail to c all write()\n");
     exit(1);
    }

	printf("I'm done!\n");
 
   }
 
   if (num == 2) {
 
    idenstr[0] = '2';
    
    sprintf(tempstr2, "%.1lf", temp / 10);
    sprintf(humidstr2, "%.1lf", humid / 10);
    sprintf(co2str2, "%d", co2);
    sprintf(lpgstr2, "%d", lpg);
    sprintf(fanstr2, "%d", fan);
    sprintf(buzzerstr2, "%d", buzzer);
    sprintf(lightstr2, "%d", light);
 
    if (write(Idendes, idenstr, 1) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(tempdes2, tempstr2, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(humiddes2, humidstr2, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(co2des2, co2str2, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(lpgdes2, lpgstr2, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(fandes2, fanstr2, 10) == -1) {
     printf("fail to call write()\n");
     exit(1);
    }
    if (write(buzzerdes2, buzzerstr2, 10) == -1) {
     printf("fail to c all write()\n");
     exit(1);
    }
    if (write(lightdes2, lightstr2, 10) == -1) {
     printf("fail to c all write()\n");
     exit(1);
    }

	printf("I'm done!\n");
  }

  if (num == 3) 
  {	
	idenstr[0]= '3';
  	sprintf(tempstr3,"%.1lf",temp/10);
  	sprintf(humidstr3,"%.1lf",humid/10);
  	sprintf(co2str3, "%d",co2);
  	sprintf(lpgstr3,"%d",lpg);
  	sprintf(fanstr3, "%d",fan);
  	sprintf(buzzerstr3, "%d",buzzer);
	sprintf(lightstr3, "%d",light);

   	if(write(desIden, idenstr, 1)==-1){
    		printf("fail to call write()\n");
    		exit(1);}
	if(write(tempdes3, tempstr3, 10)==-1){
    		printf("fail to call write()\n");
    		exit(1);}
	if(write(humiddes3, humidstr3, 10)==-1){
    		printf("fail to call write()\n");
    		exit(1);}
  	if(write(co2des3, co2str3, 10)==-1){
		printf("fail to call write()\n");
    		exit(1);}
  	if(write(lpgdes3, lpgstr3, 10)==-1){
    		printf("fail to call write()\n");
    		exit(1);}
  	if(write(fandes3, fanstr3, 10)==-1){
    		printf("fail to call write()\n");
    		exit(1);}
  	if(write(buzzerdes3, buzzerstr3, 10)==-1){
    		printf("fail to c all write()\n");
    		exit(1);}
  	if(write(lightdes3, lightstr3, 10)==-1){
    		printf("fail to c all write()\n");
    		exit(1);}

	printf("I'm done!\n");
  }


if((readIden = read(desIden, des, 15)) > 0 ){

   a = des[0];
     switch (a) {
     case 't':
  	if ((readSetTemp = read(desSetTemp, settemp, 17))) {
   printf("receivepacket!\n");
   resultpacket[0] = '\0';
   resultpacket[0] = '1';
   resultpacket[1] = 'S';
   resultpacket[2] = 'T';
   resultpacket[3] = settemp[11];
   resultpacket[4] = settemp[12];
   resultpacket[5] = settemp[13];
   resultpacket[6] = settemp[14];
   settemp[0]= '\0';

   printf("wait...\n");
   delay(3000);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   printf("send\n");

   break;
  }
 case 'h':
  if ((readSetHumid = read(desSetHumid, sethumid, 17))) {
   printf("receivepacket!\n");
   resultpacket[0] = '\0';
   resultpacket[0] = '1';
   resultpacket[1] = 'S';
   resultpacket[2] = 'H';
   resultpacket[3] = sethumid[11];
   resultpacket[4] = sethumid[12];
   resultpacket[5] = sethumid[13];
   resultpacket[6] = sethumid[14];
   sethumid[0]= '\0';

   printf("wait...\n");
   delay(3000);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   printf("send\n");

   break;
  }
 case 'c':
  if ((readSetCo2 = read(desSetCo2, setco2, 17))) {
   printf("receivepacket!\n");
   resultpacket[0] = '\0';
   resultpacket[0] = '1';
   resultpacket[1] = 'S';
   resultpacket[2] = 'C';
   resultpacket[3] = setco2[11];
   resultpacket[4] = setco2[12];
   resultpacket[5] = setco2[13];
   resultpacket[6] = setco2[14];
   setco2[0]= '\0';

   printf("wait...\n");
   delay(3000);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   printf("send\n");

   break;
  }
 case 'f':
  if ((readConFan = read(desConFan, confan, 15))) {
   printf("receivepacket!\n");
   resultpacket[0] = '\0';
   resultpacket[0] = '1';
   resultpacket[1] = 'C';
   resultpacket[2] = 'F';
   resultpacket[3] = confan[11];
   confan[0]= '\0';

   printf("wait...\n");
   delay(3000);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   printf("send\n");

 
   break;
  }
 case 'b':
  if ((readConBuzzer = read(desConBuzzer, conbuzzer, 15))) {
   printf("receivepacket!\n");
   resultpacket[0] = '\0';
   resultpacket[0] = '1';
   resultpacket[1] = 'C';
   resultpacket[2] = 'B';
   resultpacket[3] = conbuzzer[11];
   conbuzzer[0]= '\0';

   printf("wait...\n");
   delay(3000);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   printf("send\n");

 
   break;
  }
 case 'l':
  if ((readConLight = read(desConLight, conlight, 15))) {
   printf("receivepacket!\n");

   resultpacket[0] = '\0';
   resultpacket[0] = '1';
   resultpacket[1] = 'C';
   resultpacket[2] = 'L';
   resultpacket[3] = conlight[11];
   conlight[0]= '\0';
   printf("wait...\n");

   delay(3000);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   e = sx1272.sendPacketTimeout(2, resultpacket);
   printf("send\n");

   break;
  }
 case 'e':
  if ((readSetTemp2 = read(desSetTemp2, settemp2, 17))) {
   printf("receivepacket!\n");
   resultpacket[0] = '\0';
   resultpacket[0] = '2';
   resultpacket[1] = 'S';
   resultpacket[2] = 'T';
   resultpacket[3] = settemp2[11];
   resultpacket[4] = settemp2[12];
   resultpacket[5] = settemp2[13];
   resultpacket[6] = settemp2[14];
   settemp2[0]= '\0';

   printf("wait...\n");
   delay(3000);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   printf("send\n");

   break;
  }
 case 'u':
  if ((readSetHumid2 = read(desSetHumid2, sethumid2, 17))) {
   printf("receivepacket!\n");
   resultpacket[0] = '\0';
   resultpacket[0] = '2';
   resultpacket[1] = 'S';
   resultpacket[2] = 'H';
   resultpacket[3] = sethumid2[11];
   resultpacket[4] = sethumid2[12];
   resultpacket[5] = sethumid2[13];
   resultpacket[6] = sethumid2[14];
   sethumid2[0]= '\0';

   printf("wait...\n");
   delay(3000);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   printf("send\n");

   break;
  }
 case 'o':
  if ((readSetCo22 = read(desSetCo22, setco22, 17))) {

   printf("receivepacket!\n");
   resultpacket[0] = '\0';
   resultpacket[0] = '2';
   resultpacket[1] = 'S';
   resultpacket[2] = 'C';
   resultpacket[3] = setco22[11];
   resultpacket[4] = setco22[12];
   resultpacket[5] = setco22[13];
   resultpacket[6] = setco22[14];
   setco22[0]= '\0';

   printf("wait...\n");
   delay(3000);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   printf("send\n");

 
   break;
  }
 case 'a':
  if ((readConFan2 = read(desConFan2, confan2, 15))) {
   printf("receivepacket!\n");

   resultpacket[0] = '\0';


   resultpacket[0] = '2';
   resultpacket[1] = 'C';
   resultpacket[2] = 'F';
   resultpacket[3] = confan2[11];

   confan2[0]= '\0';

   printf("wait...\n");
   delay(3000);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   e = sx1272.sendPacketTimeout(3, resultpacket);
   e = sx1272.sendPacketTimeout(3, resultpacket);

 
   printf("send\n");

 
   break;
  }
 case 'z':
  if ((readConBuzzer2 = read(desConBuzzer2, conbuzzer2, 15))) {
   printf("receivepacket!\n");
	resultpacket[0] = '\0';
   	resultpacket[0] = '2';
   	resultpacket[1] = 'C';
   	resultpacket[2] = 'B';
   	resultpacket[3] = conbuzzer2[11];

	conbuzzer2[0] = '\0';
   	printf("wait...\n");
	delay(3000);
   	e = sx1272.sendPacketTimeout(3, resultpacket);
   	e = sx1272.sendPacketTimeout(3, resultpacket);
   	e = sx1272.sendPacketTimeout(3, resultpacket);
   	printf("send\n");
   
   
   break;
  }
 case 'i':
  if ((readConLight2 = read(desConLight2, conlight2, 15))) {
   printf("receivepacket!\n");
   	resultpacket[0] = '\0';
   	resultpacket[0] = '2';
   	resultpacket[1] = 'C';
   	resultpacket[2] = 'L';
   	resultpacket[3] = conlight2[11];
	conbuzzer2[0] = '\0';

 	printf("wait...\n");
	delay(3000);
   	e = sx1272.sendPacketTimeout(3, resultpacket);
   	e = sx1272.sendPacketTimeout(3, resultpacket);
   	e = sx1272.sendPacketTimeout(3, resultpacket);
   	printf("send\n");

   break;
  }
        case 'm' : 
	if((readSetTemp3 = read(desSetTemp3, settemp3, 17))){

		printf("receivepacket!\n");
		resultpacket[0]='\0';
		resultpacket[0]='3';
		resultpacket[1]='S';
		resultpacket[2]='T';
		resultpacket[3]=settemp3[11];
		resultpacket[4]=settemp3[12];
		resultpacket[5]=settemp3[13];
		resultpacket[6]=settemp3[14];
		settemp3[0] = '\0';

		printf("wait...\n");
		delay(3000);
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);	
	
		printf("send\n");
	break;}
				
	case 'd' : 
	if((readSetHumid3= read(desSetHumid3, sethumid3,17))){
		
		printf("receivepacket!\n");
		resultpacket[0]='\0';
		resultpacket[0]='3';
		resultpacket[1]='S';
		resultpacket[2]='H';

		resultpacket[3]=sethumid3[11];
		resultpacket[4]=sethumid3[12];
		resultpacket[5]=sethumid3[13];
		resultpacket[6]=sethumid3[14];
		sethumid3[0] = '\0';

		delay(3000);
		printf("wait...\n");
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);

					
		printf("send\n");
									
	break;}
	
	case '2' : 		
	if((readSetCo23 = read(desSetCo23, setco23, 17))){
	
		printf("receivepacket!\n");
		resultpacket[0]='\0';
		resultpacket[0]='3';
		resultpacket[1]='S';		
		resultpacket[2]='C';		
		resultpacket[3]=setco23[11];
		resultpacket[4]=setco23[12];
		resultpacket[5]=setco23[13];
		resultpacket[6]=setco23[14];
		setco23[0] = '\0';

		delay(3000);
		printf("wait...\n");
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);
		printf("send\n");

	break;}
			
	case 'n' : 
	if((readConFan3 = read(desConFan3, confan3, 15))){
					
		printf("receivepacket!\n");
		resultpacket[0]='\0';
		resultpacket[0]='3';
		resultpacket[1]='C';
		resultpacket[2]='F';
		resultpacket[3]=confan3[11];
		confan3[0] = '\0';

		delay(3000);
		printf("wait...\n");
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);

		printf("send\n");
					
	break;}

	case 'r' : 
	if((readConBuzzer3 = read(desConBuzzer3, conbuzzer3, 15))){
					
		printf("receivepacket!\n");
		resultpacket[0]='\0';
		resultpacket[0]='3';
		resultpacket[1]='C';		
		resultpacket[2]='B';
		resultpacket[3]=conbuzzer3[11];
		conbuzzer3[0] = '\0';

		delay(3000);
		printf("wait...\n");
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);

		printf("send\n");
					
	break;}

	case 'g' : 
	if((readConLight3 = read(desConLight3, conlight3, 15))){
					
		printf("receivepacket!\n");
		resultpacket[0]='\0';
		resultpacket[0]='3';
		resultpacket[1]='C';
		resultpacket[2]='L';
		resultpacket[3]=conlight3[11];		
		conlight3[0] = '\0';
		
		delay(3000);
		printf("wait...\n");
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);
		e=sx1272.sendPacketTimeout(4,resultpacket);

		printf("send\n");
	
	break;}

 
 	} //switch
} //if
else {
    printf("not control data\n");
    delay(1000);
}

}//function

int main() {

 setup();
 control_setup();
 while(1){ 
	Rasp_to_AWS(); 
 } 
 return 0;
} 