typedef struct Label{
	char name[31];
	int address;
	char type[10];      /* "code" or "data"*/
	char attribute[10]; /* "entry", "external", or ""*/
	struct Label *next;
} Label;

typedef struct BinCode{
	char name[11];
	struct BinCode *next;
} BinCode;

typedef struct Ent{
	char name[31];
	struct Ent *next;
} Ent;

typedef struct assembler_context {
	FILE *in, *out;
	Label *symbol_table_head;
	BinCode *symbol_bin_code;
	Ent *symbol_ent;

	char *filename;
	char macro[10], macroName[82], line[82], lineCopy[82], totalLine[82];
	char labelName[32];
	char command[31];
	int IC, DC;
	int error;
	int lineNumber;
} assembler_context;


