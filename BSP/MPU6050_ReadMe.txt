MPU接口使用说明V1.0

1.接线：MPU6050采用USART2口与stm32核心板通信，对应
TX-PD5
RX-PD6

2.加入工程：
将压缩包内四个文件添加到BSP文件夹下，用Keil将两个c文件添加到工程中，
并且在main.h里把两个文件的头文件include进去，如下：

#include"usart2.h"
#include"MPU6050_driver.h"

(如果工程中有其他MPU6050的文件，请先把这些文件移出工程，再进行以上步骤)

3.接口解析

int MPU6050_Init(void)
功能：初始化MPU6050，请在main函数中调用
返回值：无

void MPU6050_getlastMotion(float* ax, float* ay, float* az,float* wax, float* way,float* waz,float* wx, float* wy, float* wz)
功能：读取MPU6050当前测量值
返回值：（引用参数返回，类型为float）
          ax,ay,az---x,y,z轴加速度
          wax,way,waz---x,y,z轴角速度
          wa,wy,wz---x,y,z轴角度


void MPU6050_getlastSpeed(float* Spx,float* Spy)
功能：读取当前速度
返回值：（引用参数返回，类型为float）
         Spx---x轴速度
         Spy---y轴速度


void MPU6050_getlastDisplacement(float* Disx,float* Disy)
功能：读取当前速度
返回值：（引用参数返回，类型为float）
         Disx---x轴速度
         Disy---y轴速度
