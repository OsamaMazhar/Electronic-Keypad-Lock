#include <reg51.h>

#define COL P2
#define ROW P1

sfr ldata = 0x0B0;		//LCD PORT P3

sbit rs = P2^4;				//LCD Control Pins
sbit rw = P2^5;
sbit en = P2^6;

sbit busy = P3^7;

sbit co = P1^2;
sbit out1 = P1^4;
sbit out2 = P1^5;
sbit alarm1 = P1^6;
sbit alarm2 = P1^7;

sbit reset = P2^1;


void lcdready();
void lcdcmd(unsigned char value);
void lcddata(unsigned char value);
void MSDelay(unsigned int value);
unsigned char KeypadRead();
//void InttoLCD(unsigned int value);

unsigned char code keypad[4][4] = {'7','8','9','/','4','5','6','X','1','2','3','-','O','0','=','+'};
unsigned char cnt[12];
bit Cas;					// I first used char data type, and in the main where I inverted Cas, I expected it to become 1 from 0. but it wasn't working... Then Allah allowed me to understand the thing and I just thought that inverting an 8-bit registerwould give me 0xFF not '1'... So I then used bit datatype. 

void main()
	{
	  unsigned char i, x, character[5], cnt = 0;
	  unsigned char Enter[14] = "Enter Password", Open[9] = "Lock Open", Sorry[8] = "Sorry No", More[10] = "More Tries";
	  unsigned char Pass[5] = "23456";
	  out1 = 0; out2 = 0; alarm1 = 0; alarm2 = 0;
		COL = 0xFF;								// make P2 an Input Port
	  
	  lcdcmd(0x38);								//2 lines and 5X7 matrix
	  lcdcmd(0x0E);								//Display ON, Cursor Blinking = 0x0E But this command is Display ON, Cursor OFF
	  lcdcmd(0x80);								//Force Cursor to the beginning of the First Line
	  lcdcmd(0x06);

	  for(x=0; x<4; x++)
		{	  
start:	lcdcmd(0x01);								//Clear Display Screen
		 for(i=0; i<14; i++)
	  	 	lcddata(Enter[i]);
	  	 lcdcmd(0xC0);								// Go to Second Line of LCD	
		 for(i=0; i<5; i++)
			 {
			  character[i] = KeypadRead();
			  lcddata('*');
			 }
// Compare Character Array (of 5 Elements) with pre-defined Array in the program.

		if(character[0] == Pass[0])
			cnt++;
		if(character[1] == Pass[1])
			cnt++;
		if(character[2] == Pass[2])
			cnt++;		
		if(character[3] == Pass[3])
			cnt++;		
		if(character[4] == Pass[4])
			cnt++;
		
		if(cnt > 4)
			{
			 lcdcmd(0x01);
			 for(i=0; i<9 ; i++)
			 	lcddata(Open[i]);
				out1 = 1;
				out2 = 0;
			  MSDelay(5000);
				out1 = 0;
				cnt = 0;
				goto start;
			}
		//Here
		}
	co = 0;
	lcdcmd(0x01);
	for(i=0; i<8; i++)
		lcddata(Sorry[i]);
	lcdcmd(0xC0);
	for(i=0; i<10; i++)
		lcddata(More[i]);
	MSDelay(500);
re:	while(reset == 1) {alarm1 = 1; alarm2 = 0;}
	cnt = 0;
	if(reset == 0)
		{
		 MSDelay(8000);
			if(reset == 0){
				alarm1 = 0, alarm2 = 0; x = 0;
				goto start;}
			else goto re;
		}
	}

void MSDelay(unsigned int value)
	{
		unsigned int x,y;
		for(x=0;x<value;x++)
			for(y=0;y<127;y++);
	}

void lcdready()
	{
		busy = 1;	  						//make the busy pin an input
		rs = 0;
		rw = 1;
		while(busy == 1)					//wait here for busy flag
			{
				en = 0;						//strobe the enable pin
				MSDelay(1);
				en = 1;
			}
		return;					
	 }
	 	  
void lcdcmd(unsigned char value)
	{
		lcdready();
		ldata = value;
		rs = 0;
		rw = 0;
		en = 1;
		MSDelay(1);
		en = 0;
		return;
	}


