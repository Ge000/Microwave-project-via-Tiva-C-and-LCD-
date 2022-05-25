#include "tm4c123gh6pm.h"
#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"

#define LCD GPIOB    		//LCD port with Tiva C 
#define RS 0x01				 	//RS -> PB0 (0x01)
#define RW 0x02         //RW -> PB1 (0x02)
#define EN 0x04  		 	 	//EN -> PB2 (0x04)

#define SYSCTL_RCGCGPIO_R   (*((volatile int *) 0x400FE608))   
#define GPIO_PORTA_DEN_R    (*((volatile int *) 0x4000451C))
#define GPIO_PORTA_DIR_R    (*((volatile int *) 0x40004400))
#define GPIO_PORTA_DATA_R   (*((volatile int *) 0x40004010))
#define GPIO_PORTA_AMSEL_R  (*((volatile int *) 0x40004528))
#define GPIO_PORTA_AFSEL_R  (*((volatile int *) 0x40004420))
#define GPIO_PORTA_PCTL_R   (*((volatile int *) 0x4000452C))


#define GPIO_PORTA_CLK_EN  0x01      //enable clock for PORTA
#define GPIO_PORTA_PIN2_EN 0x04			 //enable pin 2 of PORTA
#define GPIO_PORTA_PIN3_EN 0x08

#define LED_ON2            0x04			 //turn on  LED on Pin 2 PORTA
#define LED_OFF2           0x00			 //turn off  LED on Pin 2 PORTA
#define  B_ON3          0x08
#define  B_OFF3          0x00

#define  RowsSize  4
#define  ColsSize  4

#define NVIC_ACTLR_R            (*((volatile unsigned long *)0xE000E008))
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))


unsigned static char symbol[4][4] = {{ '1', '2',  '3', 'A'},      
                                                { '4', '5',  '6', 'B'},      
                                                { '7', '8',  '9', 'C'},      
                                                { '*', '0',  '#', 'D'}}; 

 
																								
//Functions Declaration

void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void wait_S (unsigned long delay);
void delayUs(int);   														   //Delay in Micro Seconds
void delayMs(int);
void LCD4bits_Init(void);													 //Initialization of LCD Dispaly
void LCD_Write4bits(unsigned char, unsigned char); //Write data as (4 bits) on LCD
void LCD_WriteString(char*);											 //Write a string on LCD 
void LCD4bits_Cmd(unsigned char);									 //Write command 
void LCD4bits_Data(unsigned char);								 //Write a character
int Timer (int);
void LCD_Write_Char(unsigned int data);
void LCD_Write_len( unsigned int data,  char control);
																								
void keypad_Init(void); /* Keypad initialization */
char keypad_getkey(void); /*Scan and detect key pressed */
																								
void buzzer_lunch (void);
void light_ON (void);
void light_OFF (void);
void light_Blink (void);
void f1 (int x);
int strtoint(char str);

