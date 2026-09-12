// Final gameplay chain from 0.60 onward.
#include "../world/weather60.inc"
#define game_update game_update_legacy60
#include "../sim/sim60_game.inc"
#undef game_update
#include "../ui/ui70_state.inc"
#include "../sim/sim70_game.inc"
