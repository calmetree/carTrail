/*********************************************************************
         作者：神秘藏宝室
         店铺：ILoveMCU.taobao.com

    本例程仅适用于在本店购买模块的用户使用，鄙视其它店铺的盗用行为
    版权所有，盗版必究！！！
    A6模块链接：
    A6 GSM/GPRS模块：  
    https://item.taobao.com/item.htm?id=542988971078
    A7 GSM/GPRS/GPS 三合一模块：
    https://item.taobao.com/item.htm?id=542988971078
    ARDUINO UNO R3开发板：
    https://item.taobao.com/item.htm?id=27554596680
    ARDUINO MEGA2560 R3开发板：
    https://item.taobao.com/item.htm?id=38041409136
    USB-TTL模块：
    https://item.taobao.com/item.htm?id=39481188174
    移动场合供电用锂电池套装：
    https://item.taobao.com/item.htm?id=530904849115
*********************************************************************/

#include <TimerOne.h>
#include <SoftwareSerial.h>


#define GprsSerail  Serial
SoftwareSerial DebugSerial(10, 11); // RX, TX           

// #define DebugSerial Serial
// #define GprsSerail Serial3   //使用mega2560板子可以用Serial3，不用SoftSerial调试，这样更稳定


#define Success 1U
#define Failure 0U

int L = 13; //LED指示灯引脚

unsigned long  Time_Cont = 0;       //定时器计数器

const unsigned int gprsRxBufferLength = 200;
char gprsRxBuffer[gprsRxBufferLength],gpsInfo[gprsRxBufferLength];//,gprsRxBuffer1[gprsRxBufferLength];
unsigned int gprsBufferCount = 0,gpsInfoLen=0;


char resetmsg[] = "reset the board";    //短信内容  
char startmsg[] = "start the board";
char Eof_buf[2]={0x1a,'\0'};
int transInfoFlag=0;
int gpsUpdateCount=0;
int firstTime=0;
void setup() {
    pinMode(L, OUTPUT);
    digitalWrite(L, LOW);

    DebugSerial.begin(9600);
    GprsSerail.begin(115200);

    Timer1.initialize(1000);
    Timer1.attachInterrupt(Timer1_handler);

  
     initGprsAndGps();      //初始化模块
     sendShortMsg(startmsg);
     firstTime=0;
   //sendMessage("13564001162","test");
    //phone(phoneNumber);       //打电话

    DebugSerial.println("\r\nsetup end!");
}

unsigned long Time_out=30*1000;//30*1s
unsigned char wFlag=0;
unsigned int FailTimes=0;
unsigned int initTimes=0;
void loop() {
  unsigned int x;
  int i;
  //if(Time_Cont%3000 == 0) //&& (wFlag  ==0) )
  mydelays(15);
  if(transInfoFlag==0)
  {
    DebugSerial.println(Time_Cont);
    DebugSerial.println("1101 \n");
    clrGprsRxBuffer();
    gprsReadBuffer();
    i=0;
    DebugSerial.println("11010 \n");
    if((gprsBufferCount>=gpsInfoLen)&&(gprsBufferCount>0))
    {
       gpsInfoLen = gprsBufferCount;
      while(i<gprsBufferCount)
      {
          gpsInfo[i]=gprsRxBuffer[i];
          i++;
          //DebugSerial.print(gpsInfo[i++]);
      }
      gpsUpdateCount++;
      firstTime++;
    }
    DebugSerial.println("11011 \n");
    
    //Time_Cont = 0;
    
  }
  //if( (Time_Cont%Time_out == 0 ) && wFlag==0 )
  //mydelays(10);
    /*
  for(i=0;i<30;i++)
  {
    delay(1000);
    DebugSerial.print("11101 ==");
    DebugSerial.println(i);
    //DebugSerial.println(gpsInfoLen);
    
  }
  */
  mydelays(5);
  if(transInfoFlag==0)
  {
    wFlag=1;
    Time_Cont =0;
    DebugSerial.println("1102 \n");
    //sendShortMsg();
  }
  
  if((wFlag ==1)&&(transInfoFlag==0)&&(gpsInfoLen>20)&&(gpsUpdateCount>=2)&&(firstTime>=4))
  {
      gpsUpdateCount = 0;
      firstTime =4;
      DebugSerial.println("1103 \n");
      //clrGprsRxBuffer();//??need
      
      //if(gpsInfoLen>20)
      {
            
            //strcpy(gpsInfo, gprsRxBuffer);
            i=0;
            DebugSerial.print("201:");
            DebugSerial.println(gpsInfoLen);
            
            /*
            while(i<gpsInfoLen)
            {
                //gpsInfo[i]=gprsRxBuffer[i];
                DebugSerial.print(gpsInfo[i++]);
            }
            */
            //gpsInfoLen = gprsBufferCount;
            //clrGprsRxBuffer();
            //DebugSerial.print("\n");
            
            if(SendGpsToSrv()== Success )
            {
                 FailTimes = 0;
                 initTimes = 0;
                 gpsInfoLen=0;
            }
            else
            {
              FailTimes ++;
            }
       }

       if(FailTimes>10)
       {
          initGprsAndGps();
          initTimes++;
       }
       if(initTimes>10)
       {
          //读短信
          //AT+CMGF=1 ->OK
          //AT+CSDH=1 ->OK
          //AT+CNMI=0,2,0,0,0 OK
          
          initGprsAndGps();
          sendShortMsg(resetmsg);
          //if (sendCommand("AT+CMGS=\"13564001162\"\r\n", "OK", 3000, 10) == Success);
          //else errorLog(2);
          //ascsii:短信内容+hex:1A
          //get:+CMGS: 0
          //get:OK
        }
       wFlag=0;
   }
  

}


