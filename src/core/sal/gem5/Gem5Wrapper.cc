#include "../SALInst.hpp"
#include "Gem5Wrapper.hpp"
#include "Gem5Breakpoint.hpp"

#include "sim/system.hh"
#include "mem/packet.hh"
#include "mem/physical.hh"

namespace fail {

// Register-/Memory-related:
regdata_t GetRegisterContent(System* sys, unsigned int id, RegisterType type, size_t idx)
{
	switch (type) {
	case RT_GP:
		if (idx == 15) {
			return sys->getThreadContext(id)->pcState().pc();
		}
		return sys->getThreadContext(id)->readIntReg(idx);
	case RT_FP: return sys->getThreadContext(id)->readFloatReg(idx); // FIXME: correct?! (FP <-> Float?!)
	case RT_ST: return sys->getThreadContext(id)->readMiscReg(idx);
	case RT_IP: return sys->getThreadContext(id)->pcState().pc();
	}
	// This shouldn't be reached if a valid register is passed
	assert(false && "FATAL ERROR: invalid register type (should never be reached)!");
	return 0;
}

void SetRegisterContent(System* sys, unsigned int id, RegisterType type, size_t idx,
                        regdata_t value)
{
	switch (type) {
	case RT_GP: sys->getThreadContext(id)->setIntReg(idx, value); break;
	case RT_FP: sys->getThreadContext(id)->setFloatReg(idx, value); break; // FIXME: correct?! (FP <-> Float?!)
	case RT_ST: sys->getThreadContext(id)->setMiscReg(idx, value); break;
	case RT_IP: sys->getThreadContext(id)->pcState().pc(static_cast<Addr>(value)); break;
	}
	// This shouldn't be reached if a valid register is passed
	assert(false && "FATAL ERROR: invalid register type (should never be reached)!");
}

void ReadMemory(System* sys, guest_address_t addr, size_t cnt, void *dest)
{
	Request req(addr, cnt, Request::PHYSICAL, 0);

	Packet pkt(&req, MemCmd::ReadReq);
	pkt.dataStatic(dest);

	sys->getPhysMem().functionalAccess(&pkt);
}

void WriteMemory(System* sys, guest_address_t addr, size_t cnt, void const *src)
{
	Request req(addr, cnt, Request::PHYSICAL, 0);

	Packet pkt(&req, MemCmd::WriteReq);
	pkt.dataStatic(src);

	sys->getPhysMem().functionalAccess(&pkt);
}

size_t GetPoolSize(System* sys) { return sys->getPhysMem().totalSize(); }

// Breakpoint-related:
void Gem5Breakpoint::process(ThreadContext *tc)
{
	fail::simulator.onBreakpoint(&fail::simulator.getCPU(tc->cpuId()), this->evpc, fail::ANY_ADDR);
}

Gem5Breakpoint* OnBreakpointAddition(address_t watchInstrPtr)
{
	System* sys = *System::systemList.begin();
	// FIXME: begin() vs. front() (see Gem5Controller::startup())
	// FIXME: Provide "sys" using the simulator-inst?
	return new Gem5Breakpoint(&sys->pcEventQueue, watchInstrPtr);
}

void OnBreakpointDeletion(Gem5Breakpoint* bp)
{
	if (bp) {
		delete bp; // TODO: required?
	}
}

// Controller-related:
unsigned int GetCPUId(System* sys, int context)
{
	return sys->getThreadContext(context)->cpuId();
}

System* GetSystemObject()
{
	return System::systemList.front();
}

int GetNumberOfContexts(System* sys)
{
	return sys->numContexts();
}

} // end-of-namespace: fail