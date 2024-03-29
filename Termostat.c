/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : Termostat 
Version : v 3.0
Date    : 21.04.2018
Author  : Ivan Peredriy
Company : 
Comments: 
���������  ��� ���������


Chip type               : ATmega8A
Program type            : Application
AVR Core Clock frequency: 8,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega8.h>

#include <delay.h>


#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))
#define coeff_beta 15
#define coeff_alpha 1
#define N 4
eeprom signed char P4;
eeprom signed int P1=20, P2=150;
eeprom unsigned char P0, P3, P5, P6;

/* 
PD0 = A
PD1 = F
PD2 = B
PD3 = E
PD4 = D
PD5 = G
PD6 = C
PD7 = H
*/

unsigned static char kod[20]={0x05, 0xDD, 0x43, 0x51, 0x99, 0x31, 0x21, 0x5D, 0x01, 0x11, 0x85, 0x31, 0xA3, 0x0B, 0x27, 0xE1, 0xA7, 0x89, 0xE9, 0x2B};    // 0,1,2,3,4,5,6,7,8,9,U, S, t, P, C, o, L, H, n, F...
unsigned char t1,t2,t3,i;
unsigned char wait_timer1, wait_timer_for_termostat, counter_buzzer=20;
signed char screen=0;
int par, buffer, minus=0, buzzer;
int adc=1000;
int filterin, wait_timer2=0, currtemp;
bit knopka_left, knopka_right, knopka_middle, avariya_datchik_ne_podkluchen=0, show_temp=1, show_termostat=0, show_ustavku=0, par_ust=0,
P00=0, P01=0, P02=0, P03=0, P04=0, P05=0, P06=0, exit=0, show_par=0, flag_enter=0, OVERHEAT=0, flag_minus=0, flag_pereklu4=0;        

int getkodes(signed int x) {
 char c1, c2, c3;

                c1=x%10;
                c2=(x%100)/10;
                c3=(x%1000)/100; 
                
                                switch(c1) {
                            case 0: {t1=kod[0]; break;}
                            case 1: {t1=kod[1]; break;}
                            case 2: {t1=kod[2]; break;}
                            case 3: {t1=kod[3]; break;}
                            case 4: {t1=kod[4]; break;}
                            case 5: {t1=kod[5]; break;}
                            case 6: {t1=kod[6]; break;}
                            case 7: {t1=kod[7]; break;}
                            case 8: {t1=kod[8]; break;}
                            case 9: {t1=kod[9]; break;}                                            
                            }                        
                        
                                switch(c2) {
                            case 0: {t2=kod[0]; break;}
                            case 1: {t2=kod[1]; break;}
                            case 2: {t2=kod[2]; break;}
                            case 3: {t2=kod[3]; break;}
                            case 4: {t2=kod[4]; break;}
                            case 5: {t2=kod[5]; break;}
                            case 6: {t2=kod[6]; break;}
                            case 7: {t2=kod[7]; break;}
                            case 8: {t2=kod[8]; break;}
                            case 9: {t2=kod[9]; break;}                                            
                            } 
                        
                                switch(c3) {
                            case 0: {t3=kod[0]; break;}
                            case 1: {t3=kod[1]; break;}
                            case 2: {t3=kod[2]; break;}
                            case 3: {t3=kod[3]; break;}
                            case 4: {t3=kod[4]; break;}
                            case 5: {t3=kod[5]; break;}
                            case 6: {t3=kod[6]; break;}
                            case 7: {t3=kod[7]; break;}
                            case 8: {t3=kod[8]; break;}
                            case 9: {t3=kod[9]; break;}                                            
                            }
                     
           return 0;     

}

unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
delay_us(10);
ADCSRA|=(1<<ADSC);
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}

