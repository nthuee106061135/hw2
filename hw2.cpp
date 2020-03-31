#include "mbed.h"

Serial pc( USBTX, USBRX );

DigitalOut redLED(LED2);
DigitalOut greenLED(LED1);
DigitalIn Switch(SW3);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
AnalogOut Aout(DAC0_OUT);

AnalogIn Ain(A0);

float freq_data[1024];
float max_value;
int freq;
int count;
int detect[3] = {0};
int freq_display[3]={0};
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char table_dot[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xDF};

int sample = 1024;

void cal_freq();
void fre_to_dis(int f);

int main(){

   for (int i = 0; i < sample; i++)
   {
     Aout = Ain;
     freq_data[i] = Ain;
    wait(1. / sample);
   }

   for (int i = 0; i < sample; i++)
   {
     pc.printf("%1.3f\r\n",freq_data[i]);
   }

    for(int i = 1, count = 0; (i < sample && count <3); i++){

        if(freq_data[i] * freq_data[i - 1] == 0) detect[count++] = i;
        if(count >= 3){
           count = 0;
           max_value = sample / (detect[2] - detect[0]);
           if(freq < max_value){
               freq = max_value;
           }
        }
    }

    fre_to_dis(freq);

    while(1){
        for (int i = 0; i < sample; i++){
            Aout = freq_data[i];
            wait(0.01);
        }
        if(Switch == 0){      ////////////push botton   
            greenLED = 0;
            redLED = 1;
            for(int i = 2 ;i >= 0 ; i--){
                if(i == 0){
                    display = table_dot[freq_display[i]];
                    wait(1);
                }else if(i == 2 && freq_display[i] == 0){
                    display = 0;
                }else{
                    display = table[freq_display[i]];
                    wait(1);
                }
            }
        }else{
            redLED = 0;
            greenLED = 1;
            display = 0;
        }
    }
}

void fre_to_dis(int f){
    freq_display[2] = (f / 100)%10;
    freq_display[1] = (f / 10)%10;
    freq_display[0] = f %10;
}