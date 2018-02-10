#include <pthread.h>
#include "caltrain.h"
#include "stdio.h"


void station_init(struct station *station)
{
    // initialize mutex and variable conditions.
	pthread_mutex_init(&(station->train), NULL);
	pthread_cond_init(&(station->arrived), NULL);
	pthread_cond_init(&(station->passenger_enter), NULL);
	// initialize empty seats
	// and passenger going on board
	// and passenger number on station.
	station->passenger_num = 0;
	station->empty_seats = 0;
	station->going_on_board_num = 0;
}

void station_load_train(struct station *station, int count)
{
    // set number of seats available.
    station->empty_seats = count;
    // let's begin a critical section.
    pthread_mutex_lock(&station->train);
    /** start of critical section **/
       /** ---- watch out ---- **/
    /** announcing the train enters the station to all passengers **/
    pthread_cond_broadcast(&(station->arrived));
    // check if there is any passenger or there is an empty seat for a passenger.
    // if there is then it's time for the train to wait for last passenger to enter the train.
    if(station->passenger_num > 0 && station->empty_seats > 0)
    {
        /** the driver is waiting for conductor to tell him to leave the station. **/
        pthread_cond_wait(&(station->passenger_enter), &(station->train));
    }
	/** end of critical section **/
	pthread_mutex_unlock(&station->train);
	// set number of seats to zero
	// to be ready when new passengers come to station when no train is there.
	station->empty_seats = 0;

}

void station_wait_for_train(struct station *station)
{
    // let's begin a critical section.
	pthread_mutex_lock(&station->train);
	/** start of critical section **/
	   /** ---- watch out ---- **/
	// a new passenger enters the station.
    station->passenger_num++;
    // check for an empty seat for the passenger to go on board.
    // if there is no seat then wait for another train.
    while(station->empty_seats == station->going_on_board_num)
    {
        /** the driver waits for conductor to tell him to leave station **/
        pthread_cond_wait(&(station->arrived), &(station->train));
    }
    // the passenger find a seat so he will go on board.
    station->going_on_board_num++;
    // so number of passengers waiting in decreased by 1.
    station->passenger_num--;
    /** end of critical section **/
    pthread_mutex_unlock(&station->train);
}

void station_on_board(struct station *station)
{
    // let's begin a critical section.
    pthread_mutex_lock(&station->train);
    /** start of critical section **/
       /** ---- watch out ---- **/
    // the passenger finally sit down.
    station->going_on_board_num--;
    // there is an empty seat that is occupied.
    station->empty_seats--;
    /** end of critical section **/
    pthread_mutex_unlock(&station->train);
    // check if its time for train to leave.
    // if there is no seat available or there is no passengers waiting.
    if ((station->passenger_num == 0 && station->going_on_board_num == 0) || station->empty_seats == 0)
    {
        /** conductor tells the train driver to leave the station. **/
        pthread_cond_signal(&(station->passenger_enter));
    }
}
