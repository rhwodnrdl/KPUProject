#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

#include "aws_iot_config.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"

char certDirectory[PATH_MAX + 1] = "../../../certs";
char HostAddress[255] = AWS_IOT_MQTT_HOST;
uint32_t port = AWS_IOT_MQTT_PORT;
uint32_t publishCount = 0;

         char sendTemp[17], sendTemp2[17],sendTemp3[17] = "";
         char sendHumid[17], sendHumid2[17], sendHumid3[17]= "";
         char sendCo2[17],  sendCo22[17], sendCo23[17]= "";
         char sendLpg[17], sendLpg2[17], sendLpg3[17]= "";
         char sendFan[15],  sendFan2[15], sendFan3[15]= "";
         char sendBuzzer[15], sendBuzzer2[15], sendBuzzer3[15]= "";
         char sendLight[15], sendLight2[15], sendLight3[15]= "";

char fandata, buzzerdata, lightdata; 

bool infinitePublishFlag = true;

	char rootCA[PATH_MAX + 1];
	char clientCRT[PATH_MAX + 1];
	char clientKey[PATH_MAX + 1];
	char CurrentWD[PATH_MAX + 1];

	IoT_Error_t rc = FAILURE;

	AWS_IoT_Client client;
	IoT_Client_Init_Params mqttInitParams = iotClientInitParamsDefault;
	IoT_Client_Connect_Params connectParams = iotClientConnectParamsDefault;


int desConFan, desConBuzzer, desConLight, desIden, desSetTemp, desSetHumid, desSetCo2;
int desConFan2, desConBuzzer2, desConLight2, desSetTemp2, desSetHumid2, desSetCo22;
int desConFan3, desConBuzzer3, desConLight3, desSetTemp3, desSetHumid3, desSetCo23;

        IoT_Publish_Message_Params paramsNum, paramsTemp,paramsHumid,paramsCo2,paramsLpg, paramsFan, paramsBuzzer, paramsLight;
        IoT_Publish_Message_Params paramsNum2, paramsTemp2,paramsHumid2,paramsCo22,paramsLpg2, paramsFan2, paramsBuzzer2, paramsLight2;
        IoT_Publish_Message_Params paramsNum3, paramsTemp3,paramsHumid3,paramsCo23,paramsLpg3, paramsFan3, paramsBuzzer3, paramsLight3;


