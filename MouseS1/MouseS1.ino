#include <SPI.h>
#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#define Trigger    4
//pwm_define
// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     1000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN         16  

void ledcAnalogWrite(uint8_t channel, uint32_t duty) 
{
  //pwm control 
  // calculate duty, 8191 from 2 ^ 13 - 1
  //uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

float Air_data[12];

//ad define
#define ad_start 22
#define ad_drdy 17

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define s8 int8_t
#define s16 int16_t
#define s32 int32_t
static const int spiClk = 1000000; // 4 MHz
SPIClass * vspi = NULL;
SPIClass * hspi = NULL;
//command
#define NOP 0x00
#define WAKEUP 0x02
#define POWERDOWN 0x04
#define RESET 0x06
#define START 0x08
#define STOP 0x0a
#define RDATA 0x12
#define RREG(i) (0x20 | i)
#define WREG(i) (0x40 | i)

//address
#define ID 0x00
#define STATUS 0x01
#define INPMUX 0x02
#define PGA 0x03
#define DATARATE 0x04
#define REF 0x05
#define IDACMAG 0x06
#define IDACMUX 0x07
#define VBIAS 0x08
#define SYS 0x09
#define OFCAL0 0x0b
#define OFCAL1 0x0c
#define FSCAL0 0x0e
#define FSCAL1 0x0f

//reg
#define PGA_(i) (1<<3 | i)
    //gain list: 0-1, 1-2, 2-4, 3-8, 4-16, 5-32, 6-64, 7-128
#define MUX_(i) (i<<4 | (0xc)) 
    //[7:4]-i for single-end port to positive 
    //[3:0]-1100 for negative port connect to AINCOM
#define DATARATE_ (0x3e) 
    //8'b00111110; [6]-0 for internal clock, 
    //[5]-1 for single-shot mode,
    //[4]-1 reserved
    //[3:0]-1110 for 4000SPS
#define REF_ (0x0a)
    //[5:4]-00 for disable reference buffer bypass
    //[3:2]-10 for internal 2.5v reference
    //[1:0]-10 for internal always on
#define IDACMAG_(i) (i)
#define IDACMUX_(i,j) (i<<4 | j)
#define VBIAS_(i) (i<<6)//connect vbias to AINCOM
#define SYS_(i) (i<<5 || 1<<4) //[7:5]-i for system monitor,
    //default 8 samples, default disable timeout
#define MON_DISABLE 0
#define MON_VBIAS 1
#define MON_TEMP 2
#define MON_AVDDtoAVSSdiv4 3
#define MON_DVDDdiv4 4
#define MON_p2uA 5
#define MON_1uA 6
#define MON_10uA 7
//rest regs are useless or reserved
u8 volt=53;
/**************da define***************/
//da pin define     (num=GPIO_num)
#define HSPI_MOSI       13
#define HSPI_SCLK       14
//#define HSPI_SS_DAg     12
#define HSPI_CS     2  

void wait()
{
    while (digitalRead(ad_drdy));
}

void write(u8 cmd)
{
    vspi->transfer(cmd);
}

u8 read(void)
{
    return vspi->transfer(0x0);
}
u8 read_reg(u8 address)//read 1 register for once
{
  //wait();
  write(1<<5 | address);
  write(0x00);
  return read();
}

void write_reg(u8 address, u8 reg)
{
//  wait();
  write(1<<6 | address);
  write(0x00);
  write(reg);
}

void set_pga(u8 g)
{
  //wait();
  write_reg(PGA,PGA_(g));
}

void set_mux(u8 i)
{
  write_reg(INPMUX,MUX_(i));
}

s16 read_data(void)
{
    s16 a;
  digitalWrite(ad_start, 1);
  wait();
  write(RDATA);
  a= (read()<<8);
  a|=read();
  digitalWrite(ad_start, 0);
  return a;
}
/**************da function***************/
u16 write_dag(u16 da_data)
{
  digitalWrite(HSPI_CS, LOW);
  hspi->transfer16(da_data);
  digitalWrite(HSPI_CS, HIGH);
  //Serial.println("dag transfer done");
}
void da_init(void)
{
  hspi = new SPIClass(HSPI);
  hspi->begin();
  pinMode(HSPI_CS, OUTPUT);
  //pinMode(HSPI_SS_DAd, OUTPUT);
  digitalWrite(HSPI_CS, HIGH);
//  digitalWrite(HSPI_SS_DAd, HIGH);
 // digitalWrite(HSPI_CS, LOW);
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
}

float cal_res(u8 gain, s32 div_res,s16 data)
{
    //s32 temp= div_res*data;
  return  ((float)(1<<(15+gain)) / (float)data * (float)(div_res) -div_res) + 200000 ;
}

s32 dat[12];
int gain[12];
int flag = 0;
void sweep_read()
{
  s16 temp;
  s8 i,j;
  for(i=0;i<12;i++)
  {
    set_mux(i);
    for(j=7;j>=0;j--)
    {
      set_pga(j);
      temp = read_data();
      if (temp <= 32764 && temp >= -32764)
      {
                dat[i]=temp;
                gain[i]=j;
                Air_data[i] = -cal_res(j, 100000, temp);
        break;
      }             
            dat[i]=temp;
            gain[i]=j;
            Air_data[i] = -cal_res(j, 100000, temp); 
    }
  }
}
boolean state = HIGH;
boolean isEthanol = LOW;
int receivedData[3];
int recData;
int SendCurrent = 10;
int ResHigh = 30;
int ResLow = 0;
int ledFlag = 0;
//上升沿触发外部中断
void irq1()
{
    
//     if(isEthanol)
//     {
//       digitalWrite(32,state);
//       state = !state;
//          write_dag(65535*SendCurrent/100);
//          delayMicroseconds(600);
//           write_dag(0);
//           isEthanol = LOW;
//      }
//       else
//      {
//          write_dag(0);
//     }
      
}
esp_timer_handle_t  periodic_timer;
static void periodic_timer_callback(void* arg)
{
//    int64_t time_since_boot = esp_timer_get_time();
//    digitalWrite(32,state);
//    state = !state;
      write_dag(65535*SendCurrent/100);
      delayMicroseconds(600);
      write_dag(0);
}

void ad_init(void)
{
  pinMode(ad_start, OUTPUT);
  pinMode(ad_drdy, INPUT);
  digitalWrite(ad_start, 0);
  vspi = new SPIClass(VSPI);
  vspi->begin();
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE1));
  write(RESET);
    delay(2);
