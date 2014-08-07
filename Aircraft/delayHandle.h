//延迟处理封装，用于在单线程轮询操作下非阻塞delay
class CdelayHandle
{
  private:
    unsigned long HandleTime[8];
    void(*HandleFun[8])(void*);
    void* pUser;
  public:
    CdelayHandle(void* p)
    {
      pUser = p;
      for (int i = 0; i < 8; i++)
      {
        HandleFun[i] = 0;
        HandleTime[i] = 0;
      }
    }

    bool AddHandle(unsigned long delayTime, void (*func) (void*))
    {
      for (int i = 0; i < 8; i++)
      {
        if (!HandleTime[i])
        {
          HandleFun [i] = func;
          HandleTime [i] = millis() + delayTime;
          Serial.print("now:");
          Serial.print( millis());
          Serial.print("\t");
          Serial.print(HandleTime [i]);
          Serial.print("..add Handle\n");
          return true;
        }
      }
      return false;
    }

    bool Handle()
    {
      unsigned long nowTime = millis();
      for (int i = 0; i < 8; i++)
      {
        if (HandleTime[i] && nowTime >= HandleTime[i])
        {
          HandleFun [i](pUser);
          HandleFun [i] = NULL;
          HandleTime[i] = 0;
        }
      }
    }
};
