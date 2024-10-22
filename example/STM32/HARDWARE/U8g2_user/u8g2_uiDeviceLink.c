#include "u8g2_ui.h"
#ifdef u8g2Ui_deviceLink_import

#include "core.h"

u8g2Ui_t *u8g2Ui_p = NULL;
void task_u8g2Ui(void *arg)
{
	if (!strcmp(arg, "run"))
	{
		u8g2Ui_run(u8g2Ui_p);
	}
	else if (!strcmp(arg, "timer"))
	{
		
	}
}
void oled_u8g2Ui_init(u8g2Ui_t *p)
{
	u8g2Ui_init(p);
	u8g2Ui_p = p;
	CORE_Run_add_task(task_u8g2Ui, "run");
	CORE_Timer_add_task(task_u8g2Ui, "timer", 50);
}

#endif
