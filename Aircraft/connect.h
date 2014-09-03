class Cconnect
{
 public :
  NetStruct netbuffer;
  Cconnect(int cePin, int csnPin){
    //* MISO -> 12
    //* MOSI -> 11
    //* SCK -> 13

    Serial.print("Initializing nRF2401l");
    Mirf.cePin = cePin;
    Mirf.csnPin = csnPin;
    Mirf.spi = &MirfHardwareSpi;
    Serial.print(".");
    Mirf.init();
    Serial.print(".");
    Mirf.setRADDR((byte *)"1aircraft");
    Mirf.channel = 1;
    Mirf.payload = sizeof(NetStruct);
    Mirf.config();
    Serial.print(".");
    Mirf.setTADDR((byte *)"1controler");
    Serial.print("finish! \n");
  }

  void Read(NetStruct *pmsg){
    if(!Mirf.isSending() && Mirf.dataReady()){
      Mirf.getData((byte*)pmsg);
    }
    else{
      memset(pmsg,0,sizeof(NetStruct));
      pmsg->type = TYPE_UNKNOW;
    }
  }

  void Send(NetStruct *pmsg){
    Mirf.send((byte*) pmsg);
  }
};
