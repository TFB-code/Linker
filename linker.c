#include <stdio.h>
#include <string.h>

int filesize(FILE *fp) {
	fseek(fp,0,SEEK_END);
	int fsize=ftell(fp);
	fseek(fp,0, SEEK_SET);
	return fsize;
}

char ver[] = "0.1";

int main (int argc, char *argv[]) {
	printf("Linker v%s\n-----------\n\n",ver);

	if (argc<3) {
		printf("linker failed - %d parameters passed but 2 required. \n",argc-1);
		return 0;
	}
	char *ipos_ptr = strrchr(argv[0],'\\')+1;
	char infile[strlen(argv[0])];
	strcpy(infile,ipos_ptr); 
	printf("%s %s %s\n\n",infile, argv[1], argv[2]);
	FILE *vmfile;
	FILE *bcfile;
	FILE *outfile;
	vmfile=fopen(argv[1],"rb");
	if (vmfile==0) {
		printf("linker failed - %s missing or unable to open.", argv[1]);
		return 0;
	}
	bcfile=fopen(argv[2],"rb");
	if (bcfile==0) {
		printf("linker failed - %s missing or unable to open.", argv[2]);
		return 0;
	}
	
	int len = strlen(argv[2]) + 5;		// allow room for a ".exe" and a null terminating character.

	char output[len];
	char *pos_ptr = strrchr(argv[2],'.');
	if (pos_ptr != NULL) {
		int pos = pos_ptr-argv[2];
		strncpy(output,argv[2],pos);
	}
	strcat(output,".exe");
	outfile=fopen(output,"wb");
	char buf[255];
	int eof=1;
	while (eof==1) {
		eof=fread(buf,1,1,vmfile);
		if (eof==0) break;
		fwrite(buf,1,1,outfile);
	}
	long bc_index=ftell(vmfile);	//capture bytecode index point.
	fclose(vmfile);
	eof=1;
	while (eof==1) {
		eof=fread(buf,1,1,bcfile);
		if (eof==0) break;
		fwrite(buf,1,1,outfile);
	}
	fwrite(&bc_index,sizeof(bc_index),1,outfile);	//writes index point in little endian format - lowest digits first.	
	fclose(bcfile);
	fclose(outfile);
	printf("success : %s\n",output);
	return 1;
}
