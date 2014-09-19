class Cpid
{
 private:
  double SetPoint;   //期望角
  double SumError;   //误差累计
  double LastError;  //Error[-1]
  double PrevError;  //Error[-2]
  double Proportion; //比例常数 Proportional Const
  double Integral;   //积分常数 Integral Const
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

  double PIDCalc(double NextPoint){
    double  dError, Error;
    Error = SetPoint - NextPoint;          // 偏差
    SumError += Error;                      // 积分
    dError = Error - LastError;             // 当前微分
    PrevError = LastError;
    LastError = Error;
    if(SumError>1000) SumError = 1000;
    if(SumError<-1000) SumError = -1000;
    Serial.print(SumError);
    return (Proportion * Error               // 比例项
            + Integral * SumError * 0.1      // 积分项
            + Derivative * dError* 10);      // 微分项
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
