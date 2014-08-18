
typedef struct NET_STRUCT
{
  int pitch;//left front&back
  int roll;// left left&right
  int elevation; //right front&back
  int direction;//right left&right
}NetStruct,*pNetStruct;

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
    Mirf.setRADDR((byte *)"controler1");
    Mirf.channel = 10;
    Mirf.payload = sizeof(NetStruct);
    Mirf.config();
    Mirf.setTADDR((byte *)"aircraft1");
  }

  void Handle()
  {
    Mirf.send((byte *)&netBuff);
    if(Mirf.isSending() || !Mirf.dataReady()) return;
    Mirf.getData((byte *) &netBuff);
  }
};












