
typedef struct NET_STRUCT
{
  int pitch;//left front&back
  int roll;// left left&right
  int elevation; //right front&back
  int direction;//right left&right
}
NetStruct,*pNetStruct;

class Cconnect
{
public :
  NetStruct netbuffer;
  Cconnect(int cePin, int csnPin)
  {
    //* MISO -> 12
    //* MOSI -> 11
    //* SCK -> 13

    Mirf.cePin = cePin;
    Mirf.csnPin = csnPin;  
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"aircraft1");
    Mirf.channel = 10;
    Mirf.payload = 4;//sizeof(NetStruct);
    Mirf.config();
    Mirf.setTADDR((byte *)"controler1");
  }

  long time = 0;
  bool b_WaitRecv =false;
  void Handle()
  {       
    if (!Mirf.isSending() && !Mirf.dataReady() && !b_WaitRecv)
    {
      time = millis();
      Mirf.send((byte *)&time); 
      b_WaitRecv = true; 
      for(int i =0;i<10;i++)
      {
        Serial.print("send... ");
        Serial.print(Mirf.isSending());
        Serial.print(" ");
        Serial.print(Mirf.dataReady());
        Serial.print(" ");
        Serial.print(b_WaitRecv);
        Serial.print("\n");
        delay(1);
      } 
      return;
    }

    if (!Mirf.isSending() && Mirf.dataReady()  && b_WaitRecv)
    {
      Mirf.getData((byte *) &time);
      b_WaitRecv = false; 
      Serial.print("recv... ");
      Serial.print(Mirf.isSending());
      Serial.print(" ");
      Serial.print(Mirf.dataReady());
      Serial.print(" ");
      Serial.print(b_WaitRecv);
      Serial.print("\n");
      Serial.println(millis() - time);
      return;
    }

    if (millis() - time > 1000 && b_WaitRecv) 
    {
      Serial.print("timeout... ");
      Serial.print(Mirf.isSending());
      Serial.print(" ");
      Serial.print(Mirf.dataReady());
      Serial.print(" ");
      Serial.print(b_WaitRecv);
      Serial.print("\n");
      b_WaitRecv = false;
    }
    return;

    long t;
    if(!Mirf.isSending() && Mirf.dataReady())
    {
      Mirf.getData((byte*) & t);
      Mirf.send((byte*) & t);
      Serial.println(t); 
    }
  }
};







