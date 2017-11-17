#include "main.h"
#include "mpu6050_driver.h"
volatile MPU6050_RAW_DATA    MPU6050_Raw_Data;    //原始数据
volatile MPU6050_REAL_DATA   MPU6050_Real_Data;
AHRS ahrs;
uint8_t mpu_buf[20]={0};       //save the data of acc gyro & mag using iic
extern uint8_t Re_buf[11];
float MPU6050_FIFO[9][11] = {0};//[0]-[9]为最近10次数据 [10]为10次数据的平均值
MagMaxMinData_t MagMaxMinData;

/**************************实现函数********************************************
*函数原型:		int MPU6050_Init(void)
*功　　能:	  MPU6050初始化，成功返回0 
*******************************************************************************/
int MPU6050_Init(void)
{
    USART2_Configuration();  //初始化USART2 
    return 0;
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_DataSave(int16_t ax,int16_t ay,int16_t az,int16_t wax,int16_t way,int16_t waz,int16_t wx,int16_t wy,int16_t wz)
*功　　能:	  MPU6050数据处理，取平均值，将MPU6050_ax,MPU6050_ay, MPU6050_az,MPU6050_wax, MPU6050_way, MPU6050_waz处理后存储
*******************************************************************************/

void MPU6050_a_DataSave(float ax,float ay,float az) //[0]-[9]为最近10次数据 [10]为10次数据的平均值
{
	//printf("a:%f,%f,%f\n",ax,ay,az);
	uint8_t i = 0;
	float sum=0;
	
	for(i=1;i<10;i++)
	{
		MPU6050_FIFO[0][i-1]=MPU6050_FIFO[0][i];
		MPU6050_FIFO[1][i-1]=MPU6050_FIFO[1][i];
		MPU6050_FIFO[2][i-1]=MPU6050_FIFO[2][i];
	}
	
	MPU6050_FIFO[0][9]=ax;//将新的数据放置到 数据的最后面
	MPU6050_FIFO[1][9]=ay;
	MPU6050_FIFO[2][9]=az;
	sum=0;
	for(i=0;i<10;i++)//求当前数组的合，再取平均值
	{	
		 sum+=MPU6050_FIFO[0][i];
	}
	//printf("sum:%f",sum);
	MPU6050_FIFO[0][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[1][i];
	}
	MPU6050_FIFO[1][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[2][i];
	}
	MPU6050_FIFO[2][10]=sum/10;
 //printf("a:%f,%f,%f\n",MPU6050_FIFO[0][10],MPU6050_FIFO[1][10],MPU6050_FIFO[2][10]);

}
void MPU6050_wa_DataSave(float wax,float way,float waz) //[0]-[9]为最近10次数据 [10]为10次数据的平均值
{
	uint8_t i = 0;
	float sum=0;
	
	for(i=1;i<10;i++)
	{
		MPU6050_FIFO[3][i-1]=MPU6050_FIFO[3][i];
		MPU6050_FIFO[4][i-1]=MPU6050_FIFO[4][i];
		MPU6050_FIFO[5][i-1]=MPU6050_FIFO[5][i];
	}
	MPU6050_FIFO[3][9]=wax;
	MPU6050_FIFO[4][9]=way;
	MPU6050_FIFO[5][9]=waz;
	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[3][i];
	}
	MPU6050_FIFO[3][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[4][i];
	}
	MPU6050_FIFO[4][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[5][i];
	}
	MPU6050_FIFO[5][10]=sum/10;
}
void MPU6050_w_DataSave(float wx,float wy,float wz) //[0]-[9]为最近10次数据 [10]为10次数据的平均值
{
	//printf("w\n");
	uint8_t i = 0;
	float sum=0;
	
	for(i=1;i<10;i++)
	{
		MPU6050_FIFO[6][i-1]=MPU6050_FIFO[6][i];
		MPU6050_FIFO[7][i-1]=MPU6050_FIFO[7][i];
		MPU6050_FIFO[8][i-1]=MPU6050_FIFO[8][i];
	}
	MPU6050_FIFO[6][9]=wx;
	MPU6050_FIFO[7][9]=wy;
	MPU6050_FIFO[8][9]=wz;
	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[6][i];
	}
	MPU6050_FIFO[6][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[7][i];
	}
	MPU6050_FIFO[7][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[8][i];
	}
	MPU6050_FIFO[8][10]=sum/10;
}


float MPU6050_Lastax,MPU6050_Lastay,MPU6050_Lastaz
				,MPU6050_Lastwax,MPU6050_Lastway,MPU6050_Lastwaz,
         MPU6050_Lastwx,MPU6050_Lastwy,MPU6050_Lastwz;
