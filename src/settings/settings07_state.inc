// -----------------------------------------------------------------------------
// Settings 0.7.1: persistent local preferences and pause/options menu state.
// This file intentionally contains only state so camera code can read it early.
// -----------------------------------------------------------------------------
#define VK_RETURN 0x0D
#define VK_LEFT   0x25
#define VK_UP     0x26
#define VK_RIGHT  0x27
#define VK_DOWN   0x28

// Defined later in the legacy gameplay include; declared here because settings
// persistence/menu helpers are compiled earlier in the single translation unit.
static int key_tap(int vk);
static void update_key_edges(void);

static int g_camera_invert_y=0;
static int g_settings_menu_open=0;
static int g_settings_selected=0;
static char g_settings_path[520];
static int g_settings_dirty=0;
