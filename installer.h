#ifndef INSTALLER_H
#define INSTALLER_H

void installer_run(void);
int installer_is_active(void);
void installer_feed_key(char c);
const char *installer_get_username(void);
const char *installer_get_timezone(void);

#endif
