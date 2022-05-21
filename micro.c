#include "tm4c123gh6pm.h"


GPIO_PORTA_AMSEL_R = 0x00;
GPIO_PORTA_DIR_R =  0xC0;
GPIO_PORTA_DEN_R = 0xFF;
GPIO_PORTA_PUR_R = 0x00;

GPIO_PORTB_AMSEL_R = 0x00;
GPIO_PORTB_DIR_R =  0xFF;
GPIO_PORTB_DEN_R =  0xFF;
GPIO_PORTB_PUR_R =  0x00;

GPIO_PORTC_AMSEL_R = 0x00;
GPIO_PORTC_DIR_R =  0xC0;
GPIO_PORTC_DEN_R =  0xFF;
GPIO_PORTC_PUR_R =  0x00;

GPIO_PORTD_AMSEL_R = 0x00;
GPIO_PORTD_DIR_R =  0x00;
GPIO_PORTD_DEN_R =  0xFF;
GPIO_PORTD_PUR_R =  0x00;



#define clear_display     0x01
#define returnHome        0x02
#define moveCursorRight   0x06
#define moveCursorLeft    0x08
#define shiftDisplayRight 0x1C
#define shiftDisplayLeft  0x18
#define cursorBlink       0x0F
#define cursorOff         0x0C
#define cursorOn          0x0E
#define Function_set_4bit 0x28
#define Function_set_8bit 0x38
#define Entry_mode        0x06
#define Function_8_bit    0x32
#define Set5x7FontSize    0x20
#define FirstRow          0x80
#define RS 0x20 
#define EN 0x80
#define RW 0x40
#define NVIC_ACTLR_R            (*((volatile unsigned long *)0xE000E008))
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))

void LCD_Write_len( char data,  char control);
void LCD_Cmd(unsigned char command);
void SysTick_Wait(unsigned long delay);
void SysTick_Init(void);
void wait_S (unsigned long delay);
void wait_uS (unsigned long delay);
void buzzer_lunch (void);
void light_ON (void);
void light_OFF (void);
void light_Blink (void);
char keypad_Init(void);
void LCD_init(void);
char KP_scan(void);
void LCD_Write_Char(unsigned char data);
void LCD_String (char *str);
void timer (int countdowntime);


#define LCD GPIOB   /* Define "LCD" as a symbolic name for GPIOB */
// #define RS 0x02 /* PORTB BIT5 mask */
// #define RW 0x04 /* PORTB BIT6 mask */
// #define EN 0x08 /* PORTB BIT7 mask */
#define HIGH 1
#define LOW 0

void SysTick_Wait(unsigned long delay){ // delay is in 12.5ns units
NVIC_ST_RELOAD_R = delay-1; // number of counts to wait
NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
while((NVIC_ST_CTRL_R&0x00010000)==0)
{
	// wait for count flag
}}

void SysTick_Init(void){
NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup
NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value
NVIC_ST_CURRENT_R = 0; // 3) any write to current clears it
NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock
}

void wait_S (unsigned long delay)
{	unsigned long i;
		for (i=0;i<delay;i++)
			SysTick_Wait(4000000);
}




void wait_uS (unsigned long delay)
{	unsigned long i;
		for (i=0;i<delay;i++)
			SysTick_Wait(8);
}


void buzzer_lunch (void){
	GPIO_PORTA_DATA_R |= 0x40;
	wait_S(1);
	GPIO_PORTA_DATA_R &= 0xBF;
	wait_S(1);
	GPIO_PORTA_DATA_R |= 0x40;
	wait_S(1);
	GPIO_PORTA_DATA_R &= 0xBF;
	wait_S(1);
	GPIO_PORTA_DATA_R |= 0x40;
	wait_S(1);
	GPIO_PORTA_DATA_R &= 0xBF;
	wait_S(1);
}

void light_ON (void){
	GPIO_PORTA_DATA_R |= 0x80;
}

void light_OFF(void){
		GPIO_PORTA_DATA_R &= 0x7F;
}

void light_Blink(void){
	while(1)
	{	
		GPIO_PORTA_DATA_R |= 0x80;
		wait_S(1);
		GPIO_PORTA_DATA_R &= 0x7F;
		wait_S(1);
	}
}

