class Cpid
{
 private:
  double SetPoint; //设定目标 Desired Value
  double SumError; //误差累计
  double LastError; //Error[-1]
  double PrevError; //Error[-2]
  double Proportion; //比例常数 Proportional Const
  double Integral; //积分常数 Integral Const
  double Derivative; //微分常数 Derivative Const
 public :
  Cpid(double p, double i, double d, double point){
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

  double IncPIDCalc(double NextPoint){
    register double iError, iIncpid; //当前误差
    iError = SetPoint - NextPoint; //增量计算
    //E[k]项 - E[k－1]项 + E[k－2]项
    iIncpid = Proportion * iError - Integral * LastError + Derivative * PrevError;
    //存储误差，用于下次计算
    PrevError = LastError;
    LastError = iError;
    //返回增量值
    return (iIncpid);
  }
  void ReSetPID(double p, double i, double d){
    Proportion = p;
    Integral = i;
    Derivative = d;
  }

  void ReSetPoint(double p){
    SetPoint = p;
  }
};
