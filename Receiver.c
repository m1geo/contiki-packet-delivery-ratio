/*
 * Receiver.c
 * 
 * Program which receives packets and prints the packet number.
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
int packetsRXed = 0;
int packetsRXedLast = 0;

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
  packetsRXed++;
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

PROCESS_THREAD(example_broadcast_process, ev, data)
{
  static struct etimer et;
  
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  broadcast_open(&broadcast, 129, &broadcast_call);

while (1) {

	cc2420_set_channel(channel);
	NETSTACK_MAC.on();
	printf("NETSTACK_MAC.on() at channel %u. Ready. \n", channel);

	while(1) {
		if (packetsRXedLast == packetsRXed) {
			break;
		}
		leds_on(LEDS_BLUE);
		packetsRXedLast = packetsRXed;
		printf("pRX = %d to %d on channel %u\n", packetsRXed, packetsRXedLast, channel);	
	
		leds_off(LEDS_BLUE);
		
		/* Delay 2-4 seconds */
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	}
	
	// if we haven't heard anything here, then wait.
	if (packetsRXed>0) {
		printf("*** RESULT FOR CH %u = %d\n", channel, packetsRXed);
		// reset packet count
		packetsRXed = 0;
		packetsRXedLast = 0;
		// next channel
		if (channel >= 26) {
			//break;
			channel = 11;
		} else {
			channel ++;
		}
	}
	etimer_set(&et, 1*CLOCK_SECOND);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  leds_on(LEDS_ALL);

  PROCESS_END();
}
