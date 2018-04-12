#ifndef __sender_congestion_controller_h_
#define __sender_congestion_controller_h_

#include "feedback_adapter.h"
#include "delay_base_bwe.h"
#include "bitrate_controller.h"
#include "pace_sender.h"
#include "ack_bitrate_estimator.h"

typedef struct
{
	int							accepted_queue_ms;			/*��Ƶ�����ڷ���queue������ӳ�*/
	int							was_in_alr;
	delay_base_bwe_t*			bwe;						/*�����ӳٵĴ���������*/
	bitrate_controller_t*		bitrate_controller;			/*���ʿ������������bwe��ack rate��loss�����ۺ����ʵ���*/
	ack_bitrate_estimator_t*	ack;						/*Զ��ȷ���յ������ݴ���������*/
	pace_sender_t*				pacer;						/*���Ͷ˵Ĳ���������*/
	feedback_adapter_t			adapter;					/*����������Ϣ��������*/

}sender_cc_t;

sender_cc_t*					sender_cc_create(void* trigger, bitrate_changed_func bitrate_cb, void* handler, pace_send_func send_cb, int queue_ms);
void							sender_cc_destroy(sender_cc_t* cc);

void							sender_cc_heartbeat(sender_cc_t* cc, int64_t cur_ts);

/*packet_id�Ǳ�����ţ��൱��RTP��ͷ�е�SEQ*/
void							sender_cc_add_pace_packet(sender_cc_t* cc, uint32_t packet_id, int retrans, size_t size, int64_t now_ts);
/*�����seq��transport��������ID����ʹ���ط������IDҲ�ǲ�һ����*/
void							sender_on_send_packet(sender_cc_t* cc, uint16_t seq, size_t size);

void							sender_on_feedback(sender_cc_t* cc, bin_stream_t* strm);

void							sender_cc_update_rtt(sender_cc_t* cc, int32_t rtt);
void							sender_cc_set_bitrates(sender_cc_t* cc, uint32_t min_bitrate, uint32_t start_bitrate, uint32_t max_bitrate);

int64_t							sender_cc_get_pacer_queue_ms(sender_cc_t* cc);
int64_t							sender_cc_get_first_packet_ts(sender_cc_t* cc);


#endif 