float readtemp()
{
float temper;
  if(adc < 10)
    {
     OVERHEAT=1;
    };
    
  if (adc > 17 && adc < 21)
    {
     temper=(145)+(21-adc)*2.22;
    }  
    
  if (adc > 20 && adc < 23)
    {
     temper=(140)+(23-adc)*1.93;
    }  
    
  if (adc > 22 && adc < 26)
    {
     temper=(135)+(26-adc)*1.68;
    }  
    
  if (adc > 25 && adc < 30)
    {
     temper=(130)+(30-adc)*1.46;
    }  
    
  if (adc > 29 && adc < 34)
    {
     temper=(125)+(34-adc)*1.27;
    }  
    
  if (adc > 33 && adc < 38)
    {
     temper=(120)+(38-adc)*1.09;
    }  
    
  if (adc > 37 && adc < 43)
    {
     temper=(115)+(43-adc)*0.95;
    }  
    
  if (adc > 42 && adc < 50)
    {
     temper=(110)+(50-adc)*0.82;
    }  
    
  if (adc > 49 && adc < 57)
    {
     temper=(105)+(57-adc)*0.70;
    }  
    
  if (adc > 56 && adc < 65)
    {
     temper=(100)+(65-adc)*0.60;
    }  
    
  if (adc > 64 && adc < 75)
    {
     temper=(95)+(75-adc)*0.58;
    }
      
  if (adc > 74 && adc < 86)
    {
     temper=(90)+(86-adc)*0.44;
    }
  if (adc > 85 && adc < 99)
    {
     temper=(85)+(99-adc)*0.38;
    }  
      
  if (adc > 98 && adc < 114)
    {
     temper=(80)+(114-adc)*0.33;
    }
      
  if (adc > 113 && adc < 132)
    {
     temper=(75)+(132-adc)*0.28;
    }
      
  if (adc > 131 && adc < 153)
    {
     temper=(70)+(153-adc)*0.24;
    }
  if (adc > 152 && adc < 177)
    {
     temper=(65)+(177-adc)*0.21;
    };
    
  if (adc > 176 && adc < 204)
    {
     temper=(60)+(204-adc)*0.18;
    };
    
  if (adc > 203 && adc < 236)
    {
     temper=(55)+(236-adc)*0.16;
    };
    
  if (adc > 235 && adc < 271)
    {
     temper=(50)+(271-adc)*0.14;
    };

  if (adc > 270 && adc < 312)
    {
     temper=(45)+(312-adc)*0.12;
    };
    
  if (adc > 311 && adc < 356)
    {
     temper=(40)+(356-adc)*0.11;
    };
    
  if (adc > 355 && adc < 405)
    {
     temper=(35)+(405-adc)*0.10;
    };
    
  if (adc > 404 && adc < 457)
    {
     temper=(30)+(457-adc)*0.10;
    };
    
  if (adc > 456 && adc < 469)
    {
     temper=(29)+(469-adc)*0.08;
    };
    
  if (adc > 468 && adc < 480)
    {
     temper=(28)+(480-adc)*0.09;
    };
    
  if (adc > 479 && adc < 491)
    {
     temper=(27)+(491-adc)*0.09;
    };    
    
  if (adc > 490 && adc < 502)
    {
     temper=(float)(26)+(502-adc)*0.09;
    };    
    
  if (adc > 501 && adc < 512)
    {
     temper=(float)(25)+(512-adc)*0.10;
    };    
    
  if (adc > 511 && adc < 524)
    {
     temper=(float)(24)+(524-adc)*0.08;
    };    
    
  if (adc > 523 && adc < 536)
    {
     temper=(23)+(536-adc)*0.08;
    };    
    
  if (adc > 535 && adc < 548)
    {
     temper=(22)+(548-adc)*0.09;
    };
    
  if (adc > 547 && adc < 558)
    {
     temper=(21)+(558-adc)*0.09;
    };    
    
  if (adc > 557 && adc < 569)
    {
     temper=(20)+(569-adc)*0.10;
    };    
    
  if (adc > 568 && adc < 581)
    {
     temper=(19)+(581-adc)*0.08;
    };    
    
  if (adc > 580 && adc < 593)
    {
     temper=(18)+(593-adc)*0.08;
    };    
    
  if (adc > 592 && adc < 605)
    {
     temper=(17)+(605-adc)*0.09;
    };    
    
  if (adc > 604 && adc < 615)
    {
     temper=(16)+(615-adc)*0.09;
    };    
    
  if (adc > 614 && adc < 628)
    {
     temper=(15)+(626-adc)*0.10;
    };    
    
  if (adc > 627 && adc < 638)
    {
     temper=(14)+(638-adc)*0.08;
    };    
    
  if (adc > 637 && adc < 650)
    {
     temper=(13)+(650-adc)*0.08;
    };    
    
  if (adc > 649 && adc < 661)
    {
     temper=(12)+(661-adc)*0.09;
    };    
    
  if (adc > 660 && adc < 671)
    {
     temper=(11)+(671-adc)*0.10;
    };    
    
  if (adc > 670 && adc < 681)
    {
     temper=(10)+(681-adc)*0.10;
    };    
    
  if (adc > 680 && adc < 734)
    {
     temper=(5)+(734-adc)*0.09;
    };    
    
  if (adc > 733 && adc < 783)
    {
     temper=(0)+(783-adc)*0.10;
    };    
    
  if (adc > 782 && adc < 828)
    {
     temper=(-5)+(828-adc)*0.11;
    };    
    
  if (adc > 827 && adc < 867)
    {
     temper=(-10)+(867-adc)*0.13;
    };    
    
  if (adc > 866 && adc < 900)
    {
     temper=(-15)+(900-adc)*0.15;
    };    
    
  if (adc > 899 && adc < 928)
    {
     temper=(-20)+(928-adc)*0.18;
    };    
    
  if (adc > 927 && adc < 950)
    {
     temper=(-25)+(950-adc)*0.22;
    };    
    
  if (adc > 949)
    {
      avariya_datchik_ne_podkluchen=1;
    };
  return temper;                                         
} 
// Timer 0 ������ 3,2 ��
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
TCNT0=0x9C;

   if(show_temp==1)
     {
       if(buffer<100)
        switch(i) 
          {
           case 0:{PORTD=t1;            PORTB.0=0; PORTB.6=0; PORTB.7=1; break;}
           case 1:{PORTD=t2; PORTD.0=0; PORTB.0=0; PORTB.6=1; PORTB.7=0; break;}
           case 2:{PORTD=t3;            PORTB.0=1; PORTB.6=0; PORTB.7=0; break;}
          }
        if(buffer>99)
         switch(i) 
           {
            case 0:{PORTD=t1; PORTB.0=0; PORTB.6=0; PORTB.7=1; break;}
            case 1:{PORTD=t2; PORTB.0=0; PORTB.6=1; PORTB.7=0; break;}
            case 2:{PORTD=t3; PORTB.0=1; PORTB.6=0; PORTB.7=0; break;}
           }        
          
     };
   if(show_termostat==1 | show_ustavku==1 | par_ust==1 | show_par==1)
     {     

      if (t3==0x05 & exit==0 & P03==0)
       {
       t3=0xFF;
       };
      if(flag_minus==1)
       {
       t3=PORTD.2=0b11111011;
       };       
      
      switch(i) 
            {
            case 0:{PORTD=t1; PORTB.0=0; PORTB.6=0; PORTB.7=1; break;}
            case 1:{PORTD=t2; if(screen==24){PORTD.0=0;}; PORTB.0=0; PORTB.6=1; PORTB.7=0; break;}
            case 2:{PORTD=t3; PORTB.0=1; PORTB.6=0; PORTB.7=0; break;}
            }
     };     

   
   i++;
   if(i>2)
   i=0;
}

