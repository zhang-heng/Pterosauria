class Cconnect
{
public :
  NetStruct netBuff;
  Cconnect(int cePin, int csnPin)
  {
    //* MISO -> 12
    //* MOSI -> 11
    //* SCK -> 13

    Mirf.cePin = cePin;
    Mirf.csnPin = csnPin;  
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"1controler");
    Mirf.channel = 1;
    Mirf.payload = sizeof(NetStruct);
    Mirf.config();
    Mirf.setTADDR((byte *)"1aircraft");
  }


  long lasttime = 0; 
  long recvcount = 0;
  long sendcount = 0;
  long maxdelaytime = 0;
  long mindelaytime = 0xff;

  void Handle()
  {       
    sendcount++;  
    unsigned long time = micros();  
    Mirf.send((byte *)&time);  
    while(Mirf.isSending()){
    } 
    while(!Mirf.dataReady()){ 
      if ( ( micros() - time ) > 5000 ) { 
        return;
      }
    }  
    recvcount ++;
    Mirf.getData((byte *) &time);   
    long t = micros() - time;   
    if (t> maxdelaytime)  maxdelaytime = t;
    if (t<mindelaytime)  mindelaytime = t;
  }
};






