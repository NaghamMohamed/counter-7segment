/*
 * counter_7segment.c
 *
 * Created: 18-Nov-20 8:21:56 PM
 *  Author: DELL
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd/lcd.h"

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2
#define LCD_PRT PORTC
#define LCD_DDR DDRC
#define LCD_CPRT PORTC
#define LCD_CDDR DDRC

void LCD_cmd(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_init(void);
void LCD_gotoxy(unsigned char x,unsigned char y);
void LCD_print(char * str);

int main(void)

{    DDRB&=~ (1<<0);//port B bin 1 input
	 PORTB|=(1<<0);//pull up
	 DDRA=0xff;    //port A output
     TCCR0|=(1<<3);//ctc
	 //TCCR0|=(1<<6);//normal mode
	 TCCR0|=(6<<0);//external clk falling
	 OCR0=9;
	 //ADCH=TCNT0;
	 unsigned char table[]={0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000011,0b11111000,0b10000000,0b10011000};
    while(1)
    {   int x=TCNT0;
		LCD_data(x+'0');
		PORTA=table[x];
        //TODO:: Please write your application code 
    }
}
void LCD_cmd(unsigned char cmd)
{
	LCD_PRT = cmd & (0xF0);					//write the high nibble to D4 to D7
	LCD_CPRT &=~(1<<LCD_RS);				//RS = 0 for command
	LCD_CPRT &=~(1<<LCD_RW);				//RW = 0 for write
	LCD_CPRT |=(1<<LCD_EN);					//high to low pulse
	_delay_us(1);
	LCD_CPRT &=~(1<<LCD_EN);
	_delay_us(100);
	
	LCD_PRT = cmd<<4;						//shift to write the low nibble
	LCD_CPRT |=(1<<LCD_EN);	                //high to low pulse
	_delay_us(1);
	LCD_CPRT &=~(1<<LCD_EN);
	_delay_us(100);
}

void LCD_data(unsigned char data)
{
	LCD_PRT = data & (0xF0);				//write the high nibble to D4 to D7
	LCD_CPRT |=(1<<LCD_RS);	                //RS = 1 for data
	LCD_CPRT &=~(1<<LCD_RW);			    //RW = 0 for write
	LCD_CPRT |=(1<<LCD_EN);					//high to low pulse
	_delay_us(1);
	LCD_CPRT &=~(1<<LCD_EN);
	
	LCD_PRT = (data<<4);					//shift to write the low nibble
	LCD_CPRT |=(1<<LCD_EN);				    //high to low pulse
	_delay_us(1);
	LCD_CPRT &=~(1<<LCD_EN);
	_delay_us(100);
}

void LCD_init(void)
{
	LCD_DDR|=0xff;				//LCD Data PORT
	LCD_CDDR|=0xff;				//LCD control PORT
	//LCD_CPRT &=~(1<<LCD_EN);
	LCD_cmd(0x33);				//4 bit mode
	LCD_cmd(0x32);				//4 bit mode
	LCD_cmd(0x28);				//2 lines 5*7 matrix D4 to D7
	LCD_cmd(0x0F);				//display on cursor on
	LCD_cmd(0x01);				//clear LCD
	_delay_us(2000);			//wait 2ms after executing 0x01 and 0x02 modes
	
}

void LCD_gotoxy(unsigned char x,unsigned char y)
{
	unsigned char firstadd[]={0x80,0xc0,0x94,0xD4};
	LCD_cmd(firstadd[x-1] + y-1);
	_delay_us(100);
}

void LCD_print(char* str)
{
	unsigned char i=0;
	while(str[i]!=0)
	{
		LCD_data(str[i]);
		i++;
	}
}

