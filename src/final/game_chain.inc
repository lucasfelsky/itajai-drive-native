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
#define game_update game_update_legacy130
#include "../sim/sim130_game.inc"
#undef game_update
#define game_update game_update_legacy180
#include "../sim/sim180_game.inc"
#undef game_update
#include "../foundation/vehicle23.inc"
#include "../foundation/vehicle25_state.inc"
#define game_update game_update_legacy230
#include "../sim/sim230_game.inc"
#undef game_update
#define game_update game_update_legacy240
#include "../sim/sim240_game.inc"
#undef game_update
#include "../foundation/vehicle25.inc"
#define game_update game_update_legacy250
#include "../sim/sim250_game.inc"
#undef game_update
#define game_update game_update_legacy211
#include "../sim/sim211_game.inc"
#undef game_update
#define game_update game_update_legacy212
#include "../sim/sim212_game.inc"
#undef game_update
#include "../world/life213.inc"
#define game_update game_update_legacy213
#include "../sim/sim213_game.inc"
#undef game_update
#include "../ui/ui215_state.inc"
#define game_update game_update_legacy215
#include "../sim/sim215_game.inc"
#undef game_update
#define game_update game_update_legacy250feel
#include "../sim/feel250_game.inc"
#undef game_update
#define game_update game_update_legacy260
#include "../sim/sim260_game.inc"
#undef game_update
#include "../net/net301.inc"
#define game_update game_update_legacy301
#include "../sim/net301_game.inc"
#undef game_update
#include "../net/net320.inc"
#define game_update game_update_legacy320
#include "../sim/net320_game.inc"
#undef game_update
#include "../net/net321_hamachi.inc"
#define game_update game_update_legacy321
#include "../sim/net321_game.inc"
#undef game_update
#include "../world/world390_scope.inc"
#include "../world/world390_route.inc"
#define fetch_host world390_fetch_host
#include "../world/city330_world.inc"
#undef fetch_host
#include "../ui/map380_game.inc"
