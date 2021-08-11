# Ring
  This program creates a ring of N processes that send a signal round
  and round cnt times. It is invoked with ./ring N leader cnt where N
  is the number of processes it creates, leader is the process ID of the
  leader process, and cnt is the number of signals sent by every process
