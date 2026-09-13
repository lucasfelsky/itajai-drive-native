// Itajai Drive Updater source bundle.
// Split into include parts only to keep connector-sized commits manageable.
#include "updater_parts/part01.inc"
#include "updater_parts/part02.inc"
#include "updater_parts/part03.inc"
#define read_config read_config_legacy11
#define updater_thread updater_thread_legacy11
#include "updater_parts/part04.inc"
#undef read_config
#undef updater_thread
#define updater_thread updater_thread_legacy12
#define channel12_start channel12_start_legacy12
#include "updater_parts/part06_channel.inc"
#undef updater_thread
#undef channel12_start
#include "updater_parts/part07_build.inc"
#include "updater_parts/part05.inc"
