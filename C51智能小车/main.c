#include "reg52.h"
#include "intrins.h"

#define Display P0

sbit FM = P0 ^ 7;

sbit key1 = P3 ^ 3;
sbit key2 = P3 ^ 4;
sbit key3 = P3 ^ 6;
sbit key4 = P3 ^ 7;

//????????????
sbit Wl = P2 ^ 4;
sbit Wr = P2 ^ 5;
sbit Bz = P2 ^ 7;

unsigned int min = 0, sec = 0, ms100 = 0, ms10 = 0;

unsigned char code smg[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
                   			 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};


unsigned char RecvDate = 'E';
unsigned int k = 0;

//?????????
sbit La = P2 ^ 0;
sbit Lb = P2 ^ 1;
sbit Ra = P2 ^ 2;
sbit Rb = P2 ^ 3;

void delay_ms(unsigned int k)
{
	unsigned int i;
	while(k--)
	{
		i = 124;
		while(i--);
	}
}

// ????????
void waterfull_light();
// ?????????
void paomadeng();
// ??????????
void Dight_Display();
//?????
void Int1_init();

void GoFront();	//??????
void GoBack();	//???????
void GoStop();	//??????
void Left();	//??????
void Right();	//??????



//?????????
void Car_xuanji();

//?????????
void UsartInit();
//??????????????
void BlueToothControl();

void Trum();

//??????
void Bizhang();

void main()
{
	//??????????????? 
	UsartInit();
	
	while(1)
	{
		switch (RecvDate)
		{
		case '1':
			BlueToothControl();
			break;
		case '2':
			Dight_Display();
			break;
		case '3':
			paomadeng();
			break;
		default:
			break;
		}
		
	}
}

void Car_xuanji()
{
	P1 = 0x7e;
	while(1)
	{
		if(RecvDate == 'Q')
		{
			P1 = 0xff;
			while(RecvDate == 'Q');
			return;
		}	
		else if(Wl == 0 && Wr == 0)	
		{
			GoFront();
		}
		else if(Wl == 1 && Wr == 0)
		{
			Left();
			delay_ms(50);
		}
		else if(Wl == 0 && Wr == 1)
		{
			Right();
			delay_ms(50);
		}
		else if(Wl == 1 && Wr == 1)
		{
			GoStop();
		}
	}
}
//???????????
void UsartInit()
{
	TMOD = TMOD & 0x0f | 0x20;	//????????T1????????2
	TR1 = 1;					//??T1
	TH1 = 0xfd;	
	TL1 = 0xfd;

	SCON = 0x50;

	ES = 1;
	EA = 1;
}

void Bizhang()
{
	P1 = 0x3f;
	while(RecvDate != 'Q')
	{
		
		if(Bz == 0)
		{
			Left();
			delay_ms(500);
		}
		else
		{
			GoFront();		
		}
	}
	P1 = 0xff;
}
//??????????????
void BlueToothControl()
{
	while (1)
	{	
		P1 = 0x0f;
		switch (RecvDate)
		{
		case 'W':	//???
			GoFront();
			break;
		case 'S':	//????
			GoBack();
			break;
		case 'A':	//???
			Left();
			break;
		case 'D':	//???
			Right();
			break;
		case 'X':	//???
			Car_xuanji(); 
			break;
		case 'E':
			GoStop();
			break;
		case 'p':	//????????????????
			Trum();
			break;
		case 's':
			Bizhang();	//???????
			break;
		case 'Q':
			GoStop();
			P1 = 0xff;
			return;
		default:
			GoStop();
			break;
		}
	}
	
	
}

void Trum()
{
	FM = ~FM;
	delay_ms(1);
	FM = 1;
}

void Bluetoothrecv() interrupt 4
{
	if(TI)
	{
		TI = 0;
	}
	else if(RI)
	{
		RI = 0;
		RecvDate = SBUF;
	}
}

void GoFront()
{
	La = 1;
	Lb = 0;

	Ra = 1;
	Rb = 0;
}

void GoBack()
{
	La = 0;
	Lb = 1;

	Ra = 0;
	Rb = 1;
}

void GoStop()
{
	La = 0;
	Lb = 0;

	Ra = 0;
	Rb = 0;
}

void Left()
{
	La = 0;
	Lb = 1;

	Ra = 1;
	Rb = 0;
	
}

void Right()
{
	La = 1;
	Lb = 0;

	Ra = 0;
	Rb = 1;
}

void Int1_init()
{
    EX1 = 1;
    IT1 = 1;
    

    TMOD = TMOD & 0xf0 | 0x01;
    
    ET0 = 1;
    EA  = 1;
}

//?????T0???10ms???
void ms_10() interrupt 1
{
    ms10++;
    TH0 = (unsigned int)(65536 - 10000*(11.052/12))/256;
    TL0 = (unsigned int)(65536 - 10000*(11.052/12))%256;
}

void waterfull_light()
{
    unsigned char date = 0xfe;
    while (1) 
	{
        if (RecvDate == 'Q') 
		{
            P1 = 0xff;
            break;
        }

        P1 = date;
        delay_ms(200);
        date = _crol_(date, 1);
    }
}

void paomadeng()
{
    unsigned char date = 0xff;
    while (1) {
        if (RecvDate == 'Q') 
		{
            P1 = 0xff;
            break;
        }
        P1 = date;
        if (date == 0)
            date = 0xff;
        delay_ms(100);
        date <<= 1;
    }
}

void Dight_Display()
{  
	unsigned int option = 0;
    Int1_init();
	
    while (1) 
    {
        if (RecvDate == 'Q') // ??????????4,???
        {
            Display = 0xff;
            P1      = 0xff;
            TR0 =0;
            min   = 0;
            sec   = 0;
            ms100 = 0;
            ms10 = 0;
            return;
        }
        else if(RecvDate == 'p')
        {
            option = 1;
        }
		else if(RecvDate == 's')
        {
            option = 2;
        }
		else if(RecvDate == 'r')
        {
            option = 3;
        }

        if (option == 3) 
        {
            TR0 = 0;
            min   = 0;
            sec   = 0;
            ms100 = 0;
            ms10 = 0;
        }
        else if(option == 2)
        {
            TR0 = 0;
        }
        P1 = 0xfe;Display = smg[ms100];delay_ms(2);Display = 0xff;
        P1 = 0xfd;Display = smg[sec % 10] & 0x7f;delay_ms(2);Display = 0xff;
        P1 = 0xfb;Display = smg[sec / 10];delay_ms(2);Display = 0xff;
        P1 = 0xf7;Display = smg[min] & 0x7f;delay_ms(2);Display = 0xff;

        if (option == 1) 
        {
            TH0 = (unsigned int)(65536 - 10000*(11.052/12))/256;
            TL0 = (unsigned int)(65536 - 10000*(11.052/12))%256;
            TR0 = 1;
            if (ms10 >= 10) 
            {
                ms10 = ms10 % 10;
                ms100++;
                if (ms100 == 10) 
                {
                    ms100 = 0;
                    sec++;
                    if (sec == 60) 
                    {
                        sec = 0;
                        min++;
                        if (min == 10)
                            min = 0;
                    }
                }
            }
        }
    }
}
