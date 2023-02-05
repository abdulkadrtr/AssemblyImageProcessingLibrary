#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void Dilation(int n, int filter_size, short* resimadres_org);
void Erosion(int n, int filter_size, short* resimadres_org);

int main(void) {
	int M, N, Q, i, j, filter_size;
	bool type;
	int efile;
	char org_resim[100], dil_resim[] = "dilated.pgm", ero_resim[] = "eroded.pgm";
	do {
		printf("Orijinal resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &org_resim);
		system("CLS");
		efile = readImageHeader(org_resim, N, M, Q, type);
	} while (efile > 1);
	int** resim_org = resimOku(org_resim);

	printf("Orjinal Resim Yolu: \t\t\t%s\n", org_resim);

	short* resimdizi_org = (short*)malloc(N * M * sizeof(short));

	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			resimdizi_org[i * N + j] = (short)resim_org[i][j];

	int menu;
	printf("Yapmak istediginiz islemi giriniz...\n");
	printf("1-) Dilation\n");
	printf("2-) Erosion\n");
	printf("3-) Cikis\n> ");
	scanf_s("%d", &menu);
	printf("Filtre boyutunu giriniz: ");
	scanf_s("%d", &filter_size);
	switch (menu) {
	case 1:
		Dilation(N * M, filter_size, resimdizi_org);
		resimYaz(dil_resim, resimdizi_org, N, M, Q);
		break;
	case 2:
		Erosion(N * M, filter_size, resimdizi_org);
		resimYaz(ero_resim, resimdizi_org, N, M, Q);
		break;
	case 3:
		system("EXIT");
		break;
	default:
		system("EXIT");
		break;
	}

	system("PAUSE");
	return 0;
}

void Dilation(int n, int filter_size, short* resim_org) {
	__asm {
		PUSH ECX
		PUSH ESI
		PUSH EAX

		XOR EAX,EAX
		MOV ECX, n
		SHR ECX, 1
L5 :	PUSH EAX 
		LOOP L5

		PUSH EBX
		PUSH EDX
		PUSH EDI

		MOV ECX, 512
disD1:
		PUSH ECX
		//EN DIS DONGU KOMUTLARI
		//EN DIS DNGU KOMUTLARI
		MOV ECX, 512
disD2 :
		PUSH ECX
		//DONGU KODLARI BASLANGİC
		MOV ECX, filter_size //k hesaplandı ECX içinde.
		XOR EAX, EAX //max veya min degeri yeni elemanda düzenleme
		//DONGU KODLARI BİTİS
disD3 :
		PUSH ECX
		//DONGU KODLARI
		MOV ECX, filter_size
		//DONGU KODLARI
dis4 :
	//EN IC DONGU KODLARI baslangic
		PUSH EAX
		MOV EAX, [ESP + 12] //EAX içinde i değeri var.
		ADD EAX, [ESP + 4] //EAX = İ+K
		SUB EAX,2 // İNDİS DEGERİNİ DOGRU HESAPLAMAK İCİN DONGU DEGİSKENLERİNİN 1 AZALTIYORUM.
		MOV EBX, filter_size
		SHR EBX, 1 //EBX içinde filter_size/2 var
		SUB EAX, EBX //EAX = İ+K-size
		//Taşma kontrolleri baslangic
		CMP EAX, 0
		JL PASS
		CMP EAX, 512
		JNL PASS
		MOV EAX, [ESP + 8]
		ADD EAX, ECX
		SUB EAX,2 // İNDİS DEGERİNİ DOGRU HESAPLAMAK İCİN DONGU DEGİSKENLERİNİN 1 AZALTIYORUM.
		SUB EAX, EBX //EAX = J+L-SİZE
		CMP EAX, 0
		JL PASS
		CMP EAX, 512
		JNL PASS
		//Taşma kontrolleri bitis
		SHL EAX, 1 //EAX = 2*(J-SİZE+L)
		PUSH EAX
		MOV EAX, [ESP + 16]
		ADD EAX, [ESP + 8]
		SUB EAX, 2
		SUB EAX, EBX
		MOV EBX, 1024
		MUL EBX
		POP EBX
		ADD EAX, EBX
		MOV EBX, resim_org
		ADD EBX, EAX
		POP EAX //filtre_Size*3 hucre alani icin max, min deger
		CMP AX, WORD PTR[EBX]
		JA PASS1
		MOV AX, WORD PTR[EBX]
		JMP PASS1
ARA1 :  JMP disD1
ARA3 :  JMP disD2
		//EN IC DONGU KODLARI bitis
PASS :  POP EAX
PASS1 : LOOP dis4
		POP ECX
		LOOP disD3
		//Max veya min degeri stack dizisine yerlestirme
		PUSH EAX
		MOV EAX, [ESP + 8]
		DEC EAX
		MOV EBX, 1024
		MUL EBX
		MOV EDI, EAX
		MOV EAX, [ESP + 4]
		DEC EAX
		SHL EAX, 1
		ADD EDI, EAX //EDI DEGERİ MATRİSİN BASTAN KACINCI İ J OLDUGUNU GOSTERIYOR
		POP EAX
		ADD EDI, 20 // BU en basta push edilen degerleri gecmek icin
		MOV WORD PTR[ESP + EDI], AX
		//Max veya min degeri stack dizisine yerlestirme
		POP ECX
		LOOP ARA3
		POP ECX
		LOOP ARA1

		POP EDI
		POP EDX
		POP EBX

		MOV ESI, resim_org
		MOV ECX, n
		SHR ECX, 1

D:		POP EAX
		MOV WORD PTR[ESI], AX
		ADD ESI, 2
		ROL EAX, 16
		MOV WORD PTR[ESI], AX
		ADD ESI, 2
		LOOP D

		POP EAX
		POP ESI
		POP ECX
	}
	printf("\nDilation islemi sonucunda resim \"dilated.pgm\" ismiyle olusturuldu...\n");
}

void Erosion(int n, int filter_size, short* resim_org) {
	printf("\nErosion islemi yapiliyor...\n");
	__asm {
		PUSH ECX
		PUSH ESI
		PUSH EAX

		XOR EAX, EAX //Bu blok bana stackte fotograf icin istedigim alanı acar.
		MOV ECX, n
		SHR ECX, 1
L5 :	PUSH EAX
		LOOP L5

		PUSH EBX
		PUSH EDX
		PUSH EDI

		MOV ECX, 512
		disD1:
		PUSH ECX
		//EN DIS DONGU KOMUTLARI
		//EN DIS DNGU KOMUTLARI
		MOV ECX, 512
disD2 :
		PUSH ECX
		//DONGU KODLARI BASLANGİC
		MOV ECX, filter_size //k hesaplandı ECX içinde.
		MOV EAX, 255  //max veya min degeri yeni elemanda düzenleme
		//DONGU KODLARI BİTİS
disD3 :
		PUSH ECX
		//DONGU KODLARI
		MOV ECX, filter_size
		//DONGU KODLARI
dis4 :
		//EN IC DONGU KODLARI baslangic
		PUSH EAX
		MOV EAX, [ESP + 12] //EAX içinde i değeri var.
		ADD EAX, [ESP + 4] //EAX = İ+K
		SUB EAX, 2
		MOV EBX, filter_size
		SHR EBX, 1 //EBX içinde filter_size/2 var
		SUB EAX, EBX //EAX = İ+K-size
		//Taşma kontrolleri baslangic
		CMP EAX, 0
		JL PASS
		CMP EAX, 512
		JNL PASS
		MOV EAX, [ESP + 8]
		ADD EAX, ECX
		SUB EAX, 2
		SUB EAX, EBX //EAX = J+L-SİZE
		CMP EAX, 0
		JL PASS
		CMP EAX, 512
		JNL PASS
		//Taşma kontrolleri bitis
		SHL EAX, 1 //EAX = 2*(J-SİZE+L)
		PUSH EAX
		MOV EAX, [ESP + 16]
		ADD EAX, [ESP + 8]
		SUB EAX, 2
		SUB EAX, EBX
		MOV EBX, 1024
		MUL EBX
		POP EBX
		ADD EAX, EBX
		MOV EBX, resim_org
		ADD EBX, EAX
		POP EAX //filtre_Size*3 hucre alani icin max, min deger
		CMP AX, WORD PTR[EBX]
		JNA PASS1
		MOV AX, WORD PTR[EBX]
		JMP PASS1
ARA1 :  JMP disD1
ARA3 :  JMP disD2
		//EN IC DONGU KODLARI bitis
PASS :  POP EAX
PASS1 : LOOP dis4
		POP ECX
		LOOP disD3
		//Max veya min degeri stackteki diziye yerlestirme baslangic
		PUSH EAX
		MOV EAX, [ESP + 8]
		DEC EAX
		MOV EBX, 1024
		MUL EBX
		MOV EDI, EAX
		MOV EAX, [ESP + 4]
		DEC EAX
		SHL EAX, 1
		ADD EDI, EAX //EDI DEGERİ MATRİSİN BASTAN KACINCI İ J OLDUGUNU GOSTERIYOR
		POP EAX
		ADD EDI, 20 // BU en basta push edilen degerleri gecmek icin
		MOV WORD PTR[ESP + EDI], AX
		//Max veya min degeri stackteki diziye yerlestirme baslangic
		POP ECX
		LOOP ARA3
		POP ECX
		LOOP ARA1

		POP EDI
		POP EDX
		POP EBX

		MOV ESI, resim_org
		MOV ECX, n
		SHR ECX, 1
D:		POP EAX
		MOV WORD PTR[ESI], AX
		ADD ESI, 2
		ROL EAX, 16
		MOV WORD PTR[ESI], AX
		ADD ESI, 2
		LOOP D

		POP EAX
		POP ESI
		POP ECX
	}
	printf("\nErosion islemi sonucunda resim \"eroded.pgm\" ismiyle olusturuldu...\n");
}