// Timer1 ������ 10 ��
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
TCNT1H=0xD8F0 >> 8;
TCNT1L=0xD8F0 & 0xff;
wait_timer_for_termostat++;
wait_timer1++;
buzzer++;

if(wait_timer1==5 || wait_timer1==10 || wait_timer1==15 || wait_timer1==20)         // ������ 50 �� - �������������
  {
    if (show_temp==1)
      { 
        buffer=readtemp();
        if(buffer<100)
          { 
            currtemp=10*readtemp();
            getkodes(currtemp);
          }
        if(buffer>99)
          {
            currtemp=readtemp();
            getkodes(currtemp);
          }
      }
      
    if (show_termostat==1)
      {  
        if(flag_pereklu4==1)
          {
           getkodes(P6);
           buffer=readtemp();
          };
        if(flag_pereklu4==0)
          {
           getkodes(readtemp());
           buffer=readtemp();
          };
      }
    if (show_ustavku==1)
      {
       t1=kod[12]; t2=kod[11]; t3=kod[10];    // USt
      };
    if (par_ust==1)
      {
       if(P00==1)
         {
          t1=kod[0]; t2=kod[0]; t3=kod[13];
         };
       if(P01==1)
         {
          t1=kod[1]; t2=kod[0]; t3=kod[13];
         };  
       if(P02==1)
         {
          t1=kod[2]; t2=kod[0]; t3=kod[13];
         };
       if(P03==1)
         {
          t1=kod[3]; t2=kod[0]; t3=kod[13];
         };
       if(P04==1)
         {
          t1=kod[4]; t2=kod[0]; t3=kod[13];
         };
       if(P05==1)
         {
          t1=kod[5]; t2=kod[0]; t3=kod[13];
         };
       if(P06==1)
         {
          t1=kod[6]; t2=kod[0]; t3=kod[13];
         };
       if(exit==1)
         {
          t1=kod[12]; t2=kod[10]; t3=kod[0];
         };                                      
      };
    if (show_par==1)
      {   if(flag_minus==0)
            {
             getkodes(par);
            };
          if(flag_minus==1)
            {
            getkodes(minus);
            };
      }; 
 };
       
 
