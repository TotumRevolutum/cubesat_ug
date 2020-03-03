#ifndef _MQ2_H
#define _MQ2_H

#include <avr/io.h>
#include "adc.h"
#include "math.h"
#include <util/delay.h>

#define         RL_VALUE                     (5)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //(Sensor resistance in clean air)/RO,
                                                     //which is derived from the chart in datasheet

#define         LPG                      (0)         // Gas identity no.
#define         SMOKE                    (1)


//float           LPGCurve[3]  = {2.3,0.20,-0.45};   //two points from LPG curve are taken point1:(200,1.6) point2(10000,0.26)
                                                    //take log of each point (lg200, lg 1.6)=(2.3,0.20)  (lg10000,lg0.26)=(4,-0.58)
                                                    //find the slope using these points. take point1 as reference
                                                    //data format:{ x, y, slope};

//float           SmokeCurve[3] ={2.3,0.53,-0.43};    //two points from smoke curve are taken point1:(200,3.4) point2(10000,0.62)
                                                    //take log of each point (lg200, lg3.4)=(2.3,0.53)  (lg10000,lg0.63)=(4,-0.20)
                                                    //find the slope using these points. take point1 as reference
                                                    //data format:{ x, y, slope};

//float           Ro           =  10;                 //Ro is initialized to 10 kilo ohms


 int  GetPercentage(float rs_ro_ratio, float *pcurve);
 int GetGasPercentage(float rs_ro_ratio, int gas_id);
 float ReadSensor();
 float ResistanceCalculation(int raw_adc);
 float SensorCalibration();


#endif
