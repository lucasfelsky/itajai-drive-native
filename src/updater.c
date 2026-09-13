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
#include "updater_parts/part06_channel.inc"
#include "updater_parts/part05.inc"
