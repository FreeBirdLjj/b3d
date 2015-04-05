#include "view.h"

int main(int argc, char *argv[])
{
	/* Init some libraries */
	glutInit(&argc, argv);

	/* Launch the viewing window. */
	view(argv[0]);

	return 0;
}
