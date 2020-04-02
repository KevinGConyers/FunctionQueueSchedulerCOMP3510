
/*****************************************************************************\
 * Laboratory Exercises COMP 3510                                              *
 * Author: Saad Biaz                                                           *
 * Date  : April 1, 2014                                                   *
 \*****************************************************************************/

/*****************************************************************************\
 *                             Global system headers                           *
 \*****************************************************************************/


#include "common3.h"

/*****************************************************************************\
 *                             Global data types                               *
 \*****************************************************************************/



/*****************************************************************************\
 *                             Global definitions                              *
 \*****************************************************************************/
#define MAX_QUEUE_SIZE  2






/*****************************************************************************\
 *                            Global data structures                           *
 \*****************************************************************************/



/*****************************************************************************\
 *                                  Global data                                *
 \*****************************************************************************/
double EventsTotalPerDevice[MAX_NUMBER_DEVICES] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double EventsMissedPerDevice[MAX_NUMBER_DEVICES] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double EventsServedPerDevice[MAX_NUMBER_DEVICES] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double MissPercentagePerDevice[MAX_NUMBER_DEVICES] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double AvgTurnaroundTimePerDevice[MAX_NUMBER_DEVICES] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double AvgResponseTimePerDevice[MAX_NUMBER_DEVICES] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

Event DeviceQueues[MAX_NUMBER_DEVICES][MAX_QUEUE_SIZE];
int QueueSizes[MAX_NUMBER_DEVICES]  = {0, 0, 0, 0, 0, 0, 0, 0};
int NextStorage[MAX_NUMBER_DEVICES] = {0, 0, 0, 0, 0, 0, 0, 0};
int NextEvent[MAX_NUMBER_DEVICES] = {0, 0, 0, 0, 0, 0, 0, 0};

Status DeviceHasEventWaiting = 0;





/*****************************************************************************\
 *                               Function prototypes                           *
 \*****************************************************************************/

void Control(void);
void InterruptRoutineHandlerDevice(void);
void BookKeeping();
void EnqueueEvents(Status Flags);
void EnqueueEvent(Status *Flags);
void DequeueHighestPriorityEvent();




/*****************************************************************************\
 * function: main()                                                            *
 * usage:    Create an artificial environment for embedded systems. The parent *
 *           process is the "control" process while children process will gene-*
 *           generate events on devices                                        *
 ******************************************************************************** Inputs: ANSI flat C command line parameters                                 *
 * Output: None                                                                *
 *                                                                             *
 * INITIALIZE PROGRAM ENVIRONMENT                                              *
 * START CONTROL ROUTINE                                                       *
 \*****************************************************************************/

int main (int argc, char **argv) {

	if (Initialization(argc,argv)){
		Control();
	} 
} /* end of main function */



/***********************************************************************\
 * Input : none                                                          *
 * Output: None                                                          *
 * Function: Monitor Devices and process events (written by students)    *
 \***********************************************************************/
void Control(void){

	while (1) 
	{
		if (Flags > 0) 
		{
			EnqueueEvent(&Flags);
		}
		DequeueHighestPriorityEvent();
	}

}

void EnqueueEvents(Status deviceQueue) 
{
	int device_index = 0;


}

void EnqueueEvent(Status *Flags)
{
	unsigned int device_index = 0;
	while (device_index < MAX_NUMBER_DEVICES && !((*Flags) & (1 <<device_index))) {
		device_index++;
	}
	EventsTotalPerDevice[device_index] = EventsTotalPerDevice[device_index] + 1.0;
	Event eventToBeQueued = BufferLastEvent[device_index];
	if (QueueSizes[device_index] >= MAX_QUEUE_SIZE)
	{
		AvgResponseTimePerDevice[device_index] += Now() - eventToBeQueued.When;
		printf("Dropped Even for device: %d", device_index);
		//event dropped
	}
	else 
	{
		//printf("Enqueueing Event for device: %d, at position %d\n", device_index, device_index);
		AvgResponseTimePerDevice[device_index] += Now() - eventToBeQueued.When;
		DisplayEvent('+', &eventToBeQueued);
		DeviceQueues[device_index][NextStorage[device_index]] = eventToBeQueued;
		NextStorage[device_index] = (NextStorage[device_index] + 1) & (MAX_QUEUE_SIZE - 1); 

		QueueSizes[device_index]++;
//		printf("Enqueued Event for device: %d, at position %d\n", device_index, NextStorage[device_index]); 
	}
	(*Flags) = (*Flags) & ~(1<<device_index);

}

