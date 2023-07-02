/*
 ============================================================================
 Name        : Project.c
 Author      : Mohamed Nasser
 Version     : 2
 Copyright   :
 Description : Vehicle Control system
 ============================================================================
 */

/*
 * SYSTEM DESCRIPTION:
 * 1. This system doesn't call more than one function at a time inside main.
 * 2. Each function returns to the main before calling any one else being called due to memory management.
 */

#include <stdio.h>
#include <stdlib.h>
//For Bonus Section
#define WITH_ENGINE_TEMP_CONTROLLER 1
//For returning pointer to function from some calling functions
typedef void(* returnPointerToFunc)(void);
typedef enum
{
	OFF,
	ON
}bool;

/*---------------------------Global Variables-----------------------------*/
char system_state = 0;
char menu_state = 0;
char trafficLight_color = 0;
short vehcile_speed = 40;
float temperature = 22;

#if WITH_ENGINE_TEMP_CONTROLLER
float engine_temp = 115;
bool engineTemp_controller = OFF;
#endif

bool engine_state = OFF;
bool AC_state = OFF;

/*---------------------------Functions Prototypes-----------------------------*/
void startSystem (void);
void disp_sensorMenu (void);
//Function returning pointer to function
returnPointerToFunc control_sensorMenu (void);
void set_trafficLight (void);
void set_roomTemp (void);
void disp_vehcileState (void);

#if WITH_ENGINE_TEMP_CONTROLLER
void set_engineTemp (void);
#endif

/*-----------------------------Main Program-----------------------------------*/
int main(void)
{
	/*Buffer Settings*/
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	while(1)
	{
		startSystem(); //Print the system states

		/*
		 * Description:
		 * 1. Take the user's choice.
		 * 2. Determine the choice.
		 * 3. Take an action depending on the selected choice.
		 */
		do
		{
			printf("Enter A Valid Choice: ");
			scanf(" %c", &system_state);
			fflush(stdin);
		} while(system_state!='a' && system_state!='b' && system_state!='c');

		printf("\n");
		switch(system_state)
		{
		case 'a':
			engine_state = ON;
			printf("The Vehicle's Engine Turned On\n\n");
			break;

		case 'b':
			engine_state = OFF;
			printf("The Vehicle's Engine Turned Off\n\n");
			break;

		case'c':
			engine_state = OFF;
			printf("The System Turned Off\n\n");
			return 0;
		}
		while (engine_state == ON)
		{
			disp_sensorMenu();
			void (*func_ptr) (void) = control_sensorMenu();
			if (func_ptr != NULL)
			{
				(*func_ptr)();
				disp_vehcileState();
			}
		}
	}
	return 0;
}

/*---------------------------Functions Definitions Section-----------------------------*/

/*
 * Description:
 * 1. Prints the system choices.
 */
void startSystem (void)
{
	printf("a. Turn on the vehicle engine\n"
			"b. Turn off the vehicle engine\n"
			"c. Quit the system\n\n");
}

/*
 * Description:
 * 1. Prints the sensors choices.
 */
void disp_sensorMenu (void)
{
	printf("a. Turn off the engine\n"
			"b. Set the traffic light color\n"
			"c. Set the room temperature\n");
#if !WITH_ENGINE_TEMP_CONTROLLER
	printf("\n");
#endif
#if WITH_ENGINE_TEMP_CONTROLLER
	printf("d. Set the engine temperature\n\n");
#endif
}

/*
 * Description:
 * 1. Take the user's choice.
 * 2. Determine the choice.
 * 3. Call the sensor specific function depending on the choice.
 */
returnPointerToFunc control_sensorMenu (void)
{
	do
	{
		printf("Enter A Valid Choice: ");
		scanf(" %c", &menu_state);
		fflush(stdin);
		if (menu_state == 'd')
		{
#if !WITH_ENGINE_TEMP_CONTROLLER
			menu_state = 'k';
#endif
		}
	} while(menu_state!='a' && menu_state!='b' && menu_state!='c' && menu_state!='d');

	printf("\n");
	switch (menu_state)
	{
	case'a':
		printf("The Vehicle's Engine Turned Off\n\n");
		engine_state = OFF;
		AC_state = OFF;
#if WITH_ENGINE_TEMP_CONTROLLER
		engineTemp_controller = OFF;
#endif
		break;
	case'b':
		return set_trafficLight;
	case'c':
		return set_roomTemp;
#if WITH_ENGINE_TEMP_CONTROLLER
	case'd':
		return set_engineTemp;
#endif
	}
	return NULL;
}