char symbol[4][4] = {{ '1', '2',  '3', 'A'},      
                                                { '4', '5',  '6', 'B'},      
                                                { '7', '8',  '9', 'C'},      
                                                { '.', '0',  '=', 'D'}}; 

char keypad_Init(void)
{
  SYSCTL->RCGCGPIO |= 0x09;        //Enable clock to PORTD and PORTA  
  while ((SYSCTL->RCGCGPIO&0x0A)==0);  //wait for clock to be setted
  GPIOD->CR=0x0F;	          
  GPIOA->CR=0x1E;            
  // GPIOD->PDR |= 0x0F;             //Enable pull down resistor on PORTD
}

char KP_scan(void)
{int A=0;
	while(A=0)
    {
		for(int i = 0; i < 4; i++)    //Scan columns loop
    {
      GPIOD->DATA = (1U << i+4);
      wait_uS(2);
      for(int j = 0; j < 4; j++)  //Scan rows
      {
        if((GPIOA->DATA & 0x1E) & (2U << j+1))
          return symbol[j][i];
				A = symbol[j][i];
	  }
    }
	}
			wait_S(1);
	  
}

void LCD_init(void)
{
 SYSCTL->RCGCGPIO |=(1<<1); /* Enable Clock to GPIOB */
 LCD->DIR |=0xFF; /* Set GPIOB all pins a digital output pins */
 LCD->DEN |=0xFF; /* Declare GPIOB pins as digital pins */

 LCD_Cmd(Set5x7FontSize);  /* select 5x7 font size and 2 rows of LCD */
 LCD_Cmd(Function_set_4bit); /* Select 4-bit Mode of LCD */
 LCD_Cmd(moveCursorRight); /* shift cursor right */
 LCD_Cmd(clear_display); /* clear whatever is written on display */
 LCD_Cmd(cursorBlink);  /* Enable Display and cursor blinking */ 
}

void LCD_Cmd(unsigned char command)
{
    LCD_Write_len(command & 0xF0, 0);   /* Write upper part to LCD */
    LCD_Write_len(command << 4, 0);     /* Write lower part to LCD */
    
    if (command < 4)
        wait_uS(2000);         /* 2ms delay for commands 1 and 2 */
    else
        wait_uS(50);        /* 40us delay for other commands */
}


void LCD_Write_len( char data,  char control)
{
    data &= 0xF0;       /* Extract upper part for data */
    control &= 0x0F;    /* Extract lower part for control */
    LCD->DATA = data | control;       /* Set RS and R/W to zero for write operation */
    LCD->DATA = data | control | EN;  /* Provide Pulse to Enable pin to perform wite operation */
     wait_uS(10);
    LCD->DATA = data; /*Send data */
    LCD->DATA = 0; /* stop writing data to LCD */
}

void LCD_Write_Char(unsigned char data)
{
    LCD_Write_len(data & 0xF0, RS);    /* Write upper part to LCD and RS = 1 to write data */
    LCD_Write_len(data << 4, RS);      /* Write lower part to LCD and RS = 1 to write data */
     wait_uS(40);
}

void LCD_String (char *str)	/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)  /* Send each char of string till the NULL */
	{
		LCD_Write_Char(str[i]);  /* Call LCD data write */
	}
}

void timer (int countdowntime)
{
	int n = countdowntime / 60;
	int r = countdowntime % 60;
	for (int i = n; i=0;i--)
	{
		for (int j = r; j=0;j--)
		{
			LCD_String ( n );
			LCD_String ( ":" );
			LCD_String ( n );
			wait_S (2);
			light_ON ();
		}
	}
	light_OFF();
	buzzer_lunch();
}

int main (void) {
	LCD_String ("microwave pro.");
	LCD_String ("choose A B C D");
	char choice = KP_scan() ;

	switch (choice){
	
	case 'A':
		timer (60);
	case 'B':
		LCD_String ("beef weight?");
		int x = KP_scan();
		x= x*30;
		timer (x);
	case 'C':
		LCD_String ("chicken weight?");
		int km = KP_scan();
		x= km*20;
		timer (x);
	case 'D':
		LCD_String ("enter time");
		int m = KP_scan();
		int y = KP_scan();
		int a = KP_scan();
		int b = KP_scan();		
		int z = 600*m + 60*y + a*10 + b;
		if (z>1800)
			LCD_String ("error");

		else {	timer (z);
		}
	}
}
