#include <stdint.h>
#include "fb.h"
#include "interrupts/idt.h"
#include "apps/terminal.h"
#include "apps/terminal_commands.h"
#include "network/nic.h"
#include "network/ethernet.h"
#include "network/arp.h"
#include "network/ipv4.h"
#include "network/icmp.h"
#include "network/tcp.h"
#include "fs/rfs.h"
#include "gui/keybind.h"
#include "boot_log.h"
#include "installer.h"
#include "serial.h"

void desktop_init(void);
void desktop_redraw(void);

static void int_to_str_local(int val, char *out)
{
    char tmp[16];
    int i = 0;
    if (val == 0) tmp[i++] = '0';
    while (val > 0) { tmp[i++] = '0' + (val % 10); val /= 10; }
    int j = 0;
    while (i > 0) out[j++] = tmp[--i];
    out[j] = 0;
}

void kernel_main(void)
{
    fb_init();
    idt_init();
    serial_init();

    installer_run();

    boot_log_run();
    terminal_init();

    terminal_print("Welcome, ");
    terminal_print(installer_get_username());
    terminal_print("! (timezone: ");
    terminal_print(installer_get_timezone());
    terminal_print(")\n");

    desktop_init();
    nic_init();
    ethernet_init();
    arp_init();
    ipv4_init();
    tcp_init();
    rfs_init();
    keybind_init();

    unsigned char gateway_ip[4] = { 10, 0, 2, 2 };
    arp_send_request(gateway_ip);

    while (1)
    {
        ethernet_poll();

        if (icmp_reply_received())
        {
            unsigned char *ip = icmp_last_reply_ip();
            char buf[8];
            terminal_print("Reply from ");
            for (int i = 0; i < 4; i++)
            {
                int_to_str_local(ip[i], buf);
                terminal_print(buf);
                if (i < 3) terminal_print(".");
            }
            terminal_print("\n");
            desktop_redraw();
        }

        if (tcp_data_available())
        {
            char buf[512];
            tcp_read_data(buf, sizeof(buf));
            terminal_print(buf);
            desktop_redraw();
        }
    }
}
