#include <stdlib.h>
#include <string.h>
#include <svc.h>
#include <stdio.h>
#include <svctxo.h>

#define MAX_FILE_NAME 32

#define RAM_DRIVE "I:"
#define FLASH_DRIVE "F:"

static long delete_all_files_from_drive(char *drive, int group);
static void warning_beep(void);

int main(void)
{

	char activate_delete = '0';

	get_env("CME_DELETE", (char *)&activate_delete, sizeof(char));

	open(DEV_CONSOLE, 0);

	set_font("F:Helvetica16.vft");

	if (activate_delete == '1')
	{

		write_at("Wait...", 9, 1, 1);

		delete_all_files_from_drive(FLASH_DRIVE, 2);
		delete_all_files_from_drive(RAM_DRIVE, 2);

		put_env("CME_DELETE", "0", sizeof(char));

		clrscr();
	}

	warning_beep();

	set_font("F:Helvetica16.vft");

	while (1)
	{

		if ((wait_event() & EVT_ACTIVATE))
		{

			write_at("Done", 13, 1, 1);

			error_tone();

		}
	
	}

	return 0;
}

static long delete_all_files_from_drive(char *drive, int group)
{

	char file[MAX_FILE_NAME] = {'\0'};
	int last_group = get_group();
	long total_deleted_files = 0L;

	set_group(group);

	strcat(file, drive);

	while (dir_get_first(file) == 0)
	{

		if (_remove(file) == 0)
		{
			total_deleted_files++;
		}

		memset(file, '\0', sizeof(file));

		strcat(file, drive);
	}

	set_group(last_group);

	return total_deleted_files;
}

static void warning_beep(void)
{

	error_tone();
	SVC_WAIT(150);
	error_tone();
	SVC_WAIT(150);
	error_tone();

	return;
}