extern int MPU6050_Readdone;
/**************************实现函数********************************************
*函数原型:		void MPU6050_getMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* wax, int16_t* way, int16_t* waz,int16_t* wx, int16_t* wy, int16_t* wz) {
*功　　能:	  读取 MPU6050的当前测量值
*描    述：   ax,ay,az:     x,y,z轴方向加速度
							wax,way,waz:	x,y,z轴方向角加速度
							wx,wy,wz:			x,y,z方向角度(0-360)
*******************************************************************************/
void MPU6050_getMotion(float* ax, float* ay, float* az, float* wax, float* way, float* waz,float* wx, float* wy, float* wz) 
	{
		if(MPU6050_Readdone==1)
		{
		if(Re_buf[0]==0x55)  
     {  
			switch(Re_buf [1])
		{
				case 0x51:
		MPU6050_Lastax= ((short)(Re_buf [3]<<8| Re_buf [2]))/32768.0*160;
		MPU6050_Lastay = ((short)(Re_buf [5]<<8| Re_buf [4]))/32768.0*160;
		MPU6050_Lastaz= ((short)(Re_buf [7]<<8| Re_buf [6]))/32768.0*160;
		MPU6050_a_DataSave(MPU6050_Lastax,MPU6050_Lastay,MPU6050_Lastaz);
		break;
		case 0x52:
		MPU6050_Lastwax= ((short)(Re_buf [3]<<8| Re_buf [2]))/32768.0*2000;
		MPU6050_Lastway= ((short)(Re_buf [5]<<8| Re_buf [4]))/32768.0*2000;
		MPU6050_Lastwaz= ((short)(Re_buf [7]<<8| Re_buf [6]))/32768.0*2000;
		MPU6050_wa_DataSave(MPU6050_Lastwax,MPU6050_Lastway,MPU6050_Lastwaz);
		break;
		case 0x53:
		MPU6050_Lastwx= ((short)(Re_buf [3]<<8| Re_buf [2]))/32768.0*180;
		MPU6050_Lastwy= ((short)(Re_buf [5]<<8| Re_buf [4]))/32768.0*180;
		MPU6050_Lastwz= ((short)(Re_buf [7]<<8| Re_buf [6]))/32768.0*180;
		MPU6050_w_DataSave(MPU6050_Lastwx,MPU6050_Lastwy,MPU6050_Lastwz);
		break;
		} 
		*ax = MPU6050_FIFO[0][10];//=MPU6050_FIFO[0][10];
		*ay = MPU6050_FIFO[1][10];//=MPU6050_FIFO[1][10];
		*az = MPU6050_FIFO[2][10];//=MPU6050_FIFO[2][10];
		*wax = MPU6050_FIFO[3][10];//=MPU6050_FIFO[3][10];
		*way = MPU6050_FIFO[4][10];//=MPU6050_FIFO[4][10];
		*waz = MPU6050_FIFO[5][10];//=MPU6050_FIFO[5][10];
		*wx  =MPU6050_FIFO[6][10] ;
		*wy = MPU6050_FIFO[7][10] ;
		*wz = MPU6050_FIFO[8][10] ;
	} 
	else
	{       //读取上一次的值
		*ax = MPU6050_FIFO[0][10];//=MPU6050_FIFO[0][10];
		*ay = MPU6050_FIFO[1][10];//=MPU6050_FIFO[1][10];
		*az = MPU6050_FIFO[2][10];//=MPU6050_FIFO[2][10];
		*wax = MPU6050_FIFO[3][10];//=MPU6050_FIFO[3][10];
		*way = MPU6050_FIFO[4][10];//=MPU6050_FIFO[4][10];
		*waz = MPU6050_FIFO[5][10];//=MPU6050_FIFO[5][10];
		*wx  =MPU6050_FIFO[6][10] ;
		*wy = MPU6050_FIFO[7][10] ;
		*wz = MPU6050_FIFO[8][10] ;
	}
	MPU6050_Readdone=0;
}
}
/**************************实现函数********************************************
*函数原型:		void MPU6050_getMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
*功　　能:	  读取 MPU6050的当前测量值

*******************************************************************************/
void MPU6050_getlastMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* wax, int16_t* way, int16_t* waz,int16_t* wx, int16_t* wy, int16_t* wz)
{
	*ax  =MPU6050_FIFO[0][10];
	*ay  =MPU6050_FIFO[1][10];
	*az = MPU6050_FIFO[2][10];
	*wax  =MPU6050_FIFO[3][10];
	*way = MPU6050_FIFO[4][10];
	*waz = MPU6050_FIFO[5][10];
	*wx  =MPU6050_FIFO[6][10] ;
	*wy = MPU6050_FIFO[7][10] ;
	*wz = MPU6050_FIFO[8][10] ;
}
/**************************实现函数********************************************
*函数原型:		void MPU6050_getSpeed(float* Spx,float* Spy,float* Acc_x,float* Acc_y) 
*功　　能:	  积分计算当前x、y方向上的速度

*******************************************************************************/
void MPU6050_getSpeed(float* Spx,float* Spy,float* Acc_x,float* Acc_y,float* last_Acc_x,float* last_Acc_y)
{
	if(*Acc_x<0.5&&*Acc_x>-0.5)
	{
//		*Spx += (*Acc_x - *last_Acc_x) * 0.003;
//		*Spy += (*Acc_y - *last_Acc_y) * 0.003;
	}
	else
	{
		*Spx += *Acc_x * 0.003;
		*Spy += *Acc_y * 0.003;
	}
}
/**************************实现函数********************************************
*函数原型:		void MPU6050_getDisplacement(float* Disx,float* Disy,float* Spx,float* Spy) 
*功　　能:	  积分计算当前x、y方向上的位移

*******************************************************************************/
void MPU6050_getDisplacement(float* Disx,float* Disy,float* Spx,float* Spy)
{
	*Disx += *Spx * 0.003;
	*Disy += *Spy * 0.003;
}