//    wait();
  write_reg(REF,REF_);
  //  write_reg(VBIAS,VBIAS_(0));
  write_reg(DATARATE,DATARATE_);
}

void setup() {
  Serial.begin(115200);
  
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
  
  ad_init();
  da_init();
  write_dag(0);

  const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback  };
  ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 7692));//1s回调一次
  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(Trigger, INPUT);
  attachInterrupt(Trigger, irq1, RISING);    // 设置外部中断
  digitalWrite(33,1);
  digitalWrite(32,0);
  ledcAnalogWrite(LEDC_CHANNEL_0, 8191);
}

void loop() {
  delay(5000);
  while(1){
   
        sweep_read();
        
        float ftemp1[12];
        ftemp1[0] = Air_data[0];  

        Serial.print(ftemp1[0]);
        Serial.print("\r\n");
        if (Serial.available() >= 3 * sizeof(int))
        {
           byte buffer[sizeof(int)];
         for (int i = 0; i < 3; ++i)
         {
           Serial.readBytes(buffer, sizeof(int));  // 从串口读取字节
           receivedData[i] = *(int*)buffer;  // 将字节转换为整数类型
           Serial.print(receivedData[i]);;
           Serial.print("\r\n");
         }
          if(receivedData[0] == 0) //设置电流大小
          {
            SendCurrent = receivedData[1];
          }
          else if(receivedData[0] == 1) //设置加热电压
          {
           
            ledcAnalogWrite(LEDC_CHANNEL_0, 8191/5*(receivedData[1]));
          }
          else if(receivedData[0] == 2) //设置检测酒精阈值
          {
            ResHigh = receivedData[1];
            ResLow = receivedData[2];
          }
        }
     if((ftemp1[0]/1000)>=ResLow && (ftemp1[0]/1000) <= ResHigh)
     {
         isEthanol = HIGH;
      }
      else
      {
        isEthanol = LOW;
      }
        delay(500); 
//        if(ledFlag)  
//        {
//          digitalWrite(33,1);
//          ledFlag = 0;
//        }
//        else
//        {
//          digitalWrite(33,0);
//          ledFlag = 1;
//        }
          
        } 

}
