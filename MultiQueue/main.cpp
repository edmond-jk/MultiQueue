#include "IFQueue.h"

int sc_main(int argc, char**argv)
{
	sc_clock clock("my_clock", 1, SC_NS, 0.5);
	sc_signal<bool> reset;
	sc_signal<bool> enable_a;
	sc_signal<bool> enable_b;
	sc_signal<sc_uint<32> > datain;
	sc_signal<sc_uint<32> > dataout;
	int i;

	IF_QUEUE	IFQ("IF QUEUE");
	IFQ.clock_datain(clock);
	IFQ.load_datain(enable_a);
	IFQ.load_dataout(enable_b);
	IFQ.reset(reset);
	IFQ.datain(datain);
	IFQ.dataout(dataout);

	sc_start(0, SC_NS);

	sc_trace_file *wf = sc_create_vcd_trace_file("queue_activity");
	sc_trace(wf, clock, "clock");
	sc_trace(wf, reset, "reset");
	sc_trace(wf, enable_a, "enable_a");
	sc_trace(wf, enable_b, "enable_b");
	sc_trace(wf, datain, "datain");
	sc_trace(wf, dataout, "dataout");

	reset = 1;
	enable_a = 0;
	enable_b = 0;

	sc_start(1, SC_NS);
	cout << "@" << sc_time_stamp() << ", dataout: " << dataout << endl;

	for (i = 0; i < MAX_QUEUE_DEPTH; i++)
	{
		reset = 0;
		enable_a = 1;
		enable_b = 0;
		datain = 1000 + i;

		sc_start(0.5, SC_NS);

		reset = 0;
		enable_a = 0;
		enable_b = 1;
		sc_start(0.5, SC_NS);
		cout << "@" << sc_time_stamp() << ", dataout: " << dataout << endl;
	}

	return 0;

}
