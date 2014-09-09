
class Cconnect
{
 public :
  NetStruct netBuff;

  ulong m_LastTime;
  ulong m_SuccessCount;
  ulong m_SendCount;
  ulong m_RecvCount;

  Cconnect(int cePin, int csnPin){
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

    m_LastTime = millis();
    m_SendCount = 0;
    m_RecvCount = 0;
  }

  NetStruct sendbuff;
  NetStruct recvbuff;

  //发送操作指令
  bool CommandByType(NetType t){ 
    return  SetValueByType( t,  0);
  }

  //调整参数
  bool SetValueByType(NetType t, float vIn){
    ulong v = 0;
    return  GetValue( t, *(ulong*)&vIn, v);
  }

  //仅获取
  bool GetValueByType(NetType t, float &vOut){
    return  GetValue( t,  0 ,*(ulong*)&vOut);
  }

  //修改并获取
  bool GetValueByType(NetType t, float vIn ,float &vOut){
    return  GetValue( t,  *(ulong*)&vIn ,*(ulong*)&vOut);
  }

  bool GetValue(NetType t, ulong vIn ,ulong &vOut){
    m_SendCount++;
    if (Mirf.dataReady()) Mirf.getData((byte *) &sendbuff);
    sendbuff.seq++;
    sendbuff.time = millis();
    sendbuff.type = t;
    sendbuff.value = vIn;
    Mirf.send((byte *)&sendbuff);
    while(Mirf.isSending()){
    }
    while(!Mirf.dataReady()){
      if ( ( millis() - sendbuff.time ) > 10) {
        return false;
      }
    }
    Mirf.getData((byte *) &recvbuff);
    m_RecvCount++;
    if(recvbuff.seq != sendbuff.seq)
      return false;
    vOut = recvbuff.value;
    return true;
  }

  int GetSuccessCount(){
    if(millis() - m_LastTime > 500){
      m_SuccessCount = m_RecvCount*100/ m_SendCount;
      m_LastTime = millis();
      m_SendCount = 0;
      m_RecvCount = 0;
    }
    return m_SuccessCount;
  }
};
