#include<stdio.h>
#include<stdlib.h>

#include<uart.h>

unsigned long start,end, total;

unsigned long read_cycles(void)
{
 unsigned long cycles;
  asm volatile  ("rdcycle %0" : "=r" (cycles));
  return cycles;
}
int main()
{
    int angle,angleOri;
    float xin,yin,xout,yout;
    xin=1;
    yin=0;

    int posArr[9]={256,128,64,32,16,8,4,2,1};

    float sinlookup[9]={-0.9702957263,0.7880107536,0.8987940463,0.5299192642,0.2756373558,0.139173101,0.069755647374,0.0348994967,0.01745240644};

    float coslookup[9]={-0.2419218956,-0.6156614753,0.4383711468,0.8480480962,0.9612616959,0.9902680687,0.9975640503,0.999390827,0.9998476952};

    float sinval[361], cosval[361];
    int i = 8;
    start= read_cycles();
  
    for (angleOri=1;angleOri<360; angleOri++) {
            
            int resDiv=angleOri/posArr[i];
            int l;
            
            if(i==8){
                l=8;
            }
            else{
                l = i+1;
            }

            int resMod=angleOri%posArr[l];
            
            if(resDiv==1 && angleOri<=256) {
                sinval[angleOri] = sinlookup[i];
                cosval[angleOri] = coslookup[i];
                printf("\nCos value of %d is %f", angleOri,coslookup[i]);
                printf("\nSin value of %d is %f", angleOri,sinlookup[i]);
                if(posArr[i]!=256){
                    i--;
                }
            }
            else {
                if(angleOri<256) {
                    yin=sinlookup[i+1];
                    xin=coslookup[i+1];
                }
                else {
                    yin=sinlookup[i];
                    xin=coslookup[i];
                }   
                float sinvalue = sinval[resMod]; 
                float cosvalue = cosval[resMod];

                xout=(xin*cosvalue)-(yin*sinvalue);
                yout=(xin*sinvalue)+(yin*cosvalue);

                cosval[angleOri] = xout;
                sinval[angleOri] = yout;
                end= read_cycles();
                total = (double)(end - start);
                printf("Total time taken on computation: %lu\n", total);

                printf("\nCos value of %d is %f", angleOri,xout);
                printf("\nSin value of %d is %f\n", angleOri,yout);
            }
        }
        return 0;
	}
