/*
 * Transmitter.c
 * 
 * Program which sends packets to receiver node at full power on sequ.
 * channels to monitor how many packets are lost...
 * 
 * George Smart <g.smart@ee.ucl.ac.uk>
 * Yiannis Andreopoulos <iandreop@ee.ucl.ac.uk> (Supervisor)
 * 
 * Thursday 11 April 2013.
 * Copyright 2013
 * 
 * MIT Licence. See LICENCE file.
 * 
 */

unsigned short int channel = 11;  // 2.4 GHz PHY channels 11-26
const unsigned short int maxframes = 1000;  // Number of packets to send
static struct etimer et;
char buffer [50];
int n = 0;
int pNum = 0;



#include "contiki.h"
#include "net/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
#include "net/netstack.h"



PROCESS(example_broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&example_broadcast_process);

static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
  //printf("%s\n", from->u8[0], from->u8[1], (char *) packetbuf_dataptr());
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

PROCESS_THREAD(example_broadcast_process, ev, data)
{

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  broadcast_open(&broadcast, 129, &broadcast_call);

  while (1) {
	cc2420_set_channel(channel);
	NETSTACK_MAC.on();
	pNum = 0;
	printf("NETSTACK_MAC.on() at channel %u. Ready to TX %u frames.\n", channel, maxframes);

	for (pNum = 1; pNum <= maxframes; pNum++) {
		leds_on(LEDS_RED);
		n=sprintf(buffer, "Packet Number %04d on channel %u", pNum, channel);
		packetbuf_copyfrom(buffer, n);
		broadcast_send(&broadcast);
		leds_off(LEDS_RED);
		
		etimer_set(&et, 6);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	}
		
	// reset packet count
	pNum = 0;
	// next channel
	if (channel >= 26) {
		//break;
		channel = 11;
	} else {
		channel ++;
	}
	
	etimer_set(&et, 5*CLOCK_SECOND);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	
  }
  
  leds_on(LEDS_ALL);
  
  PROCESS_END();
  
}