void sendShortMsg(char *msg)
{         //发短信
          sendCommand("AT+CMGF=1\r\n", "OK", 3000, 10);
          delay(10);//10ms
          sendCommand("AT+CSCS=\"GSM\"\r\n", "OK", 3000, 10);
          delay(10);//10ms

          sendCommand("AT+CMGS=\"13564001162\"\r\n", ">", 3000, 10);
          delay(10);//10ms
          //gprsReadBuffer();
          ///GprsSerail.read();
          sendCommand(msg, "XX", 0, 1);
          sendCommand(Eof_buf, "x", 0, 1);
          delay(10);
          clrGprsRxBuffer();
          DebugSerial.println(302);
}
unsigned int SendGpsToSrv()
{
    int i;
    transInfoFlag = 1;
    sendCommand("AT+GPSRD=0\r\n", "OK", 1000, 10);//close GPS info output!!!
    mydelays(3);
    if (sendCommand("AT+CIPSTART=\"TCP\",\"218.145.64.217\",19320\r\n", "CONNECT OK",3000, 1) == Success);
    else
    {
        mydelays(2);
        DebugSerial.println(100);
        GprsSerail.write("AT+CIPCLOSE\r\n");
        mydelays(3);
        sendCommand("AT+CIPSTART=\"TCP\",\"218.145.64.217\",19320\r\n", "CONNECT OK",3000, 10);
        //if (sendCommand("AT+CIPSTART=\"TCP\",\"218.145.64.217\",34148\r\n", "CONNECT OK",3000, 10) == Success);
        //else errorLog(3);
    }
    
    mydelays(2);
    //DebugSerial.println(101);
    delay(10);   
    //DebugSerial.println(102);
    
    sendCommand("AT+CIPSEND\r\n", ">", 0, 1);

    mydelays(2);
    
    
    //DebugSerial.println(103);
    //DebugSerial.println(gpsInfoLen);
    
    delay(10);//10ms
    DebugSerial.print("start send GPS info:");
    DebugSerial.print(gpsInfoLen);
    i=0;
    while(i<gpsInfoLen)
    {
        DebugSerial.print(gpsInfo[i]);
        GprsSerail.print(gpsInfo[i++]);
        
        delay(20);//50ms
    }
    //sendCommand(pgpsInfo, "x", 0, 1);
    delay(20);//50ms
    sendCommand(Eof_buf, "x", 0, 1);
    delay(20);//10ms
    DebugSerial.println("\n 104");
    //DebugSerial.println(Time_Cont);

    memset(gpsInfo, 0, gpsInfoLen);      //清空
    gpsInfoLen=0;
    Time_Cont = 0;
    for(i=0;i<10;i++)
    {
      mydelays(2);
      //while (Time_Cont < 3000)
      {
        gprsReadBuffer();
        if (strstr(gprsRxBuffer, "Gotit") != NULL)
        {
          clrGprsRxBuffer();
          GprsSerail.write("AT+CIPCLOSE\r\n");
          transInfoFlag =0;
          return Success;
        }
      }
    }
    clrGprsRxBuffer();
    GprsSerail.write("AT+CIPCLOSE\r\n");
    //clrGprsRxBuffer();
    
    sendCommand("AT+GPSRD=10\r\n", "OK", 1000, 10);//open GPS info output!!!
    transInfoFlag =0;
    return Failure;
}

