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
  }

  bool AddHandle(unsigned long delayTime, void (*func) (void*))
  {
    for (int i =0; i<8; i++)
    {
      if (HandleTime[i] != 0)
      {
        HandleFun [i] = func;
        HandleTime [i] = millis() + delayTime;
      }
    }
    return false;
  }

  bool Handle()
  {
    unsigned long nowTime = millis();
    for (int i =0; i<8; i++)
    {
      if (HandleTime[i] && nowTime>= HandleTime[i])
      {
        HandleFun [i](pUser);
        HandleFun [i] = NULL;
        HandleTime[i] = 0;
      }
    }
  }
};













