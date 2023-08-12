#include <servo.h>
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdint.h> // uint8_t 0-255  denenmesi lazım
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void Servo_init();
void Servo_CRR(uint8_t num, uint32_t angle);
void Servo_Sweep_dizi(uint8_t servo_num[] , uint32_t old_angle[] ,uint32_t angle[]);
void Sit(uint8_t count);

void Servo_init()
{

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);



	//HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	//HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

}

void Servo_CRR(uint8_t num, uint32_t angle)
{

	switch (num) {
		case 0:
			htim2.Instance -> CCR1 = angle;
	        break;

	    case 1:
	    	htim2.Instance -> CCR2 = angle;
	        break;

	    case 2:
	    	htim2.Instance -> CCR3 = angle;
	    	break;

	    case 3:
	    	htim2.Instance -> CCR4 = angle;
	    	break;
	    default:
	      // code block
		}

	return;
}

void Servo_Sweep_dizi(uint8_t servo_num[] , uint32_t old_angle[] ,uint32_t angle[])
{
	uint8_t num = 0; // dizi elemanlarını için

	uint8_t flag = 0; // çıkış komutu için (Tüm eski açılar yeni açılara eşitlendimi)

	uint8_t length_of_array = sizeof(servo_num) / sizeof(servo_num[0]); // dizi boyutu için

	while(1) {

		if (old_angle[num] < angle[num])
		{
			old_angle[num]++;
			Servo_CRR(servo_num[num],old_angle[num]);
			old_angle[num]++;
			HAL_Delay(25);
		}
		if (old_angle[num] > angle[num])
		{
			Servo_CRR(servo_num[num],old_angle[num]);
			old_angle[num]--;
			HAL_Delay(25);
		}

		if (old_angle[num] == angle[num])
		{
			Servo_CRR(servo_num[num],old_angle[num]);
			HAL_Delay(25);
			flag++;
		}

		num++;

		if (num >= 6)
		{
			num= 0 ;
			if (flag == 6)
			{
				break;
			}
			flag=0;
		}
	}

	return;
}

void Sit(uint8_t count)
{
    int servo_num[6] = {0, 1, 2, 3, 4, 5};  // hareket etmesi istenen servo numaralarını taşıyan sizidir
    										// burada sadece sag bacak için olanı yazılmıstır.

    int begin_angle[6] = {75, 108, 108, 42, 75, 75}; // yukarıda verilen servo numaralarının sırasıyla istene baslangıc konumunu içeren fonksiyondur
    												 // bu dizi bir fonksiyona girdiğinde değişeceğinden hangi fonksiyondan nasıl çıktıgına dikkat edilmelidir

    int sit_angle[6] = {75, 64, 58, 42, 75, 75}; // sit konumu açılarını sırasıyla barındırır

    int base_angle[6] = {75, 108, 108, 42, 75, 75}; // robotun asli duruş şekli konumu açılarını sırasıyla tutar

    for (uint8_t i = 1; i <= count; i++){   // istenilen miktar kadar eğilip kalkması için girilen count degeri kadar hareket etmesi için yazılmış bir for döngüsü

    	Servo_Sweep_dizi(servo_num , begin_angle , sit_angle); // oynanması istenen servo numaralrını eski açısını ve süpürerek yavaş yavaş geleceği konumu diziler yardımı ile alır
    															// dikkat edilmelidir ki begin angle dizisi fonksiyondan çıktığı zamna sit angle dizisine dönüşür.

    	HAL_Delay(5000); // 5 saniye konumunu koruması için delay verilmiştir

    	Servo_Sweep_dizi(servo_num , begin_angle , base_angle); // sit angle dizisine dönüşmül olan begin angle bu fonksiyona verilir ve base angle olarak karşımıza çıkar.

    }
    return;
}


/*
 * **************************************************************************************************************
 * **************************************************************************************************************
 * **************************************************************************************************************
 * **************************************************************************************************************
 * **************************************************************************************************************
 * **************************************************************************************************************
 * **************************************************************************************************************
 * **************************************************************************************************************
 * **************************************************************************************************************
 */


void Servo_Sweep(int servo_num , int old_angle ,int angle)
{

	int temp =old_angle ;

	for ( old_angle ; old_angle <= angle ;  old_angle++) // old_angle= old_angle + 5
	{
		Servo_CRR(servo_num,old_angle);
		HAL_Delay(40);
	}

	old_angle = temp ;

	for ( old_angle ; old_angle > angle ;  old_angle--)
	{
		Servo_CRR(servo_num,old_angle);
		HAL_Delay(40);

	}



}





void Servo_move(int servo_num , int angle)
{

	if (angle <= 0)
		{
		  angle = 0 ;
		}
	else if (angle>=180)
	{
		angle = 180 ;
	}
	angle = ( angle * 100 / 180 ) +25;

	switch (servo_num) {
		case 0:
			htim2.Instance -> CCR1 = angle;
	        break;

	    case 1:
	    	htim2.Instance -> CCR2 = angle;
	        break;

	    case 2:
	    	htim2.Instance -> CCR3 = angle;
	    	break;

	    case 3:
	    	htim2.Instance -> CCR4 = angle;
	    	break;
	    case 4:
	    	    break;
		}

	/*
	 * int old_angle = 0;
	angle = ( angle * 100 / 180 ) +25;

	if (old_angle != angle)
	 	{
			htim2.Instance -> CCR1 = angle;
		}
	 	old_angle = angle;                     // bu da pek işimize yaramadı

	*/
	// __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,angle);   bu fonksiyonla sanki diş atlıyomuş gibi oluyordu

}





void Sit_vol1(int count)
{
    int degree_array[6] = {75, 64, 58, 42, 75, 75};

    int degree_base[6] = {75, 108, 108, 42, 75, 75};

    int old_position[6] = {75, 108, 108, 42, 75, 75};

	for (int i = 1; i <= count; i++)
	{

		  for (int i = 0; i < 5 ; i++)
		  {
			Servo_Sweep(i, old_position[i], degree_array[i]);
		    old_position[i] = degree_array[i];
		  }

		  HAL_Delay(500);

		  for (int i = 0; i < 5 ; i++)
		  {
		 	Servo_Sweep(i, old_position[i], degree_base[i]);
		 		    old_position[i] = degree_base[i];
		  }

	}

}





void Sit_vol2(int count)
{
	htim2.Instance -> CCR1 = 75;   // tim2 - chanel1 RL1 PA0
	//HAL_Delay(100);
	htim2.Instance -> CCR4 = 42;  // tim2 - chanel3 RL4 PA3
	//HAL_Delay(100);

	  for (int i = 1; i <= count; i++) {

		  	HAL_Delay(1000);

			htim2.Instance -> CCR3 = 108; // tim2 - chanel3 RL3 PA2
			//HAL_Delay(100);

			htim2.Instance -> CCR2 = 108;  // tim2 - chanel2 RL2 PA1
			//HAL_Delay(100)


			HAL_Delay(1000);


			htim2.Instance -> CCR3 = 58; // tim2 - chanel3 RL3 PA2
			//HAL_Delay(100);

			htim2.Instance -> CCR2 = 64;  // tim2 - chanel2 RL2 PA1
			//HAL_Delay(100);

	  }


}

void Sit_vol3(int count)
{
	htim2.Instance -> CCR1 = 25;   // tim2 - chanel1 RL1 PA0
	//HAL_Delay(100);
	htim2.Instance -> CCR4 = 42;  // tim2 - chanel3 RL4 PA3
	//HAL_Delay(100);

	  for (int i = 1; i <= count; i++) {

		  	HAL_Delay(1000);

			Servo_Sweep(2, 108, 53);

			Servo_Sweep(1, 108, 64);

			HAL_Delay(1000);

			Servo_Sweep(2, 53, 108);

			Servo_Sweep(1, 64, 108);

	  }


}

void Servo_reset(void)
{

    int degree_base[6] = {75, 108, 108, 42, 75, 75};

		  for (int i = 0; i < 5 ; i++)
		  {
			  Servo_CRR(i,degree_base[i]);
			  HAL_Delay(100);
		  }

}

void Servo_reset_vol2(void)
{
	htim2.Instance -> CCR1 = 75;   // tim2 - chanel1 RL1 PA0
	HAL_Delay(100);


	htim2.Instance -> CCR2 = 108;  // tim2 - chanel2 RL2 PA1
	HAL_Delay(100);


	htim2.Instance -> CCR3 = 108; // tim2 - chanel3 RL3 PA2
	HAL_Delay(100);


	htim2.Instance -> CCR4 = 42;  // tim2 - chanel4 RL4 PA3
	HAL_Delay(100);
}