void initGprsAndGps()
{
    //sendCommand("AT+CFUN=1\r\n", "x", 0, 1);
    //delay(1000*30);//30s
    DebugSerial.println("001");
    //sendCommand("AT+RST=1\r\n", "x", 0, 1);
    //delay(1000*40);//40s
    mydelays(5);
    sendCommand("AT\r\n", "OK", 1000, 10);
    DebugSerial.println("002");
    delay(10);
    if(sendCommandReceive2Keyword("AT+CPIN?\r\n", "READY", "OK\r\n", 1000, 10)==Failure)
    {
        initGprsAndGps();
     }
    

    DebugSerial.println("003");

    delay(10);
    if(sendCommandReceive2Keyword("AT+CREG?\r\n", "CREG: 1", "OK\r\n", 1000, 10)==Failure)
    {
        initGprsAndGps();
     }
    DebugSerial.println("004");
    delay(10);
    if(sendCommand("AT+CGATT=1\r\n","OK", 1000, 10)==Failure)
    {
        initGprsAndGps();
     }
    DebugSerial.println("005");
    delay(10);
    if(sendCommand("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n","OK", 1000, 10)==Failure)
    {
        initGprsAndGps();
     }

    
    DebugSerial.println("006");
    delay(10);

    sendCommand("AT+GPS=1\r\n", "OK", 1000, 10);
    DebugSerial.println("007");
    delay(10);
    sendCommand("AT+GPSRD=10\r\n", "OK", 1000, 10);
    DebugSerial.println("008");
    delay(10);

    DebugSerial.println("\r\n init GPRS and GPS done!!");

    
}
/*
void errorLog(int num)
{
    DebugSerial.print("ERROR");
    DebugSerial.println(num);
    //while (1)
  if(1)
    {
    DebugSerial.print("\r\nRESET!!!!!!\r\n");
        digitalWrite(L, HIGH);
        delay(100);
        digitalWrite(L, LOW);
        delay(100);
        digitalWrite(L, HIGH);
        delay(100);
        digitalWrite(L, LOW);
        delay(300);

        if (sendCommand("AT\r\n", "OK", 100, 10) == Success)
        {
            DebugSerial.print("\r\nRESET!!!!!!\r\n");
            //resetFunc();
        }
    }
}
*/


unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned int Retry)
{
    unsigned int n;
    clrGprsRxBuffer();
  
    for (n = 0; n < Retry; n++)
    {
        DebugSerial.print("\r\n---------send AT Command:---------\r\n");
        DebugSerial.write(Command);
        DebugSerial.println(Retry);

        GprsSerail.write(Command);

        Time_Cont = 0;
        mydelays(2);
        //delay(1000);
        //while (Time_Cont < Timeout)
        {
            gprsReadBuffer();
            if (strstr(gprsRxBuffer, Response) != NULL)
            {
                DebugSerial.print("\r\n==1========receive AT Command:==========\r\n");
                DebugSerial.print(gprsRxBuffer); //输出接收到的信息
                clrGprsRxBuffer();
                return Success;
            }
        }
        Time_Cont = 0;
    }
    DebugSerial.print("\r\n=======2===receive AT Command:==========\r\n");
    DebugSerial.print(gprsRxBuffer);//输出接收到的信息
    clrGprsRxBuffer();
    return Failure;
}

unsigned int sendCommandReceive2Keyword(char *Command, char *Response, char *Response2, unsigned long Timeout, unsigned int Retry)
{
    clrGprsRxBuffer();
    for (unsigned int n = 0; n < Retry; n++)
    {
        DebugSerial.print("\r\n---------send AT Command:---------\r\n");
        DebugSerial.write(Command);

        GprsSerail.write(Command);

        Time_Cont = 0;
        mydelays(2);
        //delay(1000);//
        //while (Time_Cont < Timeout)
        {
            gprsReadBuffer();
            if (strstr(gprsRxBuffer, Response) != NULL && strstr(gprsRxBuffer, Response2) != NULL)
            {
                DebugSerial.print("\r\n===3=======receive AT Command:==========\r\n");
                DebugSerial.print(gprsRxBuffer); //输出接收到的信息
                clrGprsRxBuffer();
                return Success;
            }
        }
        Time_Cont = 0;
    }
    DebugSerial.print("\r\n===4=======receive AT Command:==========\r\n");
    DebugSerial.print(gprsRxBuffer);//输出接收到的信息
    clrGprsRxBuffer();
    return Failure;
}

void mydelays(int s)
{
    int i;
    for(i=0;i<s;i++)
    {
        delay(1000);
    }
}


void Timer1_handler(void)
{
    Time_Cont++;
}



void gprsReadBuffer() 
{
  int tmp=GprsSerail.available();
  int tmp0=gprsBufferCount;
  DebugSerial.println("1111101 \n");
    while (GprsSerail.available()>0)
    {
        gprsRxBuffer[gprsBufferCount++] = GprsSerail.read();
    //DebugSerial.print(gprsRxBuffer[gprsBufferCount-1]);
    //DebugSerial.println(GprsSerail.available());
    
        if (gprsBufferCount == gprsRxBufferLength)clrGprsRxBuffer();
        if((gprsBufferCount-tmp0)>=tmp) break;
    }
    if(gprsBufferCount>60)DebugSerial.print(gprsRxBuffer); //输出接收到的信息
    DebugSerial.println("11111010 \n");
}

void clrGprsRxBuffer(void)
{
    memset(gprsRxBuffer, 0, gprsRxBufferLength);      //清空
    gprsBufferCount = 0;
}
