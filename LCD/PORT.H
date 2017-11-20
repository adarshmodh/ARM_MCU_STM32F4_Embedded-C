
#define RS_Port GPIOB
#define RS_Pin GPIO_Pin_9
#define RWB_Port GPIOA
#define RWB_Pin GPIO_Pin_8
#define EN_Port GPIOB
#define EN_Pin GPIO_Pin_2
#define DATA_Port GPIOD
#define DATA_Pin GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
#define DATA_Pin_0 GPIO_Pin_0

#define LCD_LINE1_CHAR_ADD 0x80
#define LCD_LINE2_CHAR_ADD 0xC0
#define LCD_LINE_LENGTH 16
#define CLEAR_LCD 0x01

#define DOWNKEY					0x04
#define RIGHTKEY 				0x08	
#define ENTERKEY				0x10
#define LEFTKEY 				0x20	
#define UPKEY 					0x40

#define DOWNKEYCODE					(~DOWNKEY)&0x7C
#define LEFTKEYCODE 				(~LEFTKEY)&0x7C	
#define UPKEYCODE					(~UPKEY)&0x7C
#define RIGHTKEYCODE 				(~RIGHTKEY)&0x7C	
#define ENTERKEYCODE 				(~ENTERKEY)&0x7C
#define SAVEKEYCODE 				(~(RIGHTKEY+DOWNKEY))&0x7C
#define ESCAPEKEYCODE 				(~(LEFTKEY+UPKEY))&0x7C
#define CHANGEPWKEYCODE				(~(RIGHTKEY+LEFTKEY))&0x7C


