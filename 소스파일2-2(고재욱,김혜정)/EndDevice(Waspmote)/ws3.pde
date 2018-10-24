 /*
 *  ------ [SX_02a] - TX LoRa -------- 
 *
 *  Explanation: This example shows how to configure the semtech 
 *  module in LoRa mode and then send packets with plain-text payloads
 *
 *  Copyright (C) 2014 Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 *  
 *  Version:           0.1
 *  Design:            David Gascón 
 *  Implementation:    Covadonga Albiñana, Yuri Carmona
 */


// Include this library to transmit with sx1272
#include <WaspSX1272.h>
#include <DHT22.h>     

#define PARA 116.6020682
#define PARB 2.769034857
#define RZERO 76.63
#define RLOAD 10.0
// define the destination address to send packets

void ReadTempHumid();
// status variable
int8_t e;

int   mq2;
int   mq135;
float mq2_r;
float mq135_r;
float PPM135;
float PPM2;
int tmp;
float tmp_f;
int humid;
float humid_f;
char message[15];
DHT22 myDHT22(DIGITAL6);
DHT22_ERROR_t errorCode;

const int WSN=3; // 1,2,3
int mq135_th=1000;
int mq2_th=1000;
float tmp_th=28;
float humid_th=80;
bool auto_fan_flag=0; //0일때 자동 1일때 수동
bool auto_buz_flag=0;
char buf[128];
int fan_flag=0;   //0이면 꺼짐 1이면 켜짐
int buz_flag=0;
char my_packet[100]="";
int counter=0;
int led_flag=0;
int s_num=0;

void setup()
{
  // Init USB port
  //온습도
  pinMode(DIGITAL5, OUTPUT);
  digitalWrite(DIGITAL5, HIGH);
  //Fan
  pinMode(DIGITAL1, OUTPUT);
  digitalWrite(DIGITAL1, LOW);
  //Buzzer
  pinMode(DIGITAL2, OUTPUT);
  digitalWrite(DIGITAL2, LOW);
  //LED
  pinMode(DIGITAL4, OUTPUT);
  digitalWrite(DIGITAL4, LOW);
  USB.ON();

  USB.println(F("----------------------------------------"));
  USB.println(F("Setting configuration:")); 
  USB.println(F("----------------------------------------"));

  // Init sx1272 module
  sx1272.ON();
  PWR.setSensorPower(SENS_5V,SENS_ON);
  // Select frequency channel
  e = sx1272.setChannel(CH_00_900);
  USB.print(F("Setting Channel CH_00_900.\t state ")); 
  USB.println(e);

  // Select implicit (off) or explicit (on) header mode
  e = sx1272.setHeaderON();
  USB.print(F("Setting Header ON.\t\t state "));  
  USB.println(e); 

  // Select mode: from 1 to 10
  e = sx1272.setMode(1);  
  USB.print(F("Setting Mode '1'.\t\t state "));
  USB.println(e);  

  // Select CRC on or off
  e = sx1272.setCRC_ON();
  USB.print(F("Setting CRC ON.\t\t\t state "));
  USB.println(e);  

  // Select output power (Max, High or Low)
  e = sx1272.setPower('M');
  USB.print(F("Setting Power to 'M'.\t\t state "));  
  USB.println(e); 

  // Select the node address value: from 2 to 255
  e = sx1272.setNodeAddress(4); // 2,3,4
  USB.print(F("Setting Node Address to '4'.\t state "));
  USB.println(e);
  USB.println();
}

