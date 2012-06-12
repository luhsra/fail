#ifndef __WEATHERMONITOR_EXPERIMENT_INFO_HPP__
#define __WEATHERMONITOR_EXPERIMENT_INFO_HPP__

// autogenerated, don't edit!

// 0 = vanilla, 1 = guarded, 2 = plausibility
#define WEATHERMONITOR_VARIANT 0

#if WEATHERMONITOR_VARIANT == 0 // without vptr guards

// suffix for simulator state, trace file
#define WEATHER_SUFFIX				".vanilla"
// main() address:
// nm -C vanilla.elf|fgrep main
#define WEATHER_FUNC_MAIN			0x00100d70
// wait_begin address
#define WEATHER_FUNC_WAIT_BEGIN		0x00100d68
// wait_end address
#define WEATHER_FUNC_WAIT_END		0x00100d6c
// vptr_panic address (only exists in guarded variant)
#define WEATHER_FUNC_VPTR_PANIC		0x99999999
// number of main loop iterations to trace
// (determines trace length and therefore fault-space width)
#define WEATHER_NUMITER_TRACING		4
// number of instructions needed for these iterations in golden run (taken from
// experiment step #2)
#define WEATHER_NUMINSTR_TRACING	20599
// number of additional loop iterations for FI experiments (to see whether
// everything continues working fine)
#define WEATHER_NUMITER_AFTER		2
// number of instructions needed for these iterations in golden run (taken from
// experiment step #2)
#define WEATHER_NUMINSTR_AFTER		10272
// data/BSS begin:
// nm -C vanilla.elf|fgrep ___DATA_START__
#define WEATHER_DATA_START			0x00101558
// data/BSS end:
// nm -C vanilla.elf|fgrep ___BSS_END__
#define WEATHER_DATA_END			0x00102e48
// text begin:
// nm -C vanilla.elf|fgrep ___TEXT_START__
#define WEATHER_TEXT_START			0x00100000
// text end:
// nm -C vanilla.elf|fgrep ___TEXT_END__
#define WEATHER_TEXT_END			0x0010139f

#elif WEATHERMONITOR_VARIANT == 1 // with guards

// suffix for simulator state, trace file
#define WEATHER_SUFFIX				".guarded"
// main() address:
// nm -C guarded.elf|fgrep main
#define WEATHER_FUNC_MAIN			0x00100dac
// wait_begin address
#define WEATHER_FUNC_WAIT_BEGIN		0x00100da4
// wait_end address
#define WEATHER_FUNC_WAIT_END		0x00100da8
// vptr_panic address (only exists in guarded variant)
#define WEATHER_FUNC_VPTR_PANIC		0x0010104c
// number of main loop iterations to trace
// (determines trace length and therefore fault-space width)
#define WEATHER_NUMITER_TRACING		4
// number of instructions needed for these iterations in golden run (taken from
// experiment step #2)
#define WEATHER_NUMINSTR_TRACING	20599
// number of additional loop iterations for FI experiments (to see whether
// everything continues working fine)
#define WEATHER_NUMITER_AFTER		2
// number of instructions needed for these iterations in golden run (taken from
// experiment step #2)
#define WEATHER_NUMINSTR_AFTER		10272
// data/BSS begin:
// nm -C guarded.elf|fgrep ___DATA_START__
#define WEATHER_DATA_START			0x00101878
// data/BSS end:
// nm -C guarded.elf|fgrep ___BSS_END__
#define WEATHER_DATA_END			0x00103198
// text begin:
// nm -C guarded.elf|fgrep ___TEXT_START__
#define WEATHER_TEXT_START			0x00100000
// text end:
// nm -C guarded.elf|fgrep ___TEXT_END__
#define WEATHER_TEXT_END			0x001016af

#elif WEATHERMONITOR_VARIANT == 2 // with guards + plausibility check

// suffix for simulator state, trace file
#define WEATHER_SUFFIX				".plausibility"
// main() address:
// nm -C plausibility.elf|fgrep main
#define WEATHER_FUNC_MAIN			0x00100dbc
// wait_begin address
#define WEATHER_FUNC_WAIT_BEGIN		0x00100db4
// wait_end address
#define WEATHER_FUNC_WAIT_END		0x00100db8
// vptr_panic address (only exists in guarded variant)
#define WEATHER_FUNC_VPTR_PANIC		0x001010f0
// number of main loop iterations to trace
// (determines trace length and therefore fault-space width)
#define WEATHER_NUMITER_TRACING		4
// number of instructions needed for these iterations in golden run (taken from
// experiment step #2)
#define WEATHER_NUMINSTR_TRACING	20599
// number of additional loop iterations for FI experiments (to see whether
// everything continues working fine)
#define WEATHER_NUMITER_AFTER		2
// number of instructions needed for these iterations in golden run (taken from
// experiment step #2)
#define WEATHER_NUMINSTR_AFTER		10272
// data/BSS begin:
// nm -C plausibility.elf|fgrep ___DATA_START__
#define WEATHER_DATA_START			0x00101998
// data/BSS end:
// nm -C plausibility.elf|fgrep ___BSS_END__
#define WEATHER_DATA_END			0x001032b8
// text begin:
// nm -C plausibility.elf|fgrep ___TEXT_START__
#define WEATHER_TEXT_START			0x00100000
// text end:
// nm -C plausibility.elf|fgrep ___TEXT_END__
#define WEATHER_TEXT_END			0x001017cb

#else
#error Unknown WEATHERMONITOR_VARIANT
#endif

#endif
