#ifndef __pace_sender_h_
#define __pace_sender_h_

#include "alr_detector.h"
#include "pacer_queue.h"

typedef void(*pace_send_func)(void* handler, uint32_t seq, int retrans, size_t size);

typedef struct
{
	uint32_t			min_sender_bitrate_kpbs;
	uint32_t			estimated_bitrate;
	uint32_t			pacing_bitrate_kpbs;

	int64_t				last_update_ts;
	int64_t				first_sent_ts;

	alr_detector_t*		alr;
	pacer_queue_t		que;

	interval_budget_t	media_budget;

	/*�����ص�����*/
	void*				handler;
	pace_send_func		send_cb;
}pace_sender_t;

pace_sender_t*			pace_create(void* handler, pace_send_func send_cb);
void					pace_destroy(pace_sender_t* pace);

void					pace_set_estimate_bitrate(pace_sender_t* pace, uint32_t bitrate_pbs);
void					pace_set_bitrate_limits(pace_sender_t* pace, uint32_t min_sent_bitrate);

void					pace_insert_packet(pace_sender_t* pace, uint32_t seq, int retrans, size_t size, int64_t now_ts);

int64_t					pace_queue_ms(pace_sender_t* pace);
size_t					pace_queue_size(pace_sender_t* pace);

/*Ԥ�Ʒ��͵�queue�����ݵ�ʱ��*/
int64_t					pace_expected_queue_ms(pace_sender_t* pace);

int64_t					pace_get_limited_start_time(pace_sender_t* pace);

/*���Է���*/
void					pace_try_transmit(pace_sender_t* pace, int64_t now_ts);

#endif