void iot_subscribe_callback_handler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,IoT_Publish_Message_Params *params, void *pData) {

	char msgSetTemp[17], msgSetHumid[17], msgSetCo2[17] = "";
	char msgConFan[1], msgConBuzzer[1], msgConLight[1];
	char msgIden[1];
	char name[20];

         char front[12] = "{\"data\" : \""; //size(11)
         char tail[3] = "\"}"; //size(2)


	 paramsTemp.qos = QOS0;
         paramsTemp.isRetained = 0;
 
         paramsHumid.qos = QOS0;
         paramsHumid.isRetained = 0;
 
         paramsCo2.qos = QOS0;
         paramsCo2.isRetained = 0; 
 
         paramsLpg.qos = QOS0;
         paramsLpg.isRetained = 0;
 
         paramsFan.qos = QOS0;
         paramsFan.isRetained = 0;
 
         paramsBuzzer.qos = QOS0;
         paramsBuzzer.isRetained = 0;
 
         paramsLight.qos = QOS0;
         paramsLight.isRetained = 0;
        
         paramsTemp2.qos = QOS0;
         paramsTemp2.isRetained = 0;
 
         paramsHumid2.qos = QOS0;
         paramsHumid2.isRetained = 0;
 
         paramsCo22.qos = QOS0;
         paramsCo22.isRetained = 0; 
 
         paramsLpg2.qos = QOS0;
         paramsLpg2.isRetained = 0;
 
         paramsFan2.qos = QOS0;
         paramsFan2.isRetained = 0;
 
         paramsBuzzer2.qos = QOS0;
         paramsBuzzer2.isRetained = 0;
 
         paramsLight2.qos = QOS0;
         paramsLight2.isRetained = 0;
 
         paramsTemp3.qos = QOS0;
         paramsTemp3.isRetained = 0;
 
         paramsHumid3.qos = QOS0;
         paramsHumid3.isRetained = 0;
 
         paramsCo23.qos = QOS0;
         paramsCo23.isRetained = 0; 
 
         paramsLpg3.qos = QOS0;
         paramsLpg3.isRetained = 0;
 
         paramsFan3.qos = QOS0;
         paramsFan3.isRetained = 0;
 
         paramsBuzzer3.qos = QOS0;
         paramsBuzzer3.isRetained = 0;
 
         paramsLight3.qos = QOS0;
         paramsLight3.isRetained = 0;

	int i, num= 0;

	IOT_UNUSED(pData);
	IOT_UNUSED(pClient);
	IOT_INFO("%.*s\t%.*s\n", topicNameLen, topicName, (int) params->payloadLen, (char *) params->payload);
	sprintf(name, "%s", topicName);

	if(name[4] == 's' && name[12] == 't') { 
		msgIden[0] = 't';
		sprintf(msgSetTemp, "%s",params->payload);
		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		if(write(desSetTemp, msgSetTemp, 17)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("msgSetTemp: %s\n", msgSetTemp);
		params->payload = (void *)0;
		IOT_INFO("I'm done!\n");
	}
	if(name[4] == 's' && name[12] == 'h') { 
		msgIden[0] = 'h';
		sprintf(msgSetHumid, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		if(write(desSetHumid, msgSetHumid, 17)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		params->payload = '\0';
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 's' && name[12] == 'c') { 
		msgIden[0] = 'c';
		sprintf(msgSetCo2, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		if(write(desSetCo2, msgSetCo2, 17)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 'c' && name[12] == 'f') { 
		msgIden[0] = 'f';
		sprintf(msgConFan, "%s\n",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		if(write(desConFan, msgConFan, 15)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 'c' && name[12] == 'b') { 
		msgIden[0] = 'b';
		sprintf(msgConBuzzer, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		if(write(desConBuzzer, msgConBuzzer, 15)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 'c' && name[12] == 'l') { 
		msgIden[0] = 'l';
		sprintf(msgConLight, "%s",params->payload);
		
		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		if(write(desConLight, msgConLight, 15)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		params->payload = (void *)0;
		IOT_INFO("I'm done!\n")
	}

}

void iot_subscribe_callback_handler2(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
									IoT_Publish_Message_Params *params, void *pData) {

	char msgSetTemp[17], msgSetHumid[17], msgSetCo2[17] = "";
	char msgConFan[15], msgConBuzzer[15], msgConLight[15] = "";
	char msgIden[1];
	char name[20];

	IOT_UNUSED(pData);
	IOT_UNUSED(pClient);
	IOT_INFO("%.*s\t%.*s", topicNameLen, topicName, (int) params->payloadLen, (char *) params->payload);
	sprintf(name, "%s", topicName);

	if(name[4] == 's' && name[12] == 't') { 
		msgIden[0] = 'e';
		sprintf(msgSetTemp, "%s",params->payload);
		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		if(write(desSetTemp2, msgSetTemp, 17)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}

		IOT_INFO("I'm done!\n")
	}
	if(name[4] == 's' && name[12] == 'h') { 
		msgIden[0] = 'u';
		sprintf(msgSetHumid, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
		}
		if(write(desSetHumid2, msgSetHumid, 17)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 's' && name[12] == 'c') { 
		msgIden[0] = 'o';
		sprintf(msgSetCo2, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
		}
		if(write(desSetCo22, msgSetCo2, 17)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 'c' && name[12] == 'f') { 
		msgIden[0] = 'a';
		sprintf(msgConFan, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
		}
		if(write(desConFan2, msgConFan, 15)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 'c' && name[12] == 'b') { 
		msgIden[0] = 'z';
		sprintf(msgConBuzzer, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
		}
		if(write(desConBuzzer2, msgConBuzzer, 15)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 'c' && name[12] == 'l') { 
		msgIden[0] = 'i';
		sprintf(msgConLight, "%s",params->payload);
		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		if(write(desConLight2, msgConLight, 15)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
	}

}


void iot_subscribe_callback_handler3(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
									IoT_Publish_Message_Params *params, void *pData) {

	char msgSetTemp[17], msgSetHumid[17], msgSetCo2[17] = "";
	char msgConFan[15], msgConBuzzer[15], msgConLight[15] = "";
	char msgIden[1];
	char name[20] = "";
	int i = 0;

	IOT_UNUSED(pData);
	IOT_UNUSED(pClient);
	IOT_INFO("%.*s\t%.*s", topicNameLen, topicName, (int) params->payloadLen, (char *) params->payload);
	sprintf(name, "%s", topicName);
	if(name[4] == 's' && name[12] == 't') { 
		msgIden[0] = 'm';
		sprintf(msgSetTemp, "%s",params->payload);

		printf("data : %s", msgSetTemp); 
		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		if(write(desSetTemp3, msgSetTemp, 17)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
	}
	if(name[4] == 's' && name[12] == 'h') { 
		msgIden[0] = 'd';
		sprintf(msgSetHumid, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
		}
		if(write(desSetHumid3, msgSetHumid, 17)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 's' && name[12] == 'c') { 
		msgIden[0] = '2';
		sprintf(msgSetCo2, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
		}
		if(write(desSetCo23, msgSetCo2, 17)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 'c' && name[12] == 'f') { 
		msgIden[0] = 'n';
		sprintf(msgConFan, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
		}
		if(write(desConFan3, msgConFan, 15)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 'c' && name[12] == 'b') { 
		msgIden[0] = 'r';
		sprintf(msgConBuzzer, "%s",params->payload);

		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
		}
		if(write(desConBuzzer3, msgConBuzzer, 15)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	if(name[4] == 'c' && name[12] == 'l') { 
		msgIden[0] = 'g';
		sprintf(msgConLight, "%s",params->payload);
			
		if(write(desIden, msgIden, 1)==-1){
            	printf("fail to call write()\n");
            	exit(1);
		}
		if(write(desConLight3, msgConLight, 15)==-1){
            	printf("fail to call write()\n");
            	exit(1);
        	}
		IOT_INFO("I'm done!\n")
        }
	

}

void disconnectCallbackHandler(AWS_IoT_Client *pClient, void *data) {
	IOT_WARN("MQTT Disconnect");
	IoT_Error_t rc = FAILURE;

	if(NULL == pClient) {
		return;
	}

	IOT_UNUSED(data);

	if(aws_iot_is_autoreconnect_enabled(pClient)) {
		IOT_INFO("Auto Reconnect is enabled, Reconnecting attempt will start now");
	} else {
		IOT_WARN("Auto Reconnect not enabled. Starting manual reconnect...");
		rc = aws_iot_mqtt_attempt_reconnect(pClient);
		if(NETWORK_RECONNECTED == rc) {
			IOT_WARN("Manual Reconnect Successful");
		} else {
			IOT_WARN("Manual Reconnect Failed - %d", rc);
		}

	}
}

void parseInputArgsForConnectParams(int argc, char **argv) {
	int opt;

	while(-1 != (opt = getopt(argc, argv, "h:p:c:x:"))) {
		switch(opt) {
			case 'h':
				strcpy(HostAddress, optarg);
				IOT_DEBUG("Host %s", optarg);
				break;
			case 'p':
				port = atoi(optarg);
				IOT_DEBUG("arg %s", optarg);
				break;
			case 'c':
				strcpy(certDirectory, optarg);
				IOT_DEBUG("cert root directory %s", optarg);
				break;
			case 'x':
				publishCount = atoi(optarg);
				IOT_DEBUG("publish %s times\n", optarg);
				break;
			case '?':
				if(optopt == 'c') {
					IOT_ERROR("Option -%c requires an argument.", optopt);
				} else if(isprint(optopt)) {
					IOT_WARN("Unknown option `-%c'.", optopt);
				} else {
					IOT_WARN("Unknown option character `\\x%x'.", optopt);
				}
				break;
			default:
				IOT_ERROR("Error in command line argument parsing");
				break;
		}
	}

}

void sigint(int sig){

	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/temp");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/humid");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/co2");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/fan");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/buzzer");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/light");

	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/iden");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/humid");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/temp");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/co2");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/fan");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/buzzer");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/light");

	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/temp");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/humid");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/co2");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/fan");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/buzzer");
	unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/light");

	exit(1);
}

int main(int argc, char **argv) {

	IoT_Publish_Message_Params params;

	parseInputArgsForConnectParams(argc, argv);


	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/temp",0666) == -1){
        	printf("fail to call temp1()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/humid",0666) == -1){
        	printf("fail to call temp3()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/co2",0666)== -1){
        	printf("fail to call temp5()\n");
        	exit(1);
	}

	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/fan",0666) == -1){
        	printf("fail to call temp11()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/buzzer",0666) == -1){
        	printf("fail to call temp13()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/light",0666) == -1){
        	printf("fail to call temp13()\n");
        	exit(1);
	}

	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/iden",0666) == -1){
        	printf("fail to call iden\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/temp",0666) == -1){
        	printf("fail to call temp1()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/humid",0666) == -1){
        	printf("fail to call temp3()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/co2",0666)== -1){
        	printf("fail to call temp5()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/fan",0666) == -1){
        	printf("fail to call temp11()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/buzzer",0666) == -1){
        	printf("fail to call temp13()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/light",0666) == -1){
        	printf("fail to call temp13()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/temp",0666) == -1){
        	printf("fail to call temp1()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/humid",0666) == -1){
        	printf("fail to call temp3()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/co2",0666)== -1){
        	printf("fail to call temp5()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/fan",0666) == -1){
        	printf("fail to call temp11()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/buzzer",0666) == -1){
        	printf("fail to call temp13()\n");
        	exit(1);
	}
	if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/light",0666) == -1){
        	printf("fail to call temp13()\n");
        	exit(1);
	}

	if((desIden = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/iden", O_RDWR)) < 0){
        	printf("fail to call iden2()\n");
        	exit(1);
	}
	if((desSetTemp = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/temp", O_RDWR))< 0){
        	printf("fail to call temp2()\n");
        	exit(1);
	}
	if((desSetHumid = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/humid", O_RDWR)) < 0){
        	printf("fail to call temp4()\n");
        	exit(1);
	}
	if((desSetCo2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/setting/co2", O_RDWR)) < 0){
        	printf("fail to call temp6()\n");
        	exit(1);
	}
	if((desConFan = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/fan", O_RDWR)) < 0){
        	printf("fail to call temp12()\n");
        	exit(1);
	}
	if((desConBuzzer = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/buzzer", O_RDWR)) < 0){
        	printf("fail to call temp14()\n");
        	exit(1);
	}
	if((desConLight = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/control/light", O_RDWR)) < 0){
        	printf("fail to call temp14()\n");
        	exit(1);
	}

	if((desSetTemp2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/temp", O_RDWR))< 0){
        	printf("fail to call temp2()\n");
        	exit(1);
	}
	if((desSetHumid2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/humid", O_RDWR)) < 0){
        	printf("fail to call temp4()\n");
        	exit(1);
	}
	if((desSetCo22 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/setting/co2", O_RDWR)) < 0){
        	printf("fail to call temp6()\n");
        	exit(1);
	}
	if((desConFan2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/fan", O_RDWR)) < 0){
        	printf("fail to call temp12()\n");
        	exit(1);
	}
	if((desConBuzzer2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/buzzer", O_RDWR)) < 0){
        	printf("fail to call temp14()\n");
        	exit(1);
	}
	if((desConLight2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/control/light", O_RDWR)) < 0){
        	printf("fail to call temp14()\n");
        	exit(1);
	}
	if((desSetTemp3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/temp", O_RDWR))< 0){
        	printf("fail to call temp2()\n");
        	exit(1);
	}
	if((desSetHumid3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/humid", O_RDWR)) < 0){
        	printf("fail to call temp4()\n");
        	exit(1);
	}
	if((desSetCo23 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/setting/co2", O_RDWR)) < 0){
        	printf("fail to call temp6()\n");
        	exit(1);
	}
	if((desConFan3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/fan", O_RDWR)) < 0){
        	printf("fail to call temp12()\n");
        	exit(1);
	}
	if((desConBuzzer3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/buzzer", O_RDWR)) < 0){
        	printf("fail to call temp14()\n");
        	exit(1);
	}
	if((desConLight3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/control/light", O_RDWR)) < 0){
        	printf("fail to call temp14()\n");
        	exit(1);
	}


	IOT_INFO("\nAWS IoT SDK Version %d.%d.%d-%s\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_TAG);

	getcwd(CurrentWD, sizeof(CurrentWD));
	snprintf(rootCA, PATH_MAX + 1, "%s/%s/%s", CurrentWD, certDirectory, AWS_IOT_ROOT_CA_FILENAME);
	snprintf(clientCRT, PATH_MAX + 1, "%s/%s/%s", CurrentWD, certDirectory, AWS_IOT_CERTIFICATE_FILENAME);
	snprintf(clientKey, PATH_MAX + 1, "%s/%s/%s", CurrentWD, certDirectory, AWS_IOT_PRIVATE_KEY_FILENAME);

	IOT_DEBUG("rootCA %s", rootCA);
	IOT_DEBUG("clientCRT %s", clientCRT);
	IOT_DEBUG("clientKey %s", clientKey);
	mqttInitParams.enableAutoReconnect = false; // We enable this later below
	mqttInitParams.pHostURL = HostAddress;
	mqttInitParams.port = port;
	mqttInitParams.pRootCALocation = rootCA;
	mqttInitParams.pDeviceCertLocation = clientCRT;
	mqttInitParams.pDevicePrivateKeyLocation = clientKey;
	mqttInitParams.mqttCommandTimeout_ms = 20000;
	mqttInitParams.tlsHandshakeTimeout_ms = 5000;
	mqttInitParams.isSSLHostnameVerify = true;
	mqttInitParams.disconnectHandler = disconnectCallbackHandler;
	mqttInitParams.disconnectHandlerData = NULL;

	rc = aws_iot_mqtt_init(&client, &mqttInitParams);
	if(SUCCESS != rc) {
		IOT_ERROR("aws_iot_mqtt_init returned error : %d ", rc);
		return rc;
	}
	connectParams.keepAliveIntervalInSec = 600;
	connectParams.isCleanSession = true;
	connectParams.MQTTVersion = MQTT_3_1_1;
	connectParams.pClientID = (char *)AWS_IOT_MQTT_CLIENT_ID;
	connectParams.clientIDLen = (uint16_t) strlen(AWS_IOT_MQTT_CLIENT_ID);
	connectParams.isWillMsgPresent = false;


	IOT_INFO("Connecting...");
	rc = aws_iot_mqtt_connect(&client, &connectParams);
	if(SUCCESS != rc) {
		IOT_ERROR("Error(%d) connecting to %s:%d", rc, mqttInitParams.pHostURL, mqttInitParams.port);
		return rc;
	}

	/*
	 * Enable Auto Reconnect functionality. Minimum and Maximum time of Exponential backoff are set in aws_iot_config.h
	 *  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
	 *  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL
	 */
	rc = aws_iot_mqtt_autoreconnect_set_status(&client, true);
	if(SUCCESS != rc) {
		IOT_ERROR("Unable to set Auto Reconnect to true - %d", rc);
		return rc;
	}


	

	rc = aws_iot_mqtt_subscribe(&client, "ws2/setting/temp", 16, QOS0, iot_subscribe_callback_handler2, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws2/setting/humid", 17, QOS0, iot_subscribe_callback_handler2, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws2/setting/co2", 15, QOS0, iot_subscribe_callback_handler2, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws2/control/fan", 15, QOS0, iot_subscribe_callback_handler2, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws2/control/buzzer", 18, QOS0, iot_subscribe_callback_handler2, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws2/control/light", 17, QOS0, iot_subscribe_callback_handler2, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	IOT_INFO("Subscribing...");
	rc = aws_iot_mqtt_subscribe(&client, "ws1/setting/temp", 16, QOS0, iot_subscribe_callback_handler, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws1/setting/humid", 17, QOS0, iot_subscribe_callback_handler, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws1/setting/co2", 15, QOS0, iot_subscribe_callback_handler, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws1/control/fan", 15, QOS0, iot_subscribe_callback_handler, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws1/control/buzzer", 18, QOS0, iot_subscribe_callback_handler, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws1/control/light", 17, QOS0, iot_subscribe_callback_handler, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	
	rc = aws_iot_mqtt_subscribe(&client, "ws3/setting/temp", 16, QOS0, iot_subscribe_callback_handler3, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws3/setting/humid", 17, QOS0, iot_subscribe_callback_handler3, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws3/setting/co2", 15, QOS0, iot_subscribe_callback_handler3, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws3/control/fan", 15, QOS0, iot_subscribe_callback_handler3, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws3/control/buzzer", 18, QOS0, iot_subscribe_callback_handler3, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}
	rc = aws_iot_mqtt_subscribe(&client, "ws3/control/light", 17, QOS0, iot_subscribe_callback_handler3, NULL);
	if(SUCCESS != rc) {
		IOT_ERROR("Error subscribing : %d ", rc);
		return rc;
	}


	//params
	if(publishCount != 0) {
		infinitePublishFlag = false;
	}

	signal(SIGINT,sigint);
	

	while((NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc || SUCCESS == rc)
		  && (publishCount > 0 || infinitePublishFlag)) {

		//Max time the yield function will wait for read messages
		rc = aws_iot_mqtt_yield(&client, 100);


		if(NETWORK_ATTEMPTING_RECONNECT == rc) {
			// If the client is attempting to reconnect we will skip the rest of the loop.
			continue;
		}
		
	        sleep(1);

	        }


	if(SUCCESS != rc) {
		IOT_ERROR("An error occurred in the loop.\n");
	} else {
		IOT_INFO("Publish done\n"); 
	}

	return rc;
}
