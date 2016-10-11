/* chill.c
 *
 * This program takes 0, 1, or two arguments and displays windchill calculations
 * No arguments displays a table with temp's from -10 to 40 by 10s and windspeeds from 5 to 15 by 5's
 * One argument is treated like a temp, and creates a table with windspeeds from 5 to 15 by 5's
 * Two arguments are temp then wind speed, and chill.c outputs one windchill calculation
 *
 *
 * Created by Andy Werchniak, July 2016
 */
 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double calcWC(double T, double V);

/* main
 * checks arguments for validity and outputs appropriate windchill calculation
 * arguments: argc(# of arguments from command line), argv (array of arguments from command line)
 * returns: 0 if all is well (see README.txt)
 */
int main(const int argc, const char *argv[])
{
	/* Declare variables*/
	double T, V;
	int vTable, tTable;
	
	/* Do different things depending on number of arguments */
	switch(argc) {
		case 1: //no arguments = table of temps from -10 to 40 by 10's and speeds from 5-15 by 5's
			printf("Temp\tWind\tChill\n");
			printf("-----\t-----\t-----\t\n");
			for(tTable=-10;tTable<=40;tTable+=10){
				for(vTable=5;vTable<=15;vTable+=5){
					printf("%d\t%d\t%.1f\n", tTable,vTable,calcWC((double)tTable,(double)vTable));
				}
				printf("\n");
			}
			break;
		case 2: 					//one argument = temperature. table with speeds from 5-15 by 5's
			T=atof(argv[1]);		//temperature is the command line argument
			if(T<50){				//also verify the temperature constraint
				printf("Temp\tWind\tChill\n");
				printf("-----\t-----\t-----\t\n");
				for(vTable=5;vTable<=15;vTable+=5){
					printf("%.0f\t%d\t%.1f\n", T,vTable,calcWC(T,(double)vTable));
				}
			} else{
				printf("Please enter a Temperature value less than 50 degrees.\n");
				exit(2);
			}
			break;
		case 3: 					//two arguments = simply output one windchill
			T= atof(argv[1]);		//temp is the first argument, velocity is the second
			V = atof(argv[2]);
			if(T<50 && V>=0.5){		//verify temp & velocity constraints
				printf("Temp\tWind\tChill\n");
				printf("-----\t-----\t-----\t\n");
				printf("%.0f\t%.0f\t%.1f\n", T,V,calcWC(T,V));
			} else if (V<0.5 || T>=50){
				printf("Please enter a Velocity greater than or equal to 0.5mph ");
				printf("and a velocity less than 50 degrees.\n");
				exit(2);
			}
			break;
		default: 					//otherwise: invalid number of arguments!!
			printf("Error! Please input 0, 1, or 2 arguments \n.");
			exit(1);
	}
	
	return(0);
}

/* calcWC
 * calculates windchill from input temperature and velocity
 * inputs: T (temperature), V (velocity)
 * returns: windchill (float)
 */
double calcWC(double T, double V)
{
	return 35.74 + 0.6215*T - 35.75*pow(V, 0.16) + 0.4275*T*pow(V, 0.16);
}