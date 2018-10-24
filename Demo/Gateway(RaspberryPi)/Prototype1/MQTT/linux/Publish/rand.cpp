#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
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
 

void iot_subscribe_callback_handler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
                                                                                IoT_Publish_Message_Params *params, void *pData) {
         IOT_UNUSED(pData);
         IOT_UNUSED(pClient);
         IOT_INFO("Subscribe callback");
         IOT_INFO("%.*s\t%.*s", topicNameLen, topicName, (int) params->payloadLen, (char *) params->payload);
        
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
 
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/iden");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_temp");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_humid");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_co2");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_lpg");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_fan");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_buzzer");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_light");
 
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_temp");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_humid");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_co2");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_lpg");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_fan");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_buzzer");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_light");
 
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_temp");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_humid");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_co2");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_lpg");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_fan");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_buzzer");
         unlink("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_light");
         exit(1);
}

 
int main(int argc, char **argv) {
 
         char msgTemp[10], msgTemp2[10],msgTemp3[10];
         char msgHumid[10], msgHumid2[10], msgHumid3[10];
         char msgCo2[10],  msgCo22[10], msgCo23[10];
         char msgLpg[10], msgLpg2[10], msgLpg3[10];
         char msgFan[10],  msgFan2[10], msgFan3[10], a;
         char msgBuzzer[10], msgBuzzer2[10], msgBuzzer3[10], msgIden[10];
         char msgLight[10], msgLight2[10], msgLight3[10];


         char tempdata[50];
         char humiddata[50];
         char co2data[50];
         char lpgdata[50];

         char sendTemp[17], sendTemp2[17],sendTemp3[17] = "";
         char sendHumid[17], sendHumid2[17], sendHumid3[17]= "";
         char sendCo2[17],  sendCo22[17], sendCo23[17]= "";
         char sendLpg[17], sendLpg2[17], sendLpg3[17]= "";
         char sendFan[15],  sendFan2[15], sendFan3[15]= "";
         char sendBuzzer[15], sendBuzzer2[15], sendBuzzer3[15]= "";
         char sendLight[15], sendLight2[15], sendLight3[15]= "";

         int desTemp,desHumid,desCo2,desLpg,desFan,desBuzzer,desLight,desIden;
         int desTemp2,desHumid2,desCo22,desLpg2,desFan2,desBuzzer2,desLight2;
         int desTemp3,desHumid3,desCo23,desLpg3,desFan3,desBuzzer3,desLight3;
         int readTemp,readHumid,readCo2,readLpg,readFan,readBuzzer,readLight,readIden;
         int readTemp2,readHumid2,readCo22,readLpg2,readFan2,readBuzzer2,readLight2;
         int readTemp3,readHumid3,readCo23,readLpg3,readFan3,readBuzzer3,readLight3;
 	
	char old_fan = '0';
	char old_buzzer= '0';
	char old_light = '0';
	
	char old_fan2 = '0';
	char old_buzzer2 = '0';
	char old_light2 = '0';

	char old_fan3 = '0';
	char old_buzzer3 = '0';
	char old_light3 = '0';

 
         int i = 0;
 
         char front[12] = "{\"data\" : \""; //size(11)
         char tail[3] = "\"}"; //size(2)
        
         bool infinitePublishFlag = true;
 
         char rootCA[PATH_MAX + 1];
         char clientCRT[PATH_MAX + 1];
         char clientKey[PATH_MAX + 1];
         char CurrentWD[PATH_MAX + 1];
 
         IoT_Error_t rc = FAILURE;
 
 
         AWS_IoT_Client client;
         IoT_Client_Init_Params mqttInitParams = iotClientInitParamsDefault;
         IoT_Client_Connect_Params connectParams = iotClientConnectParamsDefault;
 
         IoT_Publish_Message_Params paramsNum, paramsTemp,paramsHumid,paramsCo2,paramsLpg, paramsFan, paramsBuzzer, paramsLight;
         IoT_Publish_Message_Params paramsNum2, paramsTemp2,paramsHumid2,paramsCo22,paramsLpg2, paramsFan2, paramsBuzzer2, paramsLight2;
         IoT_Publish_Message_Params paramsNum3, paramsTemp3,paramsHumid3,paramsCo23,paramsLpg3, paramsFan3, paramsBuzzer3, paramsLight3;
 
         parseInputArgsForConnectParams(argc, argv);
 
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_temp",0666) == -1){
                printf("fail to call temp1()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_humid",0666) == -1){
                printf("fail to call temp3()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_co2",0666)== -1){
                printf("fail to call temp5()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_lpg",0666) == -1){
                printf("fail to call temp7()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_fan",0666) == -1){
                printf("fail to call temp11()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_buzzer",0666) == -1){
                printf("fail to call temp13()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_light",0666) == -1){
                printf("fail to call temp13()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/iden",0666) == -1){
                printf("fail to call temp13()\n");
                exit(1);
         }
 
        if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_temp",0666) == -1){
                printf("fail to call temp1()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_humid",0666) == -1){
                printf("fail to call temp3()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_co2",0666)== -1){
                printf("fail to call temp5()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_lpg",0666) == -1){
                printf("fail to call temp7()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_fan",0666) == -1){
                printf("fail to call temp11()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_buzzer",0666) == -1){
                printf("fail to call temp13()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_light",0666) == -1){
                printf("fail to call temp13()\n");
                exit(1);
         }
        if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_temp",0666) == -1){
                printf("fail to call temp1()\n");
                exit(1);
         }
 
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_humid",0666) == -1){
                printf("fail to call temp3()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_co2",0666)== -1){
                printf("fail to call temp5()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_lpg",0666) == -1){
                printf("fail to call temp7()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_fan",0666) == -1){
                printf("fail to call temp11()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_buzzer",0666) == -1){
                printf("fail to call temp13()\n");
                exit(1);
         }
         if(mkfifo("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_light",0666) == -1){
                printf("fail to call temp13()\n");
                exit(1);
         }
 
 
         if((desTemp = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_temp", O_RDONLY ))< 0){
                printf("fail to call temp2()\n");
                exit(1);
         }
         if((desHumid = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_humid", O_RDONLY)) < 0){
                printf("fail to call temp4()\n");
                exit(1);
         }
         if((desCo2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_co2", O_RDONLY)) < 0){
                printf("fail to call temp6()\n");
                exit(1);
         }
         if((desLpg = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_lpg", O_RDONLY)) < 0){
                printf("fail to call temp8()\n");
                exit(1);
         }
         if((desFan = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_fan", O_RDONLY )) < 0){
                printf("fail to call temp12()\n");
                exit(1);
         }
         if((desBuzzer = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_buzzer", O_RDONLY)) < 0){
                printf("fail to call temp14()\n");
                exit(1);
         }
         if((desLight = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS1/WS1_light", O_RDONLY)) < 0){
                printf("fail to call temp14()\n");
                exit(1);
         }
         if((desIden = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/iden", O_RDONLY)) < 0){
                printf("fail to call temp14()\n");
                exit(1);
         }
 
         if((desTemp2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_temp", O_RDONLY))< 0){
                printf("fail to call temp2()\n");
                exit(1);
         }
         if((desHumid2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_humid", O_RDONLY )) < 0){
                printf("fail to call temp4()\n");
                exit(1);
         }
         if((desCo22 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_co2", O_RDONLY )) < 0){
                printf("fail to call temp6()\n");
                exit(1);
         }
         if((desLpg2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_lpg", O_RDONLY )) < 0){
                printf("fail to call temp8()\n");
                exit(1);
         }
         if((desFan2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_fan", O_RDONLY )) < 0){
                printf("fail to call temp12()\n");
                exit(1);
         }
         if((desBuzzer2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_buzzer", O_RDONLY )) < 0){
                printf("fail to call temp14()\n");
                exit(1);
         }
         if((desLight2 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS2/WS2_light", O_RDONLY )) < 0){
                printf("fail to call temp14()\n");
                exit(1);
         }
 
         if((desTemp3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_temp", O_RDONLY))< 0){
                printf("fail to call temp2()\n");
                exit(1);
         }
         if((desHumid3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_humid", O_RDONLY )) < 0){
                printf("fail to call temp4()\n");
                exit(1);
         }
         if((desCo23 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_co2", O_RDONLY )) < 0){
                printf("fail to call temp6()\n");
                exit(1);
         }
         if((desLpg3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_lpg", O_RDONLY )) < 0){
                printf("fail to call temp8()\n");
                exit(1);
         }
         if((desFan3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_fan", O_RDONLY )) < 0){
                printf("fail to call temp12()\n");
                exit(1);
         }
         if((desBuzzer3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_buzzer", O_RDONLY )) < 0){
                printf("fail to call temp14()\n");
                exit(1);
         }
         if((desLight3 = open("/home/pi/Prototype/Waspmote-Rasp,Lora/Lora/WS3/WS3_light", O_RDONLY )) < 0){
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
        
         if(publishCount != 0) {
                  infinitePublishFlag = false;
         }
 
         signal(SIGINT,sigint);

	
	 IOT_INFO("receive LoRa packet....\n");
        
 
         while((NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc || SUCCESS == rc)
                   && (publishCount > 0 || infinitePublishFlag)) {
 
                  //Max time the yield function will wait for read messages
                  rc = aws_iot_mqtt_yield(&client, 100);
                  if(NETWORK_ATTEMPTING_RECONNECT == rc) {
                           // If the client is attempting to reconnect we will skip the rest of the loop.
                           continue;
                  }
      
 
 /*
         if((readIden = read(desIden, msgIden, 1))) {
                  a = msgIden[0];
                  switch(a){
                  case '1' :
*/
		for( i =0; i<50; i++) {
		 	tempdata[i] = rand() % 30 + 23;
				
			   sprintf(msgTemp,"%s", tempdata[i]);
                                  
                           sendTemp[0]='\0';
                           strcat(sendTemp,front);
                           strcat(sendTemp,msgTemp);
                           strcat(sendTemp,tail);

                           paramsTemp.payloadLen = 17;
                           paramsTemp.payload = (void *) sendTemp;
			   printf("paramstemp : %s\n", paramsTemp.payload);
                           rc = aws_iot_mqtt_publish(&client, "ws1/temp", 8, &paramsTemp);
					
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendTemp);
                          
                  }
                 
		for( i =0; i<50; i++) {
		 	humiddata[i] = rand() % 34 + 30;

			sprintf(msgHumid,"%s", humiddata[i]);

                           sendHumid[0]='\0';
                           strcat(sendHumid,front);
                           strcat(sendHumid,msgHumid);
                           strcat(sendHumid,tail);
                           paramsHumid.payloadLen = 17;
                           paramsHumid.payload = (void *) sendHumid;
			   printf("paramshumid : %s\n", paramsHumid.payload);
                           rc = aws_iot_mqtt_publish(&client, "ws1/humid", 9, &paramsHumid);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendHumid);
                          
                  }
 
		for( i =0; i<50; i++) {
		 	co2data[i] = rand() % 70 + 50;
			sprintf(msgCo2,"%s", co2data[i]);
                           
			   sendCo2[0]='\0';
                           strcat(sendCo2,front);
                           strcat(sendCo2,msgCo2);
                           strcat(sendCo2,tail);
                           paramsCo2.payloadLen = 17;
                           paramsCo2.payload = (void *) sendCo2;
			   printf("paramsco2 : %s\n", paramsCo2.payload);
                           rc = aws_iot_mqtt_publish(&client, "ws1/co2",7, &paramsCo2);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendCo2);

                  }
 
               	for( i =0; i<50; i++) {
		 	lpgdata[i] = rand() % 70 + 50;   
			   sprintf(msgCo2,"%s", lpgdata[i]);
                           sendLpg[0]='\0';
                           strcat(sendLpg,front);
                           strcat(sendLpg,msgLpg);
                           strcat(sendLpg,tail);
                           paramsLpg.payloadLen = 17;
                           paramsLpg.payload = (void *) sendLpg;
			   printf("paramslpg : %s\n", paramsLpg.payload);
                           rc = aws_iot_mqtt_publish(&client, "ws1/lpg", 7, &paramsLpg);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendLpg);
                          
                  }
 
                  if((readFan = read(desFan, msgFan, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);}
		  else{
			
			if (old_fan != msgFan[0]){

                          	sendFan[0]='\0';
                          	strcat(sendFan,front);
                           	strcat(sendFan,msgFan);
                           	strcat(sendFan,tail);
                           	paramsFan.payloadLen = 15;
                           	paramsFan.payload = (void *) sendFan;
                           	rc = aws_iot_mqtt_publish(&client, "onoff/ws1/fan", 13, &paramsFan);
                           	if(publishCount > 0) {
                                	   publishCount--;}
				printf("send fan!\n");
				old_fan = msgFan[0];
			}

			else{
				printf("not send!\n");				
			}

		}
				 
 
                  if((readBuzzer = read(desBuzzer, msgBuzzer, 10) < 0 )){
                           printf("fail to call read()\n");
                           exit(1);}
		  else{
			
			if (old_buzzer != msgBuzzer[0]){

                           sendBuzzer[0]='\0';
                           strcat(sendBuzzer,front);
                           strcat(sendBuzzer,msgBuzzer);
                           strcat(sendBuzzer,tail);
                           paramsBuzzer.payloadLen = 15;
                           paramsBuzzer.payload = (void *) sendBuzzer;

                           rc = aws_iot_mqtt_publish(&client, "onoff/ws1/buzzer", 16, &paramsBuzzer);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send buzzer\n");
			   old_buzzer = msgBuzzer[0];
 			  }
				

			else{
				printf("not send!\n");

			}
		}

                  
		
 
                  if((readLight = read(desLight, msgLight, 10) < 0 )){
                           printf("fail to call read()\n");
                           exit(1);}
		  else{
		
			if (old_light != msgLight[0]){

                           sendLight[0]='\0';
                           strcat(sendLight,front);
                           strcat(sendLight,msgLight);
                           strcat(sendLight,tail);
                           paramsLight.payloadLen = 15;
                           paramsLight.payload = (void *) sendLight;

                           rc = aws_iot_mqtt_publish(&client, "onoff/ws1/light", 15, &paramsLight);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send light\n");
			   old_light = msgLight[0];
                           }
			else{
				printf("not send!\n");
			}
		}
		
 
                 
                sleep(1);
                  printf("temp: %s\n",msgTemp);
                  printf("humid: %s\n",msgHumid);
                  printf("co2: %s\n",msgCo2);
                  printf("lpg: %s\n",msgLpg);
                  printf("fan: %s\n",msgFan);
                  printf("Buzzer: %s\n",msgBuzzer);
                  printf("Light: %s\n",msgLight);
                          
                  break;
 /*
                  case '2' :
                  printf("start!\n");
                  if((readTemp2 = read(desTemp2, msgTemp2, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);
                  }
                  else{
                           sendTemp2[0]='\0';
                           strcat(sendTemp2,front);
                           strcat(sendTemp2,msgTemp2);
                           strcat(sendTemp2,tail);
                           paramsTemp2.payloadLen = 17;
                           paramsTemp2.payload = (void *) sendTemp2;
                           rc = aws_iot_mqtt_publish(&client, "ws2/temp", 8, &paramsTemp2);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendTemp2);
                          
                  }
 
                  if((readHumid2 = read(desHumid2, msgHumid2, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);
                  }
                  else{
                           sendHumid2[0]='\0';
                           strcat(sendHumid2,front);
                           strcat(sendHumid2,msgHumid2);
                           strcat(sendHumid2,tail);
                           paramsHumid2.payloadLen = 17;
                           paramsHumid2.payload = (void *) sendHumid2;
                           rc = aws_iot_mqtt_publish(&client, "ws2/humid", 9, &paramsHumid2);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendHumid2);
                          
                  }
 
                  if((readCo22 = read(desCo22, msgCo22, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);
                  }
                  else{
                           sendCo22[0]='\0';
                           strcat(sendCo22,front);
                           strcat(sendCo22,msgCo22);
                           strcat(sendCo22,tail);
                           paramsCo22.payloadLen = 17;
                           paramsCo22.payload = (void *) sendCo22;
                           rc = aws_iot_mqtt_publish(&client, "ws2/co2", 7, &paramsCo22);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendCo22);
                          
                  }
 
                  if((readLpg2 = read(desLpg2, msgLpg2, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);
                  }
                  else{
                           sendLpg2[0]='\0';
                           strcat(sendLpg2,front);
                           strcat(sendLpg2,msgLpg2);
                           strcat(sendLpg2,tail);
                           paramsLpg2.payloadLen = 17;
                           paramsLpg2.payload = (void *) sendLpg2;
                           rc = aws_iot_mqtt_publish(&client, "ws2/lpg", 7, &paramsLpg2);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendLpg2);
                          
                  }
 
 
                  if((readFan2 = read(desFan2, msgFan2, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);}
		  else{

		 	if (old_fan2 != msgFan2[0]){

                           sendFan2[0]='\0';
                           strcat(sendFan2,front);
                           strcat(sendFan2,msgFan2);
                           strcat(sendFan2,tail);
                           paramsFan2.payloadLen = 15;
                           paramsFan2.payload = (void *) sendFan2;
                           rc = aws_iot_mqtt_publish(&client, "onoff/ws2/fan", 13, &paramsFan2);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send Fan2\n");
			   old_fan2 = msgFan2[0];
			}

			else{
				printf("not send!\n");				
			}

		}
 
                  if((readBuzzer2 = read(desBuzzer2, msgBuzzer2, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);}
		  else{

			if (old_buzzer2 != msgBuzzer2[0]){

                           sendBuzzer2[0]='\0';
                           strcat(sendBuzzer2,front);
                           strcat(sendBuzzer2,msgBuzzer2);
                           strcat(sendBuzzer2,tail);
                           paramsBuzzer2.payloadLen = 15;
                           paramsBuzzer2.payload = (void *) sendBuzzer2;
                           rc = aws_iot_mqtt_publish(&client, "onoff/ws2/buzzer", 16, &paramsBuzzer2);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send Buzzer2\n");
			   old_buzzer2 = msgBuzzer2[0];
 			  }
				

			else{
				printf("not send!\n");
			    }
		}
 
                  if((readLight2 = read(desLight2, msgLight2, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);}
		  else{
		
			if (old_light2 != msgLight2[0]){

                           sendLight2[0]='\0';
                           strcat(sendLight2,front);
                           strcat(sendLight2,msgLight2);
                           strcat(sendLight2,tail);
                           paramsLight2.payloadLen = 15;
                           paramsLight2.payload = (void *) sendLight2;
                           rc = aws_iot_mqtt_publish(&client, "onoff/ws2/light", 15, &paramsLight2);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send Light2\n");
			   old_light2 = msgLight2[0];
                           }
			else{
				printf("not send!\n");
			}
		}
 
                  sleep(1);
                  printf("temp2: %s\n",msgTemp2);
                  printf("humid2: %s\n",msgHumid2);
                  printf("co22: %s\n",msgCo22);
                  printf("lpg2: %s\n",msgLpg2);
                  printf("fan2: %s\n",msgFan2);
                  printf("Buzzer2: %s\n",msgBuzzer2);
                  printf("Light2: %s\n",msgLight2);
                  break;
 
                  case '3' :
                                  
                  if((readTemp3 = read(desTemp3, msgTemp3, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);
                  }
                  else{
                           sendTemp3[0]='\0';
                           strcat(sendTemp3,front);
                           strcat(sendTemp3,msgTemp3);
                           strcat(sendTemp3,tail);
                           paramsTemp3.payloadLen = 17;
                           paramsTemp3.payload = (void *) sendTemp3;
                           rc = aws_iot_mqtt_publish(&client, "ws3/temp", 8, &paramsTemp3);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendTemp3);
                          
                  }
 
 
                  if((readHumid3 = read(desHumid3, msgHumid3, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);
                  }
                  else{
                           sendHumid3[0]='\0';
                           strcat(sendHumid3,front);
                           strcat(sendHumid3,msgHumid3);
                           strcat(sendHumid3,tail);
                           paramsHumid3.payloadLen = 17;
                           paramsHumid3.payload = (void *) sendHumid3;
                           rc = aws_iot_mqtt_publish(&client, "ws3/humid", 9, &paramsHumid3);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendHumid3);
                          
                  }
 
                  if((readCo23 = read(desCo23, msgCo23, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);
                  }
                  else{
                           sendCo23[0]='\0';
                           strcat(sendCo23,front);
                           strcat(sendCo23,msgCo23);
                           strcat(sendCo23,tail);
                           paramsCo23.payloadLen = 17;
                           paramsCo23.payload = (void *) sendCo23;
                           rc = aws_iot_mqtt_publish(&client, "ws3/co2", 7, &paramsCo23);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendCo23);
                          
                  }
 
                  if((readLpg3 = read(desLpg3, msgLpg3, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);
                  }
                  else{
                           sendLpg3[0]='\0';
                           strcat(sendLpg3,front);
                           strcat(sendLpg3,msgLpg3);
                           strcat(sendLpg3,tail);
                           paramsLpg3.payloadLen = 17;
                           paramsLpg3.payload = (void *) sendLpg3;
                           rc = aws_iot_mqtt_publish(&client, "ws3/lpg", 7, &paramsLpg3);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send: %s\n",sendLpg3);
                          
                  }
 
                  if((readFan3 = read(desFan3, msgFan3, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);}
		  else{
			
			if (old_fan3 != msgFan3[0]){

                           sendFan3[0]='\0';
                           strcat(sendFan3,front);
                           strcat(sendFan3,msgFan3);
                           strcat(sendFan3,tail);
                           paramsFan3.payloadLen = 15;
                           paramsFan3.payload = (void *) sendFan3;
                           rc = aws_iot_mqtt_publish(&client, "onoff/ws3/fan", 13, &paramsFan3);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("sendFan3\n");
			   old_fan3 = msgFan3[0];
			}

			else{
				printf("not send!\n");				
			}

                  }
 
                  if((readBuzzer3 = read(desBuzzer3, msgBuzzer3, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);}
		  else{
			
			if (old_buzzer3 != msgBuzzer3[0]){

                           sendBuzzer3[0]='\0';
                           strcat(sendBuzzer3,front);
                           strcat(sendBuzzer3,msgBuzzer3);
                           strcat(sendBuzzer3,tail);
                           paramsBuzzer3.payloadLen = 15;
                           paramsBuzzer3.payload = (void *) sendBuzzer3;
                           rc = aws_iot_mqtt_publish(&client, "onoff/ws3/buzzer", 16, &paramsBuzzer3);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send Buzzer3\n");
			   old_buzzer3 = msgBuzzer3[0];
 			  }
				

			else{
				printf("not send!\n");
			    }
		}
 
 
                  if((readLight3 = read(desLight3, msgLight3, 10)) < 0 ){
                           printf("fail to call read()\n");
                           exit(1);}
		  else{
		
			if (old_light3 != msgLight3[0]){

                           sendLight3[0]='\0';
                           strcat(sendLight3,front);
                           strcat(sendLight3,msgLight3);
                           strcat(sendLight3,tail);
                           paramsLight3.payloadLen = 15;
                           paramsLight3.payload = (void *) sendLight3;
                           rc = aws_iot_mqtt_publish(&client, "onoff/ws3/light", 15, &paramsLight3);
                           if(publishCount > 0) {
                                   publishCount--;}
                           printf("send light!\n");
			   old_light3 = msgLight3[0];
                           }
			else{
				printf("not send!\n");
			}
		}

                 
                sleep(1);
                           printf("temp3: %s\n",msgTemp3);
                           printf("humid3: %s\n",msgHumid3);
                           printf("co23: %s\n",msgCo23);
                           printf("lpg3: %s\n",msgLpg3);
                           printf("fan3: %s\n",msgFan3);
                           printf("Buzzer3: %s\n",msgBuzzer3);
                           printf("Light3: %s\n",msgLight3);
                          
                           break;
*/
         }

         if(SUCCESS != rc) {
                  IOT_ERROR("An error occurred in the loop.\n");
         } else {
                  IOT_INFO("Publish done\n"); 
         }
 
         return rc;
}