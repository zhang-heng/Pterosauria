#define ulong unsigned long 

enum NetType
{
 TYPE_TIME_SYN,  //对时
 TYPE_POWER,     //电量

 TYPE_MOTOR_A,   //电机A速度
 TYPE_MOTOR_B,   //电机B速度
 TYPE_MOTOR_C,   //电机C速度
 TYPE_MOTOR_D,   //电机D速度

 TYPE_PITCH,     //俯仰
 TYPE_ROLL,      //横滚
 TYPE_YAW,       //航向
 TYPE_ELEVATION, //海拔

 TYPE_SELF_STATIONARY //自稳
};

typedef struct NET_STRUCT
{
  ulong seq;
  ulong time;
  ulong type;
  ulong value;  
}
NetStruct,*pNetStruct;
