//------------------Functions---------------------
void GPTM1_init();
void GPTM2_init();
void GPTM3_init();
void PORTF_init();
void PORTB_init();
void PORTC_init();
void PORTD_init();
//-------------------Handlers---------------------
void carshandler(void);
void Timer2Handler(void);
void Timer3Handler(void);
void Pedestrian1(void);
void Pedestrian2(void);
void flag_Handler(void);
//-----------------PortF--------------------------
#define DirF (*((unsigned int*)0x40025400))
#define DenF (*((unsigned int*)0x4002551C))
#define PurrF (*((unsigned int*)0x40025510))
//------------------Clock Register-------------------------
#define clock (*((unsigned int*)0x400FE608))
//-----------------LEDs---------------------------------
#define LED_Red (*((volatile unsigned int*)0x400253FC)^=0x02)//0010
#define LED_Blue ( *((volatile unsigned int*)0x400253FC)=0x04)//0100
#define LED_Green ( *((volatile unsigned int*)0x400253FC)=0x08)//1000
#define LED_Yellow ( *((volatile unsigned int*)0x400253FC)=0x0A)//1010