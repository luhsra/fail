for (auto reg = registers.begin(); reg != registers.end(); ++reg) {
	for (int bitnr = 0; bitnr < (*reg).width(); ++bitnr) {
		for (int instr = 0; ; ++instr) {
			clearEvents();

			// restore previously saved simulator state
			RestoreEvent ev_restore("./bochs_save_point");
			addEventAndWait(&ev_restore);

			// breakpoint $instr instructions in the future
			BPEvent ev_instr_reached(ANY_ADDR, instr);
			addEvent(&ev_instr_reached);
			// breakpoint at function exit
			BPEvent ev_func_end(INST_ADDR_FUNC_END);
			addEvent(&ev_func_end);

			// if we reach the exit first, this round is done
			if (waitAny() == ev_func_end.id()) {
				break;
			}

			// inject bit-flip at bit $bitnr in register $reg
			RegisterBitflip fi_bitflip(*reg, bitnr);
			inject(&fi_bitflip);

			// catch traps and timeout
			TrapEvent ev_trap(ANY_TRAP);
			addEvent(&ev_trap);
			BPEvent ev_timeout(ANY_ADDR, 1000);
			addEvent(&ev_timeout);

			// we assert the succeeded bit-flip is the first thing we hear
			id = waitAny();
			assert(id == fi_bitflip.id());

			// wait for function exit, trap or timeout
			id = waitAny();
			if (id == ev_func_end.id()) {
				// log result
				int result = registers(REG_EAX).cur_value();
				log_test_result(*reg, bitnr, instr, LOG_RESULT, result);
			} else if (id == ev_trap.id()) {
				log_test_result(*reg, bitnr, instr, LOG_TRAP, ev_trap.TrapNr());
			} else if (id == ev_timeout.id()) {
				log_test_result(*reg, bitnr, instr, LOG_TIMEOUT);
			}
		}
	}
}
