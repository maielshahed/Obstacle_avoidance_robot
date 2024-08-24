/*
 * automatic_smart_car_barrier_project.c
 *
 * Created: 8/10/2024 2:59:40 PM
 *  Author: Mai El Shahed
 */ 


#include <avr/io.h>
#include "Dio.h"

#include "Std_macros.h"
#include "direction_speed.h"

#define  F_CPU 1000000UL
#include <util/delay.h>



unsigned short a,b,high,distance,right_distance=0,left_distance=0;

int main(void)
{
	
		
		DIO_vsetpindir('D',5,1);//output servo motor
		DIO_vsetpindir('D',7,1);//tr--- PD6 echo
	motor_vinti();
		
    while(1)
    {
        //TODO:: Please write your application code 
			
		
		TCCR1A = 0;
		TIFR = (1<<ICF1);  	/* Clear ICF (Input Capture flag)  */
		DIO_vwrite('D',7,1);
		_delay_us(50);
		DIO_vwrite('D',7,0);
		
		TCCR1B = 0xc1;  	/* Rising edge, no prescaler , noise canceler*/
		while ((TIFR&(1<<ICF1)) == 0);
		a = ICR1;  		/* Take value of capture register */
		TIFR = (1<<ICF1);  	/* Clear ICF flag */
		TCCR1B = 0x81;  	/* Falling edge, no prescaler ,noise canceler*/
		while ((TIFR&(1<<ICF1)) == 0);
		b = ICR1;  		/* Take value of capture register */
		TIFR = (1<<ICF1);  	/* Clear ICF flag */
		TCNT1=0;
		TCCR1B = 0;  		/* Stop the timer */
		high=b-a;
		distance=((high*34600)/(F_CPU*2)) ;
		
		//no object
		SET_BIT(PORTD,5);
		_delay_ms(1.5);
		CLR_BIT(PORTD,5);
		
		while(distance>20){
			motor_forword(); 
		}
stop_motor();
		
		
			//object  servo motor
			SET_BIT(PORTD,5); //right
			_delay_ms(1);
			CLR_BIT(PORTD,5);
			_delay_ms(500);
			right_distance=((high*34600)/(F_CPU*2)) ;
			
			
			SET_BIT(PORTD,5);//left
			_delay_ms(2);
			CLR_BIT(PORTD,5);
			_delay_ms(500);
			left_distance=((high*34600)/(F_CPU*2)) ;
			
	
			if (right_distance>=left_distance)
			{
				turn_right();
				_delay_ms(380);
				stop_motor(); 
			} 
			else
			{
				turn_left();
				_delay_ms(380);
				stop_motor();
			}
			
			
		
		
		
		
    }
}