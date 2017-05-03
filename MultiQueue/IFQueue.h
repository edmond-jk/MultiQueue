/*
 * IFQueue.h
 *
 *  Created on: May 2, 2017
 *      Author: jk
 */

#ifndef IFQUEUE_H_
#define IFQUEUE_H_

#include <systemc.h>
#define	MAX_QUEUE_DEPTH	32 //32 x 32 Byte
#define CDB_SIZE	8 // 8 x 4Byte --> 32 Bytes

SC_MODULE(IF_QUEUE) {
	// producer --used by interface
	sc_in<bool> 	clock_datain;
	sc_in<bool> 	load_datain;
	sc_in<bool>		reset;
	sc_in<sc_uint<32> > 	datain;

	// consumer --used by tx controller
	sc_in<bool>		load_dataout;
	sc_out<sc_uint<32> >	dataout;

	sc_uint<8>	head, tail;
	sc_uint<32>	buffer[256];
	void core_process(void);

	SC_CTOR(IF_QUEUE) {
		SC_METHOD(core_process);
		dont_initialize();
		sensitive << clock_datain.pos() << clock_datain.neg();
	}
};

void IF_QUEUE::core_process(void)
{
	if(reset.read() == 1)
	{
		head = tail = 0;
		cout << "Init @" << sc_time_stamp() << ", head: " << head <<", tail: " << tail << endl;
	}
	else if(load_datain.read() == 1)
	{
		buffer[head*CDB_SIZE] = datain;

		head++;
		if(head == MAX_QUEUE_DEPTH)
		{
			head = 0;
		}
	}
	else if (load_dataout.read() == 1)
	{
		if(tail != head)
		{
			dataout.write(buffer[tail*CDB_SIZE]);

			tail++;
			if(tail == MAX_QUEUE_DEPTH)
			{
				tail = 0;
			}
		}
	}
}

#endif /* IFQUEUE_H_ */
