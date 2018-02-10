#ifndef CALTRAIN_H_INCLUDED
#define CALTRAIN_H_INCLUDED

#include <pthread.h>

struct station {
	pthread_mutex_t train;
	pthread_cond_t passenger_enter;
	int passenger_num;
	int going_on_board_num;
	int empty_seats;
	pthread_cond_t arrived;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);

#endif // CALTRAIN_H_INCLUDED
