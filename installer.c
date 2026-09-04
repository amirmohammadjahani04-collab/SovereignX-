#include "installer.h"
#include "fb.h"
#include "graphics.h"
#include "text_render.h"
#include "serial.h"

#define NAME_MAX 32

static volatile int installer_active_flag = 0;
static volatile char last_key = 0;
static volatile int key_ready = 0;

static char username[NAME_MAX] = "user";
static char timezone_str[NAME_MAX] = "UTC";

int installer_is_active(void)
{
    return installer_active_flag;
}

void installer_feed_key(char c)
{
    last_key = c;
    key_ready = 1;
}

const char *installer_get_username(void)
{
    return username;
}

const char *installer_get_timezone(void)
{
    return timezone_str;
}

static void small_delay(void)
{
    for (volatile uint32_t i = 0; i < 500000; i++);
}

static void fast_delay(void)
{
    for (volatile uint32_t i = 0; i < 50000; i++);
}

static void flush_pending_key(void)
{
    key_ready = 0;
    last_key = 0;
}

static char wait_key(void)
{
    while (!key_ready)
    {
        asm volatile("hlt");
    }
    char c = last_key;
    key_ready = 0;
    return c;
}

static char wait_for_yn_enter(void)
{
    char choice = 0;

    flush_pending_key();
    small_delay();
    flush_pending_key();

    while (1)
    {
        char c = wait_key();

        if (c == 'y' || c == 'Y') choice = 'Y';
        else if (c == 'n' || c == 'N') choice = 'N';
        else if (c == '\n' && choice != 0)
        {
            return choice;
        }
    }
}

static void wait_enter(void)
{
    flush_pending_key();
    small_delay();
    flush_pending_key();

    while (1)
    {
        char c = wait_key();
        if (c == '\n') return;
    }
}

static void draw_question_with_input(const char *q, const char *hint, const char *buf)
{
    Framebuffer *fb = get_fb();
    clear_screen(fb, 0x000000);
    draw_string(20, 20, q, 0x33FF66);
    draw_string(20, 44, hint, 0xAAAAAA);
    draw_string(20, 76, "> ", 0xFFFFFF);
    draw_string(36, 76, buf, 0xFFFFFF);
    fb_present();
}

static void read_line(const char *question, const char *hint, char *out, int max_len)
{
    int len = 0;
    out[0] = 0;

    flush_pending_key();
    small_delay();
    flush_pending_key();

    draw_question_with_input(question, hint, out);

    while (1)
    {
        char c = wait_key();

        if (c == '\n')
        {
            if (len > 0)
            {
                out[len] = 0;
                return;
            }
            continue;
        }

        if (c == '\b')
        {
            if (len > 0)
            {
                len--;
                out[len] = 0;
                draw_question_with_input(question, hint, out);
            }
            continue;
        }

        if (len < max_len - 1 && c >= 32 && c < 127)
        {
            out[len++] = c;
            out[len] = 0;
            draw_question_with_input(question, hint, out);
        }
    }
}

static void ask_question(const char *q)
{
    Framebuffer *fb = get_fb();
    clear_screen(fb, 0x000000);
    draw_string(20, 20, q, 0x33FF66);
    draw_string(20, 44, "(Type Y or N, then press Enter)", 0xAAAAAA);
    fb_present();
}

static void halted_by_user(void)
{
    Framebuffer *fb = get_fb();
    clear_screen(fb, 0x000000);
    draw_string(20, 20, "Setup cancelled by user.", 0xFF4444);
    draw_string(20, 44, "System halted. You can now turn off your device.", 0xAAAAAA);
    fb_present();

    while (1)
    {
        asm volatile("hlt");
    }
}

static int line_ends_with_ok(const char *s)
{
    int len = 0;
    while (s[len]) len++;
    if (len < 2) return 0;
    return (s[len - 2] == 'O' && s[len - 1] == 'K');
}

static int line_contains_successfully(const char *s)
{
    while (*s)
    {
        const char *p = s;
        const char *target = "successfully";
        int match = 1;
        while (*target)
        {
            if (*p != *target) { match = 0; break; }
            p++; target++;
        }
        if (match) return 1;
        s++;
    }
    return 0;
}

static void show_fake_terminal_install(void)
{
    Framebuffer *fb = get_fb();
    clear_screen(fb, 0x000000);

    const char *lines[] = {
        "u0_a646@localhost:~$ pip install bakOS",
        "Collecting bakOS kernel core...",
        "Building bakOS kernel modules ......... OK",
        "Installing bakOS drivers .............. OK",
        "bakOS kernel installed successfully.",
        "",
        "u0_a646@localhost:~$ pkg install SovereignX",
        "Resolving packages...",
        "Fetching SovereignX-core .............. OK",
        "Fetching SovereignX-desktop ........... OK",
        "Fetching SovereignX-network ............ OK",
        "Installing SovereignX ................. OK",
        "Configuring SovereignX system ......... OK",
        "",
        "SovereignX installed successfully.",
        "",
        "Press ENTER to continue setup..."
    };

    int count = sizeof(lines) / sizeof(lines[0]);
    int y = 20;

    for (int i = 0; i < count; i++)
    {
        uint32_t color = 0xCCCCCC;

        if (line_ends_with_ok(lines[i]) || line_contains_successfully(lines[i]))
            color = 0x33FF66;

        draw_string(20, y, lines[i], color);
        y += 20;
        fb_present();
        fast_delay();
    }

    wait_enter();
}