void lcddata(unsigned char value)
	{
		lcdready();
		ldata = value;
		rs = 1;
		rw = 0;
		en = 1;
		MSDelay(1);
		en = 0;
		return;
	}

unsigned char KeypadRead()
	{
		unsigned char colloc, rowloc; 		
		do
		   {
		   ROW = 0x00;							//ground all rows at once
		   colloc = COL;						//read the columns
		   colloc &= 0x0F;						//mask unused bits
		   }
		while(colloc != 0x0F);					//check until all keys are released
		
		do
		   {
		  	 do
		   		{
				MSDelay(1);  						//call delay
				colloc = COL;						//see if any key is pressed
				colloc &= 0x0F;						//mask unsused bits
				}
		 	 while(colloc == 0x0F);					//keep checking for keypress

			MSDelay(1);								//call delay for debounce
			colloc = COL;							//read columns
			colloc &= 0x0F;							//masku unused bits
			}
			while(colloc == 0x0F);					//wait for keypress

			while(1)
				{
					ROW = 0xFE;						//ground row 0 FE = 11111110b
					colloc = COL;					//read columns
					colloc &= 0x0F;					//masku unused bits
					if(colloc != 0x0F)				//column detected
						{
							rowloc = 0;				//save row location
							break;
						}
					ROW = 0xFD;						//ground row 0 Fd = 11111101b
					colloc = COL;					//read columns
					colloc &= 0x0F;					//masku unused bits
					if(colloc != 0x0F)				//column detected
						{
							rowloc = 1;				//save row location
							break;
						}
					ROW = 0xFB;						//ground row 0 FB = 11111011b
					colloc = COL;					//read columns
					colloc &= 0x0F;					//masku unused bits
					if(colloc != 0x0F)				//column detected
						{
							rowloc = 2;				//save row location
							break;
						}
					ROW = 0xF7;						//ground row 0 F7 = 11110111b
					colloc = COL;					//read columns
					colloc &= 0x0F;					//mask unused bits
					if(colloc != 0x0F)				//column detected
					rowloc = 3;						//save row location
					break;
				}
		//check columns and send result to LCD
		if(colloc == 0x0E)							//0E = 00001110
			return keypad[rowloc][0];
		else if(colloc == 0x0D)						//0D = 00001101
			return keypad[rowloc][1];
		else if(colloc == 0x0B)						//0B = 00001011
			return keypad[rowloc][2];
		else
			return keypad[rowloc][3];
		}

/*void InttoLCD(unsigned int value)
	{
	 unsigned int x, y, z, d[5];
	 char l;
	 x = value / 10; 							// => 6553
	 d[0] = (value % 10) + 48; 					// => 6 (LSD) *
	 d[1] = (x % 10) + 48;	 					// => 3 *
	 y = x / 10; 								// => 655
	 d[2] = (y % 10) + 48; 						// => 5 *
	 z = y / 10; 								// => 65
	 d[3] = (z % 10) + 48;						// => 5 *
	 d[4] = (z / 10) + 48;						// => 6 (MSD) *
	 if(d[4] == 48 & d[3] == 48 & d[2] == 48 & d[1] == 48)
		{
		  lcddata(d[0]);
		  for(l=0; l<4; l++)
		 	 lcddata(' ');
		  for(l=0; l<4; l++)
		  	 lcdcmd(0x10);
		}
	 if(d[4] == 48 & d[3] == 48 & d[2] == 48 & d[1] != 48)
		{
		 for(l=1; l>=0; l--)
	 	 	lcddata(d[l]);
		 for(l=0; l<3; l++)
		 	 lcddata(' ');
		 for(l=0; l<3; l++)
		  	 lcdcmd(0x10);
		}
	 if(d[4] == 48 & d[3] == 48 & d[2] != 48)
		{
		 for(l=2; l>=0; l--)
	 	 	lcddata(d[l]);
		 for(l=0; l<2; l++)
		 	 lcddata(' ');
		 for(l=0; l<2; l++)
		  	 lcdcmd(0x10);
		}
	 if(d[4] == 48 & d[3] != 48)
	 	{
		 for(l=3; l>=0; l--)
	 	 	lcddata(d[l]);
		 lcddata(' ');
		 lcdcmd(0x10);
		}
	 if(d[4] != 48)
	 	{
		 for(l=4; l>=0; l--)
		 lcddata(d[l]);
		}
	}		*/