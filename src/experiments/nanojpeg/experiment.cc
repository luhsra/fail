#include <iostream>

// getpid
#include <sys/types.h>
#include <unistd.h>

#include "util/Logger.hpp"

#include "experiment.hpp"
#include "experimentInfo.hpp"
//#include "campaign.hpp"

#include "sal/SALConfig.hpp"
#include "sal/SALInst.hpp"
#include "sal/Memory.hpp"
#include "sal/Listener.hpp"
#include "sal/Register.hpp"

// you need to have the tracing plugin enabled for this
#include "../plugins/tracing/TracingPlugin.hpp"

#define LOCAL 1

using namespace std;
using namespace fail;

// Check if configuration dependencies are satisfied:
//#if !defined(CONFIG_EVENT_BREAKPOINTS) || !defined(CONFIG_SR_RESTORE) || \
//    !defined(CONFIG_SR_SAVE) || !defined(CONFIG_EVENT_TRAP)
//  #error This experiment needs: breakpoints, traps, save, and restore. Enable these in the configuration.
//#endif

bool NanoJPEGExperiment::run()
{
	char const *statename = "bochs.state"; // WEATHER_SUFFIX;
	Logger log("nJPEG", true);
	
	log << "startup" << endl;

#if 1
	// STEP 1: run until main starts, save state, record trace
	// TODO: store golden run output
	IOPortListener io(0x3f8, true);
	simulator.addListenerAndResume(&io);

	log << "main() reached, saving state" << endl;
	simulator.save(statename);

	// record trace
	log << "restoring state" << endl;
	simulator.restore(statename);
	log << "EIP = " << hex << simulator.getRegisterManager().getInstructionPointer() << endl;
	log << "enabling tracing" << endl;
	TracingPlugin tp;
	tp.setLogIPOnly(true);
	ofstream of(NANOJPEG_TRACE);
	tp.setTraceFile(&of);
	// this must be done *after* configuring the plugin:
	simulator.addFlow(&tp);

	// count instructions
	simulator.addListener(&io);
	BPSingleListener step(ANY_ADDR);
	long counter = 0;
	while (true) {
		BaseListener *l = simulator.addListenerAndResume(&step);
		if (l == &io) {
			break;
		}
		counter++;
	}
	log << "golden run took " << dec << counter << " instructions" << endl;
	simulator.removeFlow(&tp);
	of.close();
#endif
	// Explicitly terminate, or the simulator will continue to run.
	simulator.terminate();
}