static const char *boot_wizard_lines[] = {
"================================================================================",
"[51/110] Kernel service registration",
"================================================================================",
"Registering kernel services ................ OK",
"Registering system calls ................... OK",
"Registering process manager ................ OK",
"Registering memory services ................. OK",
"",
"================================================================================",
"[52/110] Process manager",
"================================================================================",
"Initializing process table ................ OK",
"Initializing process IDs ................... OK",
"Initializing process states ................ OK",
"Process manager ............................ OK",
"",
"================================================================================",
"[53/110] Scheduler configuration",
"================================================================================",
"Selecting scheduler ....................... SovereignX Default",
"Configuring scheduler tick ................ OK",
"Configuring task priorities ................ OK",
"Scheduler configuration .................... OK",
"",
"================================================================================",
"[54/110] System call interface",
"================================================================================",
"Loading system call table .................. OK",
"Registering process calls .................. OK",
"Registering file calls .................... OK",
"Registering memory calls .................. OK",
"Registering device calls .................. OK",
"",
"================================================================================",
"[55/110] Device manager",
"================================================================================",
"Scanning devices .......................... OK",
"Building device tree ...................... OK",
"Registering devices ....................... OK",
"Device manager ............................ OK",
"",
"================================================================================",
"[56/110] PCI configuration",
"================================================================================",
"Scanning PCI bus .......................... OK",
"Reading PCI configuration space ........... OK",
"Detecting PCI devices ..................... OK",
"Assigning device resources ................ OK",
"",
"================================================================================",
"[57/110] Network stack",
"================================================================================",
"Initializing Ethernet layer ................ OK",
"Initializing IP layer ...................... OK",
"Initializing ARP ........................... OK",
"Initializing TCP ........................... OK",
"",
"================================================================================",
"[58/110] Storage subsystem",
"================================================================================",
"Initializing storage manager .............. OK",
"Detecting block devices ................... OK",
"Registering storage devices ............... OK",
"",
"================================================================================",
"[59/110] Filesystem driver",
"================================================================================",
"Loading filesystem driver ................. OK",
"Mounting system filesystem ................ OK",
"Checking filesystem metadata .............. OK",
"",
"================================================================================",
"[60/110] Shell environment",
"================================================================================",
"Initializing SovereignX shell .............. OK",
"Loading shell configuration ................ OK",
"Registering built-in commands .............. OK",
"Shell environment .......................... OK",
"",
"================================================================================",
"[61/110] GUI core",
"================================================================================",
"Initializing GUI subsystem ................. OK",
"Creating GUI context ........................ OK",
"Initializing drawing primitives ............. OK",
"GUI core ..................................... OK",
"",
"================================================================================",
"[110/110] Starting SovereignX",
"================================================================================",
"",
"All first-boot configuration steps completed successfully.",
"",
"System status:",
"",
"  Kernel       [OK]",
"  CPU          [OK]",
"  Memory       [OK]",
"  Storage      [OK]",
"  Filesystem   [OK]",
"  Graphics     [OK]",
"  Keyboard     [OK]",
"  Mouse        [OK]",
"  Network      [OK]",
"  GUI          [OK]",
"",
"================================================================================",
"                            SOVEREIGNX READY",
"================================================================================",
"",
"Welcome to SovereignX."
};

static void show_boot_wizard_log(void)
{
    Framebuffer *fb = get_fb();
    clear_screen(fb, 0x000000);

    int count = sizeof(boot_wizard_lines) / sizeof(boot_wizard_lines[0]);
    int y = 12;

    for (int i = 0; i < count; i++)
    {
        if (y > (int)fb->height - 24)
        {
            clear_screen(fb, 0x000000);
            y = 12;
        }

        uint32_t color = 0xCCCCCC;
        const char *l = boot_wizard_lines[i];

        if (l[0] == '=') color = 0x666666;
        else if (l[0] == '[') color = 0x55FFFF;
        else if (line_ends_with_ok(l) || line_contains_successfully(l)) color = 0x33FF66;

        draw_string(20, y, l, color);
        y += 14;
        fb_present();
    }

    char welcome_buf[64];
    int j = 0;
    const char *w = "Welcome, ";
    while (w[j]) { welcome_buf[j] = w[j]; j++; }
    int k = 0;
    while (username[k] && j < 62) { welcome_buf[j++] = username[k++]; }
    welcome_buf[j++] = '.';
    welcome_buf[j] = 0;

    draw_string(20, y, welcome_buf, 0x55FFFF);
    y += 14;
    draw_string(20, y, "The system is now configured and ready.", 0xAAAAAA);
    y += 20;
    draw_string(20, y, "Press ENTER to start SovereignX Desktop...", 0xFFFFFF);
    fb_present();

    wait_enter();
}

static void show_welcome_screen(void)
{
    Framebuffer *fb = get_fb();
    clear_screen(fb, 0x000000);
    draw_string(20, 20, "Setup complete!", 0x33FF66);
    draw_string(20, 44, "Welcome, ", 0xFFFFFF);
    draw_string(96, 44, username, 0x55FFFF);
    draw_string(20, 64, "Timezone: ", 0xAAAAAA);
    draw_string(104, 64, timezone_str, 0xFFFFFF);
    draw_string(20, 92, "Starting SovereignX...", 0xAAAAAA);
    fb_present();
}

void installer_run(void)
{
    installer_active_flag = 1;

    ask_question("Welcome to SovereignX Setup. Install to hard drive? Yes/No");
    char answer1 = wait_for_yn_enter();

    if (answer1 == 'N')
    {
        halted_by_user();
        return;
    }

    show_fake_terminal_install();

    read_line("Enter your username:", "(Type text, then press Enter)", username, NAME_MAX);
    read_line("Enter your timezone:", "(e.g. UTC, Tehran, London)", timezone_str, NAME_MAX);

    show_boot_wizard_log();
    show_welcome_screen();

    installer_active_flag = 0;
}