if(wait_timer1==10 || wait_timer1==20 && show_temp==1)         // ������ 100 �� - ����� ���.
  { 
   filterin=read_adc(5);
   adc=(coeff_beta * adc + filterin  * coeff_alpha) >> N; // ����������� ����� 1 �������
  };
  
  
if(wait_timer1==20)         // ������ 200 �� - ����� ������
  {
   if(PINB.1==0)
     {
      knopka_left=1;
     };
   if(PINB.2==0)
     {
      knopka_middle=1;
     };
   if(PINB.3==0)
     {
      knopka_right=1;
     };
   wait_timer1=0;     
  };               
  
if(wait_timer_for_termostat==255)
 { 
  flag_pereklu4= !flag_pereklu4;
  wait_timer_for_termostat=0;
 }

if(counter_buzzer<10 && flag_enter==1 && P3==1)
    { 
     if(buzzer==50)
      {
      PORTC.0=1;
      counter_buzzer++;
      }
     if(buzzer==100)
      {
      PORTC.0=0;
      counter_buzzer++;
      };
       
    };

if(buzzer>100) 
 {
  buzzer=0;
  PORTC.0=0;
 }
}

// Timer2 ������ 200 ���
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
// Reinitialize Timer2 value
TCNT2=0x38;
wait_timer2++;
switch(screen)                // ����
 {
       case 0:                            // ����� �����������
          {
           show_termostat=0;
           show_ustavku=0; 
           show_temp=1;
           if (knopka_left==1)
             { 
              screen++;
              knopka_left=0;
             };
           if (knopka_right==1)
             {
              screen--;
              knopka_right=0;
              if (screen<0)
                screen=2;
             };
           if (knopka_middle==1)
             knopka_middle=0; 
           break;
           };
       
       case 1:                            // ������ ����������
           {
            show_termostat=1;
            show_ustavku=0; 
            show_temp=0;
            if (knopka_left==1)
              { 
               screen++;
               knopka_left=0;
              };
            if (knopka_right==1)
              {
               screen--;
               knopka_right=0;
              };
             if (knopka_middle==1)
             knopka_middle=0;       
            break;
           };
       
       case 2:                            // ���� � ���� �������
           {
            show_termostat=0;
            show_ustavku=1; 
            show_temp=0;
            if (knopka_right==1)
              {
               screen--;
               knopka_right=0;
              }; 
            if (knopka_left==1)
              { 
               screen++;
               knopka_left=0;
               if(screen>2)
                 screen=0;
              };              
            if (knopka_middle==1)
             {
              screen=4;
              par_ust=1;
              knopka_middle=0;
             };
            break;
           };
       
       case 4:                            // ����� ������ (������\����������)
           {
            show_termostat=0;
            show_ustavku=0; 
            show_temp=0;
            exit=0;
            P00=1;
            P01=0;
            par_ust=1;
            if (knopka_right==1)
              {
               screen++;
               knopka_right=0;
              };
            if (knopka_left==1)
              { 
                screen--;
                if(screen<4)
                  screen=11;
                knopka_left=0;
              };              
            if (knopka_middle==1)
            {
             knopka_middle=0;
             screen=20;
            };  
            break;
           };
           
       case 5:                            // ����������� ������������. ������ �����
           {
            show_termostat=0;
            show_ustavku=0; 
            show_temp=0;
            P00=0;
            P01=1;
            P02=0;
            par_ust=1;
            if (knopka_left==1)
              { 
                screen--;
                knopka_left=0;
              };
            if (knopka_right==1)
              {
               screen++;
               knopka_right=0;
              };
            if (knopka_middle==1)
              {
               knopka_middle=0;
               screen=21;
              };  
            break;
           };
           
       case 6:                            // ����������� ������������. ������� �����.
           {
            show_termostat=0;
            show_ustavku=0; 
            show_temp=0;
            P01=0;
            P02=1;
            P03=0;
            par_ust=1;          
            if (knopka_left==1)
              { 
                screen--;
                knopka_left=0;
              };
            if (knopka_right==1)
              {
               screen++;
               knopka_right=0;
              };
            if (knopka_middle==1)
              {
               knopka_middle=0;
               screen=22;
              };  
            break;
           };
           
       case 7:                            // ���\���� ������
           {
            show_termostat=0;
            show_ustavku=0; 
            show_temp=0;
            P02=0;
            P03=1;
            P04=0;
            par_ust=1;           
            if (knopka_left==1)
              { 
                screen--;
                knopka_left=0;
              };
            if (knopka_right==1)
              {
               screen++;
               knopka_right=0;
              };
            if (knopka_middle==1)
            {
             knopka_middle=0;
             screen=23;
            };  
            break;
           };
                                            
       case 8:                            // ��������� �����������
           {
            show_termostat=0;
            show_ustavku=0; 
            show_temp=0;
            P03=0;
            P04=1;
            P05=0;
            par_ust=1;
            if (knopka_left==1)
              { 
                screen--;
                knopka_left=0;
              };
            if (knopka_right==1)
              {
               screen++;
               knopka_right=0;
              };
            if (knopka_middle==1)
              {
               knopka_middle=0;
               screen=24;
              };  
            break;
           };
           
       case 9:                            // ����������
           {
            show_termostat=0;
            show_ustavku=0; 
            show_temp=0;
            P04=0;
            P05=1;
            P06=0;
            par_ust=1;
            if (knopka_left==1)
              { 
                screen--;
                knopka_left=0;
              };
            if (knopka_right==1)
              {
               screen++;
               knopka_right=0;
              };
            if (knopka_middle==1)
              {
               knopka_middle=0;
               screen=25;
              };  
            break;
           };           

       case 10:                            // ������� �����������
           {
            show_termostat=0;
            show_ustavku=0; 
            show_temp=0;
            P05=0;
            P06=1;
            exit=0;
            par_ust=1;
            if (knopka_left==1)
              { 
                screen--;
                knopka_left=0;
              };
            if (knopka_right==1)
              {
               screen++;
               knopka_right=0;
              };
            if (knopka_middle==1)
              {
               knopka_middle=0;
               screen=26;
              };  
            break;
           };
           
       case 11:                            // ����������� �����
           {
            show_termostat=0;
            show_ustavku=0; 
            show_temp=0;
            par_ust=1;
            P06=0;
            exit=1;
            P00=0;
            if (knopka_left==1)
              { 
                screen--;
                knopka_left=0;
              };
            if (knopka_right==1)
              {
               screen++; 
               if(screen>11)
                 screen=4;               
               knopka_right=0;
              };
            if (knopka_middle==1)
             {
              show_termostat=0;
              show_ustavku=0; 
              show_temp=1;
              par_ust=0;
              screen=0;
              exit=0;
              knopka_middle=0;
             };  
            break;
           };
           
       case 20:                            // ����� ������
           {
            par_ust=0;
            show_par=1;
            if(P0==1)                      // HOT
              {
               t1=kod[12]; 
               t2=kod[15]; 
               t3=kod[17];
              }
            else                           // COLD
              {
               t1=kod[16]; 
               t2=kod[15]; 
               t3=kod[14];
              };
            if(knopka_left==1)             
              {
               knopka_left=0;
               P0=0;
              };
            if(knopka_right==1)
              {
               knopka_right=0;
               P0=1;
              };
            if(knopka_middle==1)
              {
               knopka_middle=0;
               screen=4;
               P00=0;
               show_par=0;
              };
            break;    
           };
           
       case 21:                            // ������ ����� ����������� ������������
           {
            par_ust=0;
            show_par=1;
             if(flag_enter==0)
             {
              par=P1;
              flag_enter=1;
             }
            if(knopka_left==1)             
              {
               knopka_left=0;
               par-=5;
               if (par < 0)
                  par = 150;  
              };
            if(knopka_right==1)
              {
               knopka_right=0;
               par+=5;
               if (par>150)
                  par = 0; 
              };
            if(knopka_middle==1)
              {
               knopka_middle=0;
               screen=5;
               P01=0;
               show_par=0;
               flag_enter=0;
               P1=par;
              };    
            break;           
           };
           
       case 22:                            // ������� ����� ����������� ������������
           {
            par_ust=0;
            show_par=1;
            if(flag_enter==0)
             {
              par=P2;
              flag_enter=1;
             }
            if(knopka_left==1)             
              {
               knopka_left=0;
               par-=5;
               if (par < 0)
                  par = 150;  
              };
            if(knopka_right==1)
              {
               knopka_right=0;
               par+=5;
               if (par>150)
                  par = 0; 
              };
            if(knopka_middle==1)
              {
               knopka_middle=0;
               screen=6;
               P02=0;
               show_par=0;
               flag_enter=0;
               P2=par;
              }; 
            break;           
           };                                                    
      
       case 23:                            // ���\���� ������
           {
            par_ust=0;
            show_par=1;
            if(P3==1)
              {
               t1=0xFF; 
               t2=kod[18]; 
               t3=kod[0];
              };
            
            if(P3==0)
              {
               t1=kod[19]; 
               t2=kod[19]; 
               t3=kod[0];
              };  
            if(knopka_left==1)             
              {
               knopka_left=0;
               P3=1;
              };
            if(knopka_right==1)
              {
               knopka_right=0;
               P3=0;
              };
            if(knopka_middle==1)
              {
               knopka_middle=0;
               screen=7;
               P03=0;
               show_par=0;
              };
            break;           
           };
           
       case 24:                            // ��������� �����������
           {
            par_ust=0;
            show_par=1;
            if(flag_enter==0)
             {
              par=P4;
              flag_enter=1;
              minus=par;
             }
            if(knopka_left==1)             
             {
              knopka_left=0;
              par--;
              minus=par;
              if(par < 0)
                {
                flag_minus=1;
                minus=(-par);
                 if (par<-90)
                   {
                   par=90;
                   minus=par;
                   };
                   
                }
             };
            if(knopka_right==1)
             {
              knopka_right=0;
              par++;
              minus=par;
              if(par<0)
                {
                 flag_minus=1;
                 minus=-par;
                };
              if(par>90)
                {
                 par=-90;
                 minus=-par;
                 flag_minus=1;
                };
             };
            if(par>0)
              flag_minus=0;
            if(knopka_middle==1)
             {
              knopka_middle=0;
              screen=8;
              P04=0;
              show_par=0;
              flag_enter=0;
              P4=par;
             };
            break;           
           };
           
       case 25:                            // ����������
           {
            par_ust=0;
            show_par=1;
            if(flag_enter==0)
              {
               par=P5;
               flag_enter=1;
              }
            if(knopka_left==1)             
              {
               knopka_left=0;
               par-=2;
               if(par<0)
                 par=10;
              };
            if(knopka_right==1)
              {
               knopka_right=0;
               par+=2;
               if(par>10)
                 par=0;
              };
            if(knopka_middle==1)
              {
               knopka_middle=0;
               screen=9;
               P05=0;
               show_par=0;
               flag_enter=0;
               P5=par/2;
              };
            break;           
           };
       
       case 26:                            // ������� �����������
           {
            par_ust=0;
            show_par=1;
            if(flag_enter==0)
              {
               par=P6;
               flag_enter=1;
              }
            if(knopka_left==1)             
              {
               knopka_left=0;
               par-=5;
               if(par<P1)
                 par=P2;
              };
            if(knopka_right==1)
              {
               knopka_right=0;
               par+=5;
               if(par>P2)
                 par=P1;
              };
            if(knopka_middle==1)
              {
               knopka_middle=0;
               screen=10;
               P01=0;
               show_par=0;
               flag_enter=0;
               P6=par;
              };
            break;           
           };                                
      }     
      
