#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

void usage(char **argv)
{
	printf("Usage: %s [-t target] <filename>\n", argv[0]);
}

void target_c(FILE *f)
{
	int c;
	char buf[1024];
	int incseq = 0;
	int shiftseq = 0;

	printf("#include <stdio.h>\nchar a[1024];int main(){char *p=a;");
	while(1) {
		buf[0] = 0;
		c = fgetc(f);

		if (c == EOF) {
			break;
		}

		if (c != '+' && c != '-' && c != '<' && c != '>' && c != '[' && c != ']' && c != '.' && c != ',') {
			continue;
		}

		if (c == '+' || c == '-') {
			incseq += c == '+' ? 1 : -1;
		} else if (incseq != 0) {
			sprintf(buf + strlen(buf), "*p%c=%d;", incseq < 0 ? '-' : '+', abs(incseq));
			incseq = 0;
		}

		if (c == '<' || c == '>') {
			shiftseq += c == '>' ? 1 : -1;
		} else if (shiftseq != 0) {
			sprintf(buf + strlen(buf), "p%c=%d;", shiftseq < 0 ? '-' : '+', abs(shiftseq));
			shiftseq = 0;
		}

		if (c == '[') {
			sprintf(buf + strlen(buf), "while(*p){");
		}

		if (c == ']') {
			sprintf(buf + strlen(buf), "}");
		}

		if (c == '.') {
			sprintf(buf + strlen(buf), "putchar(*p);");
		}

		if (c == ',') {
			sprintf(buf + strlen(buf), "scanf(\"%%c\",p);");
		}

		if (buf[0]) {
			fputs(buf, stdout);
		}
		buf[0] = 0;
	}
	
	if (incseq != 0) {
		sprintf(buf + strlen(buf), "*p%c=%d;", incseq < 0 ? '-' : '+', abs(incseq));
	}

	if (shiftseq != 0) {
		sprintf(buf + strlen(buf), "p%c=%d;", shiftseq < 0 ? '-' : '+', abs(shiftseq));
	}

	if(buf[0]) {
		fputs(buf, stdout);
	}

	fputs("return 0;}", stdout);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		usage(argv);
		return 1;
	}

	char *target = NULL;
	int opt;
	while ((opt = getopt(argc, argv, "t:")) != -1) {
		switch (opt) {
			case 't':
				target = optarg;
				break;
			default:
				usage(argv);
				return 0;
		}
	}

	FILE *f = fopen(argv[argc-1], "r");

	if (f == NULL) {
		perror("Error while opening file");
		return 1;
	}

	if (target == NULL) {
		target = "c";
	}
	
	if (strcmp(target, "c") == 0) {
		target_c(f);
	} else {
		printf("Invalid target %s\n", target);
	}

	fclose(f);
	return 0;
}
