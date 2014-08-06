typedef struct PID
{
  int SetPoint; //设定目标 Desired Value
  long SumError; //误差累计
  double Proportion; //比例常数 Proportional Const
  double Integral; //积分常数 Integral Const
  double Derivative; //微分常数 Derivative Const
  int LastError; //Error[-1]
  int PrevError; //Error[-2]
} 
PID;

static PID sPID;
static PID *sptr = &sPID;

void IncPIDInit(double p, double i, double d, int point)
{
  sptr->SumError = 0;
  sptr->LastError = 0; //Error[-1]
  sptr->PrevError = 0; //Error[-2]
  sptr->Proportion = p; //比例常数 Proportional Const
  sptr->Integral = i; //积分常数Integral Const
  sptr->Derivative = d; //微分常数 Derivative Const
  sptr->SetPoint = point;
}

int IncPIDCalc(int NextPoint)
{
  register int iError, iIncpid; //当前误差
  iError = sptr->SetPoint - NextPoint; //增量计算
  iIncpid = sptr->Proportion * iError //E[k]项
  - sptr->Integral * sptr->LastError //E[k－1]项
  + sptr->Derivative * sptr->PrevError; //E[k－2]项
  //存储误差，用于下次计算
  sptr->PrevError = sptr->LastError;
  sptr->LastError = iError;
  //返回增量值
  return(iIncpid);
}

void SetPID(double p, double i, double d)
{
  sptr->Proportion = p; //比例常数 Proportional Const
  sptr->Integral = i; //积分常数Integral Const
  sptr->Derivative = d; //微分常数 Derivative Const
}


