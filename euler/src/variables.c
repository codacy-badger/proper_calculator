
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/includes.h"
#include "../parser/errors.h"

symbol_table stbl[MAX_VAR_QTY];
static int vrc = 0;
char *errmsg;
char *emsg;

void list_vars(void);
int lookup_vars(char *vname);
double add_var(char *vname, double vval);
double get_var_val(char *vname, int idx);
void get_var_name(int idx);

static int terr(int code, char *msg)
{
	console_puts("euler(error): ");
	console_puts(msg);
	console_puts("\n");
	__err = code;
	return __err;
}
void list_vars(void)
{
	int idx = vrc;
	if (idx < 0)
		return;
	while (idx--) {
		console_puts("Name: ");
		console_puts(stbl[idx].name);
		console_puts(" value: ");
		gcvt(stbl[idx].val, 4, emsg);
		console_puts(emsg);
		console_puts("\n");
	}
}
int lookup_vars(char *vname)
{
	int idx = vrc - 1;
	if (idx < 0)
		return NVF;
	while (strcmp(vname, stbl[idx].name) != 0) {
		idx--;
		if (idx < 0)
			return NVF;
	}
	return idx;
}

double get_var_val(char *vname, int idx)
{
	if (idx >= 0) { /* if idx > 0 function returns directly with id*/
		return stbl[idx].val;
	}
	idx = lookup_vars(vname);
	if (idx < 0) {
		errmsg = (char *)malloc(sizeof(char) * 30);
		sprintf(errmsg, "No variable found named: %s", vname);
		terr(NVF, errmsg);
		free(errmsg);
		return 0;
	} else {
		return stbl[idx].val;
	}
}

double add_var(char *vname, double vval)
{
	int idx = -1; /* arbitrary negative value */
	if (vrc >= MAX_VAR_QTY) {
		terr(MXVQ, "Max variable quantity has been reached!");
		return MXVQ;
	}

	/* Update if variable already exists. */
	idx = lookup_vars(vname);
	if (idx >= 0 && vrc > 0) {
		stbl[idx].val = vval;
		return vval;
	}

	strcpy(stbl[vrc].name, vname);
	stbl[vrc].val = vval;
	vrc++;

	return stbl[vrc - 1].val;
}
