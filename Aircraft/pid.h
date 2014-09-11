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
    register double thisError, incpid;
    thisError = SetPoint - NextPoint;
    double pError = thisError - LastError;
    double iError = thisError;
    double dError = thisError - 2*LastError + PrevError;
    PrevError = LastError;
    LastError = thisError;
    incpid =Proportion*pError + Integral*iError + Derivative*dError;
    return incpid;

    //    register double iError, iIncpid; //当前误差
    //    iError = SetPoint - NextPoint; //增量计算
    //    //E[k]项 - E[k－1]项 + E[k－2]项
    //    iIncpid = Proportion * iError - Integral * LastError + Derivative * PrevError;
    //    //存储误差，用于下次计算
    //    PrevError = LastError;
    //    LastError = iError;
    //    //返回增量值
    //    return iIncpid;
  }

  void ReSetPID(double p, double i, double d){
    Proportion = p;
    Integral = i;
    Derivative = d;
  }

  void ReSetPoint(double p){
    SetPoint = p;
  }

  //  double pidUpdate(pidsuite* pid, const double measured,double expect,double gyro){
  //    double output;
  //    static double lastoutput=0;
  //    pid->desired=expect;//获取期望角度
  //    pid->error = pid->desired - measured;//偏差：期望-测量值
  //    pid->integ += pid->error * IMU_UPDATE_DT;//偏差积分
  //    //作积分限制
  //    if (pid->integ > pid->iLimit)pid->integ = pid->iLimit;
  //    else if (pid->integ < -pid->iLimit)pid->integ = -pid->iLimit;
  //    // pid->deriv = (pid->error - pid->prevError) / IMU_UPDATE_DT; //微分 应该可用陀螺仪角速度代替
  //    pid->deriv = -gyro;
  //    //pid死区
  //    if(fabs(pid->error)>Piddeadband){
  //      pid->outP = pid->kp * pid->error;//方便独立观察
  //      pid->outI = pid->ki * pid->integ;
  //      pid->outD = pid->kd * pid->deriv;
  //      output = (pid->kp * pid->error) +
  //        (pid->ki * pid->integ) +
  //        (pid->kd * pid->deriv);
  //    }
  //    else{
  //      output=lastoutput;
  //    }
  //    pid->prevError = pid->error;//更新前一次偏差
  //    lastoutput=output;
  //    return output;
  //  }

};