int main(void)
{
	
	keypad_Init(); 										//Initialize keypad and TM4C123 GPIO pins
	LCD4bits_Init();									//Initialization of LCD
	LCD4bits_Cmd(0x80);               //Force the cursor to beginning of 1st line
	delayMs(500);											//delay 500 ms for LCD (MCU is faster than LCD)
	
	char *gogo = "3rd year CSE";    
	LCD_WriteString(gogo);							
	delayMs(1000);		
	LCD4bits_Cmd(0xC0);
  gogo = "Microwave pro.";    
	LCD_WriteString(gogo);						
	delayMs(1000);
	
	LCD4bits_Cmd(0x01);
  LCD_WriteString("choose A B C D");
	LCD4bits_Cmd(0xC0);
	
	
	
	char **value = keypad_getkey(); 	//get the key pressed value
  LCD_WriteString(&value); 						// display key value on LCD
  delayMs(1000);
	
	
	int x= 0;
	while (x==0)
		{if (value == 'A')
			{
x=1;
			LCD4bits_Cmd(0x01);
			LCD_WriteString ("Popcorn...");
			
			f1(60);
			light_OFF();
			LCD4bits_Cmd (0xc2);
			LCD_WriteString ("Popcorn ready");
		  buzzer_lunch();
			}
else if(value =='B'){
x=1;
int xx =0;
	while (xx==0)
		{	  
			LCD4bits_Cmd(0x01);
			LCD_WriteString ("beef weight?");
			LCD4bits_Cmd(0xC0);
			
			char **value = keypad_getkey(); 	//get the key pressed value
  LCD_WriteString(&value);
			delayMs(1500);
			
	if ((value == 'A')||(value == 'D')||(value == 'C')||(value == 'B')||(value == '#')||(value == '*')){
	LCD_WriteString("error try again");
	delayMs(1500);
	}
	else { if (value == '1'){xx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Beef...");f1(30);LCD4bits_Cmd (0xc2);LCD_WriteString (" Beef ready");light_OFF();buzzer_lunch();}
if (value == '2'){xx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Beef...");f1(60);LCD4bits_Cmd (0xc2);LCD_WriteString (" Beef ready");light_OFF();buzzer_lunch();}
if (value == '3'){xx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Beef...");f1(90);LCD4bits_Cmd (0xc2);LCD_WriteString (" Beef ready");light_OFF();buzzer_lunch();}
if (value == '4'){xx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Beef...");f1(120);LCD4bits_Cmd (0xc2);LCD_WriteString (" Beef ready");light_OFF();buzzer_lunch();}
if (value == '5'){xx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Beef...");f1(150);LCD4bits_Cmd (0xc2);LCD_WriteString (" Beef ready");light_OFF();buzzer_lunch();}
if (value == '6'){xx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Beef...");f1(180);LCD4bits_Cmd (0xc2);LCD_WriteString (" Beef ready");light_OFF();buzzer_lunch();}
if (value == '7'){xx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Beef...");f1(210);LCD4bits_Cmd (0xc2);LCD_WriteString (" Beef ready");light_OFF();buzzer_lunch();}
if (value == '8'){xx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Beef...");f1(240);LCD4bits_Cmd (0xc2);LCD_WriteString (" Beef ready");light_OFF();buzzer_lunch();}
if (value == '9'){xx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Beef...");f1(270);LCD4bits_Cmd (0xc2);LCD_WriteString (" Beef ready");light_OFF();buzzer_lunch();}
	
	}
		
	}}
else if((value =='C')){
x=1;
	
int xxx =0;
	while (xxx==0)
		{	  
			LCD4bits_Cmd(0x01);
			LCD_WriteString ("chicken weight?");
			LCD4bits_Cmd(0xC0);
			
			char **value = keypad_getkey(); 	//get the key pressed value
  LCD_WriteString(&value);
			delayMs(1500);
			
	if ((value == 'A')||(value == 'D')||(value == 'C')||(value == 'B')||(value == '#')||(value == '*')){
	LCD_WriteString("error try again");
	delayMs(1500);
	}
	else { if (value == '1'){xxx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Chicken...");f1(20);LCD4bits_Cmd (0xc2);LCD_WriteString ("Chicken ready");light_OFF();buzzer_lunch();}
if (value == '2'){xxx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Chicken...");f1(40);LCD4bits_Cmd (0xc2);LCD_WriteString ("Chicken ready");light_OFF();buzzer_lunch();}
if (value == '3'){xxx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Chicken...");f1(60);LCD4bits_Cmd (0xc2);LCD_WriteString ("Chicken ready");light_OFF();buzzer_lunch();}
if (value == '4'){xxx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Chicken...");f1(80);LCD4bits_Cmd (0xc2);LCD_WriteString ("Chicken ready");light_OFF();buzzer_lunch();}
if (value == '5'){xxx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Chicken...");f1(100);LCD4bits_Cmd (0xc2);LCD_WriteString ("Chicken ready");light_OFF();buzzer_lunch();}
if (value == '6'){xxx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Chicken...");f1(120);LCD4bits_Cmd (0xc2);LCD_WriteString ("Chicken ready");light_OFF();buzzer_lunch();}
if (value == '7'){xxx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Chicken...");f1(140);LCD4bits_Cmd (0xc2);LCD_WriteString ("Chicken ready");light_OFF();buzzer_lunch();}
if (value == '8'){xxx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Chicken...");f1(160);LCD4bits_Cmd (0xc2);LCD_WriteString ("Chicken ready");light_OFF();buzzer_lunch();}
if (value == '9'){xxx=1; LCD4bits_Cmd(0x01);LCD_WriteString ("Chicken...");f1(180);LCD4bits_Cmd (0xc2);LCD_WriteString ("Chicken ready");light_OFF();buzzer_lunch();}
	
	}
		
	}

				
}

else if((value =='D')){
x=1;
	int xxxx =0;
	while (xxxx==0)
		{
			//char z0 ='9';
			//int b0 =strtoint(z0);
			//f1(b0);
			
	int b1, b2 , b3 , b4;
LCD4bits_Cmd (0x01);
LCD_WriteString("enter time then#");

unsigned char m1 = keypad_getkey();LCD4bits_Cmd (0xc5);LCD_WriteString("00:0");LCD4bits_Data(m1);
			delayMs(1200);
	
if ((m1 == 'A')||(m1 == 'D')||(m1 == '0')||(m1 == 'C')||(m1 == 'B')||(m1 == '#')||(m1 == '*')){
	LCD_WriteString("error try again");}
			else {char m2  = keypad_getkey() ;LCD4bits_Cmd (0xc5);LCD_WriteString("00:");LCD4bits_Data(m1);LCD4bits_Data(m2);delayMs(1200);
			if ((m2 == 'A')||(m2 == 'D')||(m2 == 'C')||(m2 == 'B')||(m2 == '*')){
							LCD_WriteString("error try again");delayMs(1500);}
							else if (m2 == '#'){
								b1=strtoint(m1);
								f1(b1);xxxx=1;light_OFF();buzzer_lunch();}
									 else {char m3  = keypad_getkey();LCD4bits_Cmd (0xc5);LCD_WriteString("0");LCD4bits_Data(m1);LCD_WriteString(":"); LCD4bits_Data(m2);LCD4bits_Data(m3);delayMs(1200);
									 if ((m3 == 'A')||(m3 == 'D')||(m3 == 'C')||(m3 == 'B')||(m3 == '*')){
											LCD_WriteString("error try again");
											delayMs(1500);}
											else if (m3 == '#'){
													b1=strtoint(m1);
													b2=strtoint(m2);
													f1(b1*10+b2);xxxx=1;light_OFF();buzzer_lunch();}
													else {char m4  = keypad_getkey();LCD4bits_Cmd (0xc5);;LCD4bits_Data(m1); LCD4bits_Data(m2);LCD_WriteString(":");LCD4bits_Data(m3);LCD4bits_Data(m4);delayMs(1200);
														if ((m4 == 'A')||(m4 == 'D')||(m4 == 'C')||(m4 == 'B')||(m4 == '*')){
																LCD_WriteString("error try again");delayMs(1500);}
																else if (m4 == '#'){
																		 b1=strtoint(m1);
																		 b2=strtoint (m2);
																		 b3=strtoint(m3);
																		 f1(b1*60+b2*10+b3);xxxx=1;light_OFF();buzzer_lunch();}			
																			else b1=strtoint(m1);
																		 b2=strtoint (m2);
																		 b3=strtoint(m3);
																		 b4=strtoint(m4);
																		 if (b1>3){LCD_WriteString("error overflow");delayMs(1500);}
																		 else {char m5 = keypad_getkey();
																					if (m5 != '#'){
																							LCD_WriteString("error");delayMs(1500);}
																							else {f1(b1*600+b2*60+b3*10+b4);xxxx=1;light_OFF();buzzer_lunch();} 
				}}}}
				}
		LCD4bits_Cmd(0x01);
		LCD_WriteString("done");
				}


else {LCD_WriteString("error try again");
			delayMs(1500);
		  LCD4bits_Cmd(0x01);
			LCD_WriteString("choose A B C D");
			LCD4bits_Cmd(0xC0);
			value = keypad_getkey(); 	//get the key pressed value
			LCD_WriteString(&value); 						// display key value on LCD
			delayMs(1000);
}
	}
	}
void LCD4bits_Init(void)
{
	SYSCTL->RCGCGPIO |= 0x02;    //enable clock for PORTB
	delayMs(10);                 //delay 10 ms for enable the clock of PORTB
  LCD->DIR = 0xFF;             //let PORTB as output pins
	LCD->DEN = 0xFF;             //enable PORTB digital IO pins
	LCD4bits_Cmd(0x28);          //2 lines and 5x7 character (4-bit data, D4 to D7)
	LCD4bits_Cmd(0x06);          //Automatic Increment cursor (shift cursor to right)
	LCD4bits_Cmd(0x01);					 //Clear display screen
	LCD4bits_Cmd(0x0F);          //Display on, cursor blinking
}


void LCD_Write4bits(unsigned char data, unsigned char control)
{
	data &= 0xF0;                       //clear lower nibble for control 
	control &= 0x0F;                    //clear upper nibble for data
	LCD->DATA = data | control;         //Include RS value (command or data ) with data 
	LCD->DATA = data | control | EN;    //pulse EN
	delayUs(0);													//delay for pulsing EN
	LCD->DATA = data | control;					//Turn off the pulse EN
	LCD->DATA = 0;                      //Clear the Data 
}

void LCD_WriteString(char * str)
{  
	volatile int i = 0;          //volatile is important 
	
	while(*(str+i) != '\0')       //until the end of the string
	{
		LCD4bits_Data(*(str+i));    //Write each character of string
		i++;                        //increment for next character
	}
}

void LCD4bits_Cmd(unsigned char command)
{
	LCD_Write4bits(command & 0xF0 , 0);    //upper nibble first
	LCD_Write4bits(command << 4 , 0);			 //then lower nibble
	
	if(command < 4)
		delayMs(2);       //commands 1 and 2 need up to 1.64ms
	else
		delayUs(40);      //all others 40 us
}

void LCD4bits_Data(unsigned char data)
{
	LCD_Write4bits(data & 0xF0 , RS);   //upper part first
	LCD_Write4bits(data << 4 , RS);     //then part nibble
	delayUs(40);												//delay for LCD (MCU is faster than LCD)
}


void SysTick_Init(void){
NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup
NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value
NVIC_ST_CURRENT_R = 0; // 3) any write to current clears it
NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock
}

void SysTick_Wait(unsigned long delay){ // delay is in 12.5ns units
NVIC_ST_RELOAD_R = delay-1; // number of counts to wait
NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
while((NVIC_ST_CTRL_R&0x00010000)==0)
{
	// wait for count flag
}}

void wait_S (unsigned long delay){
	unsigned long i;
		for (i=0;i<delay;i++)
			SysTick_Wait(4000000);
}
void delayUs(int n){
	volatile int i,j;							//volatile is important for variables incremented in code
	for(i=0;i<n;i++)
		for(j=0;j<3;j++)            //delay for 1 micro second
		{}
}

void delayMs(int n){
	volatile int i,j;							//volatile is important for variables incremented in code
	for(i=0;i<n*1000;i++)
		for(j=0;j<3;j++)            //delay for 1 micro second
		{}
}
void keypad_Init(void)
{
  SYSCTL->RCGCGPIO |= 0x14;        //Enable clock to PORTC and PORTE  
  while ((SYSCTL->RCGCGPIO&0x14)==0);  //wait for clock to be setted
  GPIOC->CR  |= 0xF0;             //Allow settings for all pins of PORTC
  GPIOE->CR  |= 0x0F;             //Allow settings for all pins of PORTE
  GPIOE->AMSEL &= 0xF0;
  GPIOC->AMSEL &= 0x0F;
  GPIOE->PCTL &= 0xFFFF0000;
  GPIOC->PCTL &= 0x0000FFFF;
  GPIOE->DIR &= 0xF0;             //PE0-PE3 are used with row and set them as digital input pins
  GPIOC->DIR |= 0xF0;             //PC4-PC7 are used with col and set them as digital output pins
  GPIOE->AFSEL &= 0xF0;
  GPIOC->AFSEL &= 0x0F;
  GPIOE->PDR |= 0x0F;             //Enable pull down resistor on PORTE
  GPIOC->DEN |= 0xF0;             //Set PORTC as digital pins
  GPIOE->DEN |= 0x0F;             //Set PORTE as digital pins
}

char keypad_getkey(void)
{
  while(1)
  {
    for(int i = 0; i < 4; i++)    //Scan columns loop
    {
      GPIOC->DATA = (1U << i+4);
      delayMs(2);
      for(int j = 0; j < 4; j++)  //Scan rows
      {
        if((GPIOE->DATA & 0x0F) & (1U << j))
          return symbol[j][i];
      }
    }
  }
}

void buzzer_lunch (void){
	GPIO_PORTA_DEN_R |= GPIO_PORTA_PIN3_EN;        	  // Enable pin 2 of PORTA 
  GPIO_PORTA_DIR_R |= GPIO_PORTA_PIN3_EN;        	  // Make pin 2 of PORTA as ouptut pin
	GPIO_PORTA_DATA_R = B_ON3;
delayMs(500);
	GPIO_PORTA_DATA_R = B_OFF3;
delayMs(500);	 	
	GPIO_PORTA_DATA_R = B_ON3;
delayMs(500);
	GPIO_PORTA_DATA_R = B_OFF3;
delayMs(500);	 
	GPIO_PORTA_DATA_R = B_ON3;
delayMs(500);
	GPIO_PORTA_DATA_R = B_OFF3;
delayMs(500);	 
	
}

void light_ON (void){
	SYSCTL_RCGCGPIO_R |= GPIO_PORTA_CLK_EN;           //activate clock for Port A
  delayMs(1000);           															//Delay 10 msec to allow clock to start on PORTA                  
  GPIO_PORTA_DEN_R |= GPIO_PORTA_PIN2_EN;        	  // Enable pin 2 of PORTA 
  GPIO_PORTA_DIR_R |= GPIO_PORTA_PIN2_EN;        	  // Make pin 2 of PORTA as ouptut pin
	GPIO_PORTA_PCTL_R &= ~GPIO_PORTA_PIN2_EN; 				// Regular GPIO of PORTA
  GPIO_PORTA_AMSEL_R &= ~GPIO_PORTA_PIN2_EN;        // Disable analog function on pin 2 of PORTA
	GPIO_PORTA_AFSEL_R &= ~GPIO_PORTA_PIN2_EN;        // Regular port function
	GPIO_PORTA_DATA_R = LED_ON2;    //Turn on Blue LED 	 
}

void light_OFF(void){
		GPIO_PORTA_DATA_R &= LED_OFF2;
}

void light_Blink(void){
	while(1)
	{	
			GPIO_PORTA_DATA_R = LED_ON2;
		wait_S(1);
		GPIO_PORTA_DATA_R &= LED_OFF2;
		wait_S(1);
	}
}


int Timer (int x){
	
	LCD4bits_Cmd(0x01);
	LCD_WriteString ("done");
	
	return 0 ;

}
void f1 (int x){
		delayMs(100);
		light_ON();
		char** f1_char;
		char** f2_char;
int min,sec;
	x=x+1;
	min =x/60;
	sec=x%60;
	while (min!=-1){
				LCD4bits_Cmd(0xC5);
				sprintf(&f2_char,"%d",min);
				if (min<11){
				LCD_WriteString ("0");
				}
				LCD_WriteString (&f2_char);
				LCD_WriteString (":");
		
		while (sec!=0)
			{	LCD4bits_Cmd(0xC8);
				sprintf(&f1_char,"%d",sec-1);
				if (sec<11){
				LCD_WriteString ("0");
				}
				LCD_WriteString (&f1_char);
				sec = sec-1;
							delayMs(300);
							}
				min = min - 1;
				sec=sec+60;
}}

int strtoint(char str){
	int x1x;
if (str =='1'){x1x = 1;};
if (str =='2'){x1x = 2;};
if (str =='3'){x1x = 3;};
if (str =='4'){x1x = 4;};
if (str =='5'){x1x = 5;};
if (str =='6'){x1x = 6;};
if (str =='7'){x1x = 7;};
if (str =='8'){x1x = 8;};
if (str =='9'){x1x = 9;};
if (str =='0'){x1x = 0;};
return x1x;
}