void DequeueHighestPriorityEvent()
{
	int device_index = 0;
	while(QueueSizes[device_index] == 0 && device_index < MAX_NUMBER_DEVICES) {
		device_index++;
	}
	if(device_index >= MAX_NUMBER_DEVICES) {

	} else 
	{

//		printf("Deqeueing Event for device: %d Postion: %d\n", device_index, NextEvent[device_index]);
		Server(&DeviceQueues[device_index][NextEvent[device_index]]);
		AvgTurnaroundTimePerDevice[device_index] += Now() - DeviceQueues[device_index][NextEvent[device_index]].When;
		EventsServedPerDevice[device_index] = EventsServedPerDevice[device_index] + 1.0;
		QueueSizes[device_index]--;
		if (QueueSizes[device_index] == 0) 
		{
			DeviceHasEventWaiting = DeviceHasEventWaiting & ~(1<<device_index); 
		}
		NextEvent[device_index] = (NextEvent[device_index] + 1) & (MAX_QUEUE_SIZE - 1);
	}
}


/***********************************************************************\
 * Input : None                                                          *
 * Output: None                                                          *
 * Function: This routine is run whenever an event occurs on a device    *
 *           The id of the device is encoded in the variable flag        *
 \***********************************************************************/
void InterruptRoutineHandlerDevice(void){
	printf("An event occured at %f  Flags = %d \n", Now(), Flags);
	EnqueueEvent(&Flags); //Enqueus the First Event in the Flag Array
}


/***********************************************************************\
 * Input : None                                                          *
 * Output: None                                                          *
 * Function: This must print out the number of Events buffered not yet   *
 *           not yet processed (Server() function not yet called)        *
 \***********************************************************************/
void BookKeeping(void){
	// For EACH device, print out the following metrics :
	// 1) the percentage of missed events, 2) the average response time, and 
	// 3) the average turnaround time.
	// Print the overall averages of the three metrics 1-3 above

	//Check for starved events

	int i =0;	
	for (i = 0; i < MAX_NUMBER_DEVICES; i++)
	{
		while(QueueSizes[i] > 0) 
		{
			AvgTurnaroundTimePerDevice[i] += Now() - DeviceQueues[i][NextEvent[i]].When;
			QueueSizes[i]--;
			NextEvent[i] = (NextEvent[i] + 1) & (MAX_QUEUE_SIZE - 1);

		}
	}


	int low = 0;
	for (i = 0; i < MAX_NUMBER_DEVICES; i++){
		if(EventsTotalPerDevice[i] > 0) 
		{
			low = i;

			EventsMissedPerDevice[i] = EventsTotalPerDevice[i] - EventsServedPerDevice[i]; 
			MissPercentagePerDevice[i] = EventsMissedPerDevice[i]/EventsTotalPerDevice[i];
			AvgResponseTimePerDevice[i] = AvgResponseTimePerDevice[i] / EventsTotalPerDevice[i]; 
			AvgTurnaroundTimePerDevice[i] = AvgTurnaroundTimePerDevice[i] / EventsTotalPerDevice[i]; 
		}
	}

	printf("Metrics for Device 0 (highest priority)\n\t+ Avg Response Time: %f\n\t+ Avg Turnaround Time: %f\n\t+ Miss Percentage: %f%\n", AvgResponseTimePerDevice[0], AvgTurnaroundTimePerDevice[0], MissPercentagePerDevice[0] * 100);
	printf("\t+ Total Events: %f\n\t+ Events Served: %f\n", EventsTotalPerDevice[0], EventsServedPerDevice[0]);
	printf("\n\t***\t\n");
	printf("Metrics for Device %d (lowest priority)\n\t+ Avg Response Time: %f\n\t+ Avg Turnaround Time: %f\n\t+ Miss Percentage: %f%\n", low, AvgResponseTimePerDevice[low], AvgTurnaroundTimePerDevice[low], MissPercentagePerDevice[low] * 100);
	printf("\t+ Total Events: %f\n\t+ Events Missed: %f\n", EventsTotalPerDevice[low], EventsMissedPerDevice[low]);

	printf("\n\t***\t\n");

	for ( i = 1; i < low; i++){

		printf("Metrics for Device %d (middle priority)\nAvg Response Time: %f\nAvg Turnaround Time: %f\nMiss Percentage: %f%\n", i, AvgResponseTimePerDevice[i], AvgTurnaroundTimePerDevice[i], MissPercentagePerDevice[i] * 100);
		printf("\n\t***\t\n");
	}
}






