#include "readHeader.h"
#include "readContent.h"
#include "getTableSymbSection.c"

Elf32_Info * initElf32_Inf(){
    Elf32_Info * ElfInfo = malloc(sizeof(Elf32_Info));
    ElfInfo->Header = malloc(sizeof(Elf32_Ehdr));
    return ElfInfo;
}

Elf32_Info * getAllInfo(FILE * file){

    Elf32_Info * ElfInfo = initElf32_Inf();

	getHeader(file, ElfInfo->Header);

	fseek(file, ElfInfo->Header->e_shoff, SEEK_SET); // On oublie pas de pointer vers l'en-tête de la section

    ElfInfo->AllSections = initSectionTable(ElfInfo->Header->e_shnum);
    getAllSectionsTables(file, ElfInfo->Header, ElfInfo->AllSections);

    getAllSectionsContent(file, ElfInfo->AllSections);

    int indexSymbSec = getSectionByType(ElfInfo->AllSections, SHT_SYMTAB);
    Elf32_Shdr * SecSymbTab = ElfInfo->AllSections->TabAllSec[indexSymbSec];
    Elf32_SecContent SectionContent = ElfInfo->AllSections->TabAllSecContent[indexSymbSec];

	fseek(file, SecSymbTab->sh_offset, SEEK_SET);

	Elf32_Sym * AllSymbolTables = malloc(sizeof(Elf32_Sym) * SecSymbTab->sh_size / 16);
    
	getAllTableSymb(file, ElfInfo->Header, SectionContent, AllSymbolTables, SecSymbTab->sh_size / 16);

    ElfInfo->AllSymbol = AllSymbolTables;

    return ElfInfo;
}