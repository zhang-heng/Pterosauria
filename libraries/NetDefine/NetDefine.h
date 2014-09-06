#define ulong unsigned long

enum NetType
  {
    TYPE_UNKNOW,    //无效

    TYPE_TIME_SYN,  //对时
    TYPE_POWER,     //电量

    TYPE_MOTOR_A,   //电机A速度
    TYPE_MOTOR_B,   //电机B速度
    TYPE_MOTOR_C,   //电机C速度
    TYPE_MOTOR_D,   //电机D速度

    TYPE_PITCH,     //俯仰
    TYPE_PITCH_P,
    TYPE_PITCH_I,
    TYPE_PITCH_D,

    TYPE_ROLL,      //横滚
    TYPE_ROLL_P,
    TYPE_ROLL_I,
    TYPE_ROLL_D,

    TYPE_YAW,       //航向
    TYPE_YAW_P,
    TYPE_YAW_I,
    TYPE_YAW_D,

    TYPE_ELEVATION, //海拔
    TYPE_ELEVATION_P,
    TYPE_ELEVATION_I,
    TYPE_ELEVATION_D,

    TYPE_FLY, //解锁飞行

    TYPE_SELF_STATIONARY, //自稳
    TYPE_LAND,            //降落

    TYPE_INIT_BALANCE,    //初始化平衡

    TYPE_READ_CONFIG,     //读取配置
    TYPE_WRITE_CONFIG,     //读取配置

  };

typedef struct NET_STRUCT
{
  ulong seq;
  ulong time;
  ulong type;
  ulong value;
}
  NetStruct,*pNetStruct;
