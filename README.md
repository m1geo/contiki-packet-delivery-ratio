contiki-packet-delivery-ratio
=============================

Simple Contiki code for multichannel PDR measurement

These two programs, Transmitter and Receiver, send packets in Contiki
from one node to another.  A fixed number of packets are sent and the
receiving node counts those successfully received. From this, the PDR
(packet delivery ratio) can be found. Nodes sequentially hop channels
repeating the transmission and counting to find the PDR in each chan.

Some crude time synchronisation is used to ensure nodes hop together.

This is valuable for assessing which channels may provide for better,
more reliable communications between two motes.

 - Use "make login | tee filename.dat" to save data to filename.dat
 - Use "./parse_data.sh filename.dat" to get the averages over runs

George Smart, M1GEO <george@george-smart.co.uk>

# Telecommunications Research Group Office, Room 804, 
# Roberts Buidling, Department of Electronic & Electrical Engineering
# University College London
# Malet Place, London, WC1E 7JE, United Kingdom.
