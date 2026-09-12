// Final gameplay chain from 0.60 onward.
#include "../world/weather60.inc"
#define game_update game_update_legacy60
#include "../sim/sim60_game.inc"
#undef game_update
#include "../ui/ui70_state.inc"
#define game_update game_update_legacy70
#include "../sim/sim70_game.inc"
#undef game_update
#include "../perf/perf80.inc"
#define game_update game_update_legacy80
#include "../sim/sim80_game.inc"
#undef game_update
#include "final100.inc"
#define game_update game_update_legacy100
#include "../sim/sim100_game.inc"
#undef game_update
#define game_update game_update_legacy110
#include "../sim/sim110_game.inc"
#undef game_update
#include "../world/roads130.inc"
#include "../sim/sim130_game.inc"