void loop()
{
  //데이터 받기  
  e = sx1272.receivePacketTimeout(30000);

  for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
  {
    my_packet[i] = (char)sx1272.packet_received.data[i];
  }
  counter=0;
  sx1272.packet_received.length=0;

  USB.print("packet\n");
  USB.print(my_packet[0]);
  USB.print(my_packet[1]);
  USB.print(my_packet[2]);
  USB.print(my_packet[3]);

  //받아온 데이터를 기반으로 설비 제어 및 센서 임계값 설정
  if(e<10)
  {
      if (my_packet[0]=='6') { // 4,5,6
        USB.println("");
        USB.println("packet '6' Received");

        //센서값 받아오기  
  ReadTempHumid();
  mq135 = analogRead(ANALOG4);       
  mq2 = analogRead(ANALOG3);
  
  //센서값 가공
  mq2 = (511./(float)mq2) * 5. - 1.* RLOAD;
  mq135 = (511/(float)mq135) * 5. - 1.* RLOAD;
  mq2_r=mq2;
  mq135_r=mq135;
  PPM2 = PARA * pow((mq2_r/RZERO), -PARB);
  PPM135 = PARA * pow((mq135_r/RZERO), -PARB);
  humid=humid/320;
  humid=humid*10+(int)humid_f%10;
  tmp=tmp*10+(int)tmp_f%10;
  if(tmp>1500)
  tmp/=10;
  if(humid<200)
  humid*=4;

  // co2,lpg값 최대,최소 조정
  if (PPM135 < 0.0) PPM135 = 1.0;
  if (PPM135 > 9500.0) PPM135 = 9951.0;
  if (PPM2 < 0.0) PPM2 = 1.0;
  if (PPM2 > 9500.0) PPM2 = 9951.0;

  //측정한 센서값 출력
  USB.print("CO2 : "); 
  USB.print(PPM135); 
  USB.println("ppm");
  USB.print("LPG : ");
  USB.print(PPM2);
  USB.println("ppm");
  USB.printf("Temperature :");
  USB.print(tmp/10);
  USB.printf(".");
  USB.println(tmp%10);
  USB.printf("Humidity:");
  USB.print(humid/10);
  USB.printf(".");
  USB.println(humid%10);
  
        
        //buf에 데이터 축적
        sprintf(buf,"%d%03d%03d%04d%04d%1d%1d%1d",WSN,tmp,humid,(int)PPM135,(int)PPM2,fan_flag,buz_flag,led_flag);
        
        e = sx1272.sendPacketTimeout(9, buf); // 7,8,9
        USB.print("Sensor Data Send ");
        USB.println(e);
        USB.println(buf);
        USB.println("-------------");
        my_packet[0] = '\n';
        USB.print(my_packet[0]);
        USB.print(my_packet[1]);
        USB.print(my_packet[2]);
        USB.print(my_packet[3]);
      }
      else if(my_packet[0]=='3') // 1,2,3
    {
        switch(my_packet[1])
        {
          //설비제어
          case 'C':
            {
              switch(my_packet[2]){
                //Fan제어
                case 'F':
                {
                  if(my_packet[3]=='1')
                  {
                    auto_fan_flag =1;
                    fan_flag=0;
                    digitalWrite(DIGITAL1, LOW);
                  }
                  else if(my_packet[3]=='2')
                  {
                    auto_fan_flag=1;
                    fan_flag=1;
                    digitalWrite(DIGITAL1, HIGH);
                  }
                  else
                    auto_fan_flag=0;
                  break;
                }
                //Buzzer제어
                case 'B':
                {
                  if(my_packet[3]=='1')
                  {
                   
                    auto_buz_flag =1;
                    buz_flag=0;
                    digitalWrite(DIGITAL2, LOW);
                  }
                  else if(my_packet[3]=='2')
                  {
                    auto_buz_flag=1;
                    buz_flag=1;
                      digitalWrite(DIGITAL2, HIGH);
                  }
                  else
                    auto_buz_flag=0;                
                  break;
                }
                case 'L':
                {
                  if(my_packet[3]=='1')
                  {
                   
                    led_flag =0;
                    digitalWrite(DIGITAL4, LOW);
                  }
                  else if(my_packet[3]=='2')
                  {
                     led_flag=1;
                     digitalWrite(DIGITAL4, HIGH);
                  }
                  
                  break;
                }
              }
              break;
            }
          //임계값 설정  
          case 'S':
            switch(my_packet[2])
            {
              //온도 임계값 설정
              case 'T':
                if(my_packet[5]=='.')
                  {
                    tmp_th=(my_packet[3]-48)*100+(my_packet[4]-48)*10+(my_packet[6]-48);
                    tmp_th=tmp_th/10;
                  }
                else
                  tmp_th=(my_packet[3]-48)*10+(my_packet[4]-48);  
                break;
              //습도 임계값 설정
              case 'H':
                if(my_packet[5]=='.')
                  {
                    humid_th=(my_packet[3]-48)*100+(my_packet[4]-48)*10+(my_packet[6]-48);
                    humid_th=humid_th/10;
                  }
                else
                  humid_th=(my_packet[3]-48)*10+(my_packet[4]-48);
                break;
              //CO2 임계값 설정
              case 'C':
                if(my_packet[6]>0)
                  mq135_th = (my_packet[3]-48)*100+(my_packet[4]-48)*10+(my_packet[5]-48);
                break;
            }
            break;
        }
        my_packet[0] = '\n';
      }
  }

  //받아온 패킷값 출력
  USB.printf("\nLast Receive Packet : ");
  USB.print(my_packet);
  sx1272.showReceivedPacket();
  USB.printf("\n-------------");

  //임계값 출력
  USB.printf("\nCO2 Setting Value : ");
  USB.print(mq135_th);
  USB.printf("\nTemp Setting Value : ");
  USB.print(tmp_th);
  USB.printf("\n Humid Setting Value : ");
  USB.print(humid_th);
  USB.printf("\n-------------\n");

  //Fan 자동제어
  if(auto_fan_flag==0&&(tmp>tmp_th*10||humid>humid_th*10||PPM135>mq135_th)) 
  {
    fan_flag=1;
    digitalWrite(DIGITAL1, HIGH);
  }
  else if(auto_fan_flag==0&&(tmp<tmp_th*10&&humid<humid_th*10&&PPM135<mq135_th))
  {
    fan_flag=0;
    digitalWrite(DIGITAL1, LOW);
  }
  
  //Buzzer 자동 제어
  if(auto_buz_flag==0&&PPM2>mq2_th)
  {
    buz_flag=1;
    digitalWrite(DIGITAL2, HIGH);
  }
  else if(auto_buz_flag==0&&PPM2<mq2_th) 
  {
    buz_flag=0;
    digitalWrite(DIGITAL2, LOW);
  }
 }

//게이트웨이에서 받은 데이터로 설비제어및 임계값 설정 


//온/습도 측정
void ReadTempHumid()
{
  delay(2000);
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
    // Alternately, with integer formatting which is clumsier but more compact to store and
    // can be compared reliably for equality:
    //    
      tmp=myDHT22.getTemperatureC()/32;
      tmp_f=abs(myDHT22.getTemperatureF());
      humid = myDHT22.getHumidityInt();
      humid_f = abs(myDHT22.getHumidity());
      break;
    case DHT_ERROR_CHECKSUM:
      USB.print("check sum error ");
      USB.print(myDHT22.getTemperatureC());
      USB.print("C ");
      USB.print(myDHT22.getHumidity());
      USB.println("%");
      break;
    case DHT_BUS_HUNG:
      USB.println("BUS Hung ");
      break;
    case DHT_ERROR_NOT_PRESENT:
      USB.println("Not Present ");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      USB.println("ACK time out ");
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      USB.println("Sync Timeout ");
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      USB.println("Data Timeout "); 
      break;
    case DHT_ERROR_TOOQUICK:
      USB.println("Polled to quick ");
      break;
  }
}
