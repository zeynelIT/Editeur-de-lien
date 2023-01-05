//
//  read_elf_table_symbol.c
//  Editeur de Liens
//

#include <stdio.h>
#include <stdlib.h>
#include "modules/CheckElf.h"
#include "modules/readHeader.h"
#include "modules/readSectionTable.h"
#include "modules/getTableSymbSection.c"

int main(int argc, char **argv)
{

	if (argc < 2)
	{
		printf("Usage : ./read_elf_section_table <File>\n");
		exit(1);
	}

	FILE *file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		printf("ERROR : %s no such file.\n", argv[1]);
		exit(1);
	}

	if (!checkELF(file))
	{
		printf("Not a ELF file !\n");
		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
	}

	Elf32_Ehdr *Header = malloc(sizeof(Elf32_Ehdr));
	getHeader(file, Header);

	fseek(file, Header->e_shoff, SEEK_SET); // On oublie pas de pointer vers l'en-tête de la section

    Elf32_AllSec * AllSectionsTables = initSectionTable(Header->e_shnum);
    getAllSectionsTables(file, Header, AllSectionsTables);

	printf("\n");
	printf("Adresses are given un hexadecimal format.\n");
	printf("All values are given in bytes in decimal format.\n\n");
	printf("Nb\tName\t\tValue\t\tSize\tBind\tType\t\tOther\tShndx\n");
	printf("====================================================================");
	printf("=====================================================================\n");

	// on recupere le decalage de la table symbole puis on se decale
    int indexSymbSec = getSectionByType(AllSectionsTables, SHT_SYMTAB);
    Elf32_Shdr * SectionTable = AllSectionsTables->TabAllSec[indexSymbSec];
    Elf32_SecContent * SectionContent = AllSectionsTables->TabAllSecContent[indexSymbSec];
	fseek(file, SectionTable->sh_offset, SEEK_SET);

	Elf32_Sym *symTab = malloc(sizeof(Elf32_Sym));
	for (int i = 0; i < SectionTable->sh_size / 16; i++)
	{
		printf("%d\t", i);
		GetTableSymbPart(file, Header, SectionContent, symTab);
		printf("\n");
	}

	return 0;
}