/*
 * Description
 * 1. Takes the traffic light sensor reading.
 * 2. Determine the light.
 * 3. Set the vehicle speed depending on the light color.
 * 4. Apply the 30 Km/h speed conditions.
 * 5. Display vehicle state.
 */
void set_trafficLight (void)
{
	do
	{
		printf("Enter A Valid Traffic Light Color (G / O / R): ");
		scanf(" %c", &trafficLight_color);
		fflush(stdin);
	} while(trafficLight_color!='G' && trafficLight_color!='O' && trafficLight_color!='R');

	printf("\n");
	switch (trafficLight_color)
	{
	case'G':
		vehcile_speed = 100;
		break;
	case'O':
		vehcile_speed = 30;
		break;
	case'R':
		vehcile_speed = 0;
	}
	if (30 == vehcile_speed)
	{
		if (AC_state == OFF)
		{
			AC_state = ON;
		}
		temperature = temperature * (5.0/4) + 1;

#if WITH_ENGINE_TEMP_CONTROLLER
		if (engineTemp_controller == OFF)
		{
			engineTemp_controller = ON;
		}
		engine_temp = engine_temp * (5.0/4) + 1;
#endif
	}
}

/*
 * Description
 * 1. Takes the vehicle temperature sensor reading.
 * 2. Determine the temperature.
 * 3. Set the AC state depending on the temperature.
 * 4. Display vehicle state.
 */
void set_roomTemp (void)
{
	do
	{
		printf("Enter A Logical Valid Temperature: ");
		scanf("%f", &temperature);
		printf("\n");
	} while (temperature < -40.0000000000000 || temperature > 65.0000000000000);

	if (temperature < 10.000000000)
	{
		AC_state = ON;
		temperature = 20;
	}
	else if (temperature > 30.00000000000000)
	{
		AC_state = ON;
		temperature = 20;
	}
	else
	{
		AC_state = OFF;
	}
}

/*
 * Description
 * 1. Takes the vehicle engine temperature sensor reading.
 * 2. Determine the temperature.
 * 3. Set the engine temperature controller depending on the temperature.
 * 4. Display vehicle state.
 */
#if WITH_ENGINE_TEMP_CONTROLLER
void set_engineTemp (void)
{
	do
	{
		printf("Enter A Logical Valid Engine Temperature: ");
		scanf("%f", &engine_temp);
		printf("\n");
	} while (engine_temp < 20.000000000 || engine_temp > 250.0000000000000000);

	if (engine_temp < 100.0000000000)
	{
		engineTemp_controller = ON;
		engine_temp = 125;
	}
	else if (engine_temp > 150.000000000)
	{
		engineTemp_controller = ON;
		engine_temp = 125;
	}
	else
	{
		engineTemp_controller = OFF;
	}
}
#endif

/*
 * Description
 * 1. Display the vehicle state
 */
void disp_vehcileState (void)
{
	if (ON == engine_state)
		printf("Engine state: ON\n");
	else
		printf("Engine state: OFF\n");
	if(ON == AC_state)
		printf("AC: ON\n");
	else
		printf("AC: OFF\n");
	printf("Vehicle Speed: %hd Km/h\n"
			"Room Temperature: %.1f C\n", vehcile_speed, temperature);
#if !WITH_ENGINE_TEMP_CONTROLLER
	printf("\n");
#endif
#if WITH_ENGINE_TEMP_CONTROLLER
	if(ON == engineTemp_controller)
		printf("Engine Temperature Controller State: ON\n");
	else
		printf("Engine Temperature Controller State: OFF\n");
	printf("Engine Temperature: %.1f C\n\n", engine_temp);
#endif
}