if(show_termostat==0)
 {
  PORTC.1=0;
  PORTC.2=0;
  PORTB.5=0;
 };
 
if(P0==0 & show_termostat==1)
  {
   if((buffer-P5)<P6)
     {
      PORTB.5=0;
      PORTC.2=0;
      flag_enter=0;
      PORTC.0=0;
     };
   if((buffer+P5)>P6)
     {
      PORTB.5=1;
      PORTC.2=1;
      if(flag_enter==0)
        {
         counter_buzzer=0;
         flag_enter=1;
        };
     };
  };
   
if(P0==1 & show_termostat==1)
  {
   if((buffer+P5)<P6)
     {
      PORTB.5=1;
      PORTC.1=1;
      if(flag_enter==0)
        {
         counter_buzzer=0;
         flag_enter=1;
        };
     };
   if((buffer-P5)>P6)
     {
      PORTB.5=0;
      PORTC.1=0;
      flag_enter=0;
      PORTC.0=0;
     }
  };                       
  
/*if(buzzer==50 && P3 == 1)  
  {                                         
     if(counter_buzzer<10 && flag_enter==1)
     {
      PORTC.0 = 1;
      counter_buzzer++;
     };
  };
  
if(buzzer==100 && P3 == 1)
  {
   if(counter_buzzer<10 && flag_enter==1)
     {
      PORTC.0 = 0;
      counter_buzzer++;
     };
  }; 
  if (counter_buzzer>10)
   {PORTC.0 = 0;};  */  
                                                      
}       








void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port B initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=In Bit3=In Bit2=In Bit1=In Bit0=Out 
DDRB=(1<<DDB7) | (1<<DDB6) | (1<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (1<<DDB0);
// State: Bit7=0 Bit6=0 Bit5=0 Bit4=T Bit3=P Bit2=P Bit1=P Bit0=0 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (1<<PORTB3) | (1<<PORTB2) | (1<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=Out Bit1=Out Bit0=Out 
DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=0 Bit1=0 Bit0=0 
PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);
// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 31,250 kHz
TCCR0=(1<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x9C;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 1000,000 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 10 ms
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
TCNT1H=0xD8;
TCNT1L=0xF0;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 1000,000 kHz
// Mode: Normal top=0xFF
// OC2 output: Disconnected
// Timer Period: 0,2 ms
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (1<<CS21) | (0<<CS20);
TCNT2=0x38;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (1<<TOIE0);

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);

// USART initialization
// USART disabled
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);

// ADC initialization
// ADC Clock frequency: 125,000 kHz
// ADC Voltage Reference: AVCC pin
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
SFIOR=(0<<ACME);

// Global enable interrupts
#asm("sei")

while (1)
      {
      }
}


