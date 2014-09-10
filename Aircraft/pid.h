class Cpid
{
 private:
  float SetPoint; //设定目标 Desired Value
  float SumError; //误差累计
  float LastError; //Error[-1]
  float PrevError; //Error[-2]
  float Proportion; //比例常数 Proportional Const
  float Integral; //积分常数 Integral Const
  float Derivative; //微分常数 Derivative Const
 public :
  Cpid(float p, float i, float d, float point){
    SumError = 0;
    LastError = 0;
    PrevError = 0;
    Proportion = p;
    Integral = i;
    Derivative = d;
    SetPoint = point;
  }

  void ReSet(){
    SumError = 0;
    LastError = 0;
    PrevError = 0;
    SetPoint = 0;
  }

  float IncPIDCalc(float NextPoint){
    register float iError, iIncpid; //当前误差
    iError = SetPoint - NextPoint; //增量计算
    //E[k]项 - E[k－1]项 + E[k－2]项
    iIncpid = Proportion * iError - Integral * LastError + Derivative * PrevError;
    //存储误差，用于下次计算
    PrevError = LastError;
    LastError = iError;
    //返回增量值
    return iIncpid;
  }
  void ReSetPID(float p, float i, float d){
    Proportion = p;
    Integral = i;
    Derivative = d;
  }

  void ReSetPoint(float p){
    SetPoint = p;
  }
};
