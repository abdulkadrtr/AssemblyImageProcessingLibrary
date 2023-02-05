#include <stdio.h>

void rgb2gray(unsigned char* m1,unsigned char* m2,unsigned char* m3,int i){
	__asm{
	PUSH ECX
	PUSH ESI
	PUSH EDI
	PUSH EBX
	PUSH EAX
	PUSH EDX

	MOV ECX,i
	MOV ESI,m3
	MOV EDI,m2
	MOV EBX,m1
L1:	XOR AX,AX
	MOV AL,[EBX]
	MOV DX,AX
	MOV AL,[EDI]
	ADD DX,AX
	MOV AL,[ESI]
	ADD AX,DX
	MOV DL,3
	DIV DL
	MOV [EBX],AL
	INC EBX
	INC ESI
	INC EDI
	LOOP L1

	POP EDX
	POP EAX
	POP EBX
	POP EDI
	POP ESI
	POP ECX
	}
}

void erosion(unsigned char* img,int filter_size,int i,int j){
	int k = i*j;
	unsigned char* arr = (unsigned char*)malloc(sizeof(unsigned char)*k);
	__asm{
		PUSH ECX
		PUSH ESI
		PUSH EAX
		PUSH EBX
		PUSH EDX
		PUSH EDI
		
		MOV ECX,i
disD1:
		PUSH ECX
		MOV ECX,j
disD2 :
		PUSH ECX
		MOV ECX, filter_size
		MOV EAX, 255
disD3 :
		PUSH ECX
		MOV ECX, filter_size
dis4 :
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
		CMP EAX, i
		JNL PASS
		MOV EAX, [ESP + 8]
		ADD EAX, ECX
		SUB EAX, 2
		SUB EAX, EBX //EAX = J+L-SİZE
		CMP EAX, 0
		JL PASS
		CMP EAX, j
		JNL PASS
		//Taşma kontrolleri bitis
		PUSH EAX
		MOV EAX, [ESP + 16]
		ADD EAX, [ESP + 8]
		SUB EAX, 2
		SUB EAX, EBX
		MOV EBX, j
		MUL EBX
		POP EBX
		ADD EAX, EBX
		MOV EBX, img
		ADD EBX, EAX
		POP EAX //filtre_Size*3 hucre alani icin max, min deger
		CMP AL, [EBX]
		JNA PASS1
		MOV AL, [EBX]
		JMP PASS1
ARA1 :  JMP disD1
ARA3 :  JMP disD2
		//EN IC DONGU KODLARI bitis
PASS :  POP EAX
PASS1 : LOOP dis4
		POP ECX
		LOOP disD3
		//Max veya min degeri diziye yerlestirme baslangic
		PUSH EAX
		MOV EAX, [ESP + 8]
		DEC EAX
		MOV EBX, j
		MUL EBX
		MOV EDI, EAX
		MOV EAX, [ESP + 4]
		DEC EAX
		ADD EDI, EAX
		POP EAX
		ADD EDI,arr
		MOV [EDI], AL
		//Max veya min degeri diziye yerlestirme bitis
		POP ECX
		LOOP ARA3
		POP ECX
		LOOP ARA1
		
		MOV ESI,arr //img dizisi ile yeni dizi elemanlar� yer degistirir.
		MOV EDI,img	
		MOV ECX,k
L:		MOV AL,BYTE PTR [ESI]
		MOV BYTE PTR [EDI],AL
		INC ESI
		INC EDI
		LOOP L
		
		POP EDI
		POP EDX
		POP EBX
		POP EAX
		POP ESI
		POP ECX
	}
	free(arr);
}

void dilation(unsigned char* img,int filter_size,int i,int j){
	int k = i*j;
	unsigned char* arr = (unsigned char*)malloc(sizeof(unsigned char)*k);
	__asm{
		PUSH ECX
		PUSH ESI
		PUSH EAX
		PUSH EBX
		PUSH EDX
		PUSH EDI
		
		MOV ECX,i
disD1:
		PUSH ECX
		MOV ECX,j
disD2 :
		PUSH ECX
		MOV ECX, filter_size
		XOR EAX,EAX
disD3 :
		PUSH ECX
		MOV ECX, filter_size
dis4 :
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
		CMP EAX, i
		JNL PASS
		MOV EAX, [ESP + 8]
		ADD EAX, ECX
		SUB EAX, 2
		SUB EAX, EBX //EAX = J+L-SİZE
		CMP EAX, 0
		JL PASS
		CMP EAX, j
		JNL PASS
		//Taşma kontrolleri bitis
		PUSH EAX
		MOV EAX, [ESP + 16]
		ADD EAX, [ESP + 8]
		SUB EAX, 2
		SUB EAX, EBX
		MOV EBX, j
		MUL EBX
		POP EBX
		ADD EAX, EBX
		MOV EBX, img
		ADD EBX, EAX
		POP EAX //filtre_Size*3 hucre alani icin max, min deger
		CMP AL, [EBX]
		JA PASS1
		MOV AL, [EBX]
		JMP PASS1
ARA1 :  JMP disD1
ARA3 :  JMP disD2
		//EN IC DONGU KODLARI bitis
PASS :  POP EAX
PASS1 : LOOP dis4
		POP ECX
		LOOP disD3
		//Max veya min degeri diziye yerlestirme baslangic
		PUSH EAX
		MOV EAX, [ESP + 8]
		DEC EAX
		MOV EBX, j
		MUL EBX
		MOV EDI, EAX
		MOV EAX, [ESP + 4]
		DEC EAX
		ADD EDI, EAX
		POP EAX
		ADD EDI,arr
		MOV [EDI], AL
		//Max veya min degeri diziye yerlestirme bitis
		POP ECX
		LOOP ARA3
		POP ECX
		LOOP ARA1
		
		MOV ESI,arr //img dizisi ile yeni dizi elemanlar� yer degistirir.
		MOV EDI,img	
		MOV ECX,k
L:		MOV AL,BYTE PTR [ESI]
		MOV BYTE PTR [EDI],AL
		INC ESI
		INC EDI
		LOOP L
		
		POP EDI
		POP EDX
		POP EBX
		POP EAX
		POP ESI
		POP ECX
	}
	free(arr);
}

void gBlur(unsigned char* img,int i,int j){
	int k = i*j;
	unsigned char* arr = (unsigned char*)malloc(sizeof(unsigned char)*k);
	unsigned char* kernel=(unsigned char*)malloc(sizeof(unsigned char)*9);
	kernel[0] = 4; kernel[1] = 3; kernel[2] = 4; kernel[3] = 3; kernel[4] = 2;
	kernel[5] = 3; kernel[6] = 4; kernel[7] = 3; kernel[8] = 4;
	unsigned char a=0;
	__asm{
		PUSH ECX
		PUSH ESI
		PUSH EAX
		PUSH EBX
		PUSH EDX
		PUSH EDI
		
		MOV ECX,i
disD1:
		PUSH ECX
		MOV ECX,j
disD2 :
		PUSH ECX
		MOV ECX, 3
		MOV AL,a
		XOR EAX,EAX
		MOV a,AL
disD3 :
		PUSH ECX
		MOV ECX, 3
dis4 :
		MOV EAX, [ESP + 8] //EAX içinde i değeri var.
		ADD EAX, [ESP] //EAX = İ+K
		SUB EAX, 3
		//Taşma kontrolleri baslangic
		CMP EAX, 0
		JL PASS1
		CMP EAX, i
		JNL PASS1
		MOV EAX, [ESP + 4]
		ADD EAX, ECX
		SUB EAX, 3
		CMP EAX, 0
		JL PASS1
		CMP EAX, j
		JNL PASS1
		//Taşma kontrolleri bitis
		PUSH EAX
		MOV EAX, [ESP + 12]
		ADD EAX, [ESP + 4]
		SUB EAX, 3
		MOV EBX, j
		MUL EBX
		POP EBX
		ADD EAX, EBX
		MOV EBX, img
		ADD EBX, EAX
		JMP ALT
ARA4:   JMP dis4
ARA5:	JMP disD3
ALT: 
		PUSH EBX
		XOR EAX,EAX
		MOV AX,3
		MOV EBX,[ESP+4]
		MUL BL
		ADD AX,CX
		SUB AX,4
		MOV EBX,kernel
		ADD EBX,EAX
		PUSH ECX
		MOV CL,[EBX]
		MOV EBX,[ESP+4]
		MOV AL,[EBX]
		SHR AL,CL
		ADD a,AL
		POP ECX
		POP EBX

		JMP PASS1
ARA1 :  JMP disD1
ARA3 :  JMP disD2
PASS1 :
		//EN IC DONGU KODLARI bitis
		LOOP ARA4
		POP ECX
		LOOP ARA5
		//Max veya min degeri diziye yerlestirme baslangic
		MOV EAX, [ESP + 4]
		DEC EAX
		MOV EBX, j
		MUL EBX
		MOV EDI, EAX
		MOV EAX, [ESP]
		DEC EAX
		ADD EDI, EAX
		ADD EDI,arr
		MOV AL,a
		MOV [EDI], AL
		//Max veya min degeri diziye yerlestirme bitis
		POP ECX
		LOOP ARA3
		POP ECX
		LOOP ARA1
		
		MOV ESI,arr //img dizisi ile yeni dizi elemanlar� yer degistirir.
		MOV EDI,img	
		MOV ECX,k
L:		MOV AL,BYTE PTR [ESI]
		MOV BYTE PTR [EDI],AL
		INC ESI
		INC EDI
		LOOP L
		
		POP EDI
		POP EDX
		POP EBX
		POP EAX
		POP ESI
		POP ECX
	}
	free(kernel);
	free(arr);
}

void sEdge(unsigned char* img,int i,int j){
	int k = i*j;
	unsigned char* arr = (unsigned char*)malloc(sizeof(unsigned char)*k);
	short* kernel=(short*)malloc(sizeof(short)*9);
	unsigned char* arr1 = (unsigned char*)malloc(sizeof(unsigned char)*k);
	short* kernel1=(short*)malloc(sizeof(short)*9);
	kernel[0] = 1; kernel[1] = 2; kernel[2] = 1; kernel[3] = 0; kernel[4] = 0;
	kernel[5] = 0; kernel[6] = -1; kernel[7] = -2; kernel[8] = -1;	
	kernel1[0] = -1; kernel1[1] = 0; kernel1[2] = 1; kernel1[3] = -2; kernel1[4] = 0;
	kernel1[5] = 2; kernel1[6] = -1; kernel1[7] = 0; kernel1[8] = 1;
	short a1=0;
	short a2=0;
	__asm{
		PUSH ECX
		PUSH ESI
		PUSH EAX
		PUSH EBX
		PUSH EDX
		PUSH EDI
		
		MOV ECX,i
disD1:
		PUSH ECX
		MOV ECX,j
disD2 :
		PUSH ECX
		MOV ECX, 3
		XOR EAX,EAX
		MOV a1,AX
		MOV a2,AX
disD3 :
		PUSH ECX
		MOV ECX, 3
dis4 :
		MOV EAX, [ESP + 8] //EAX içinde i değeri var.
		ADD EAX, [ESP] //EAX = İ+K
		SUB EAX, 3
		//Taşma kontrolleri baslangic
		CMP EAX, 0
		JL PASS1
		CMP EAX, i
		JNL PASS1
		MOV EAX, [ESP + 4]
		ADD EAX, ECX
		SUB EAX, 3
		CMP EAX, 0
		JL PASS1
		CMP EAX, j
		JNL PASS1
		//Taşma kontrolleri bitis
		PUSH EAX
		MOV EAX, [ESP + 12]
		ADD EAX, [ESP + 4]
		SUB EAX, 3
		MOV EBX, j
		MUL EBX
		POP EBX
		ADD EAX, EBX
		MOV EBX, img
		ADD EBX, EAX
		JMP ALT
ARA4:   JMP dis4
ARA5:	JMP disD3
ALT: //
		PUSH EBX
		XOR EAX, EAX
		MOV AX, 3
		MOV EBX,[ESP+4]
		MUL BL
		ADD AX, CX
		SUB AX, 4
		SHL AX,1
		PUSH EAX
		MOV EBX, kernel
		ADD EBX, EAX
		MOV DX, [EBX]
		MOV EBX,[ESP+4]
		XOR EAX,EAX
		MOV AL, [EBX]
		IMUL DX
		ADD a1,AX
		XOR EAX,EAX
		MOV AL,[EBX]
		POP EBX
		ADD EBX, kernel1
		MOV DX, [EBX]
		IMUL DX
		ADD a2,AX
		POP EBX
	//
		JMP PASS1
ARA1 :  JMP disD1
ARA3 :  JMP disD2
PASS1 :
		//EN IC DONGU KODLARI bitis
		LOOP ARA4
		POP ECX
		LOOP ARA5
		//Max veya min degeri diziye yerlestirme baslangic
		MOV EAX, [ESP + 4]
		DEC EAX
		MOV EBX, j
		MUL EBX
		MOV EDI, EAX
		MOV EAX, [ESP]
		DEC EAX
		ADD EDI, EAX
		PUSH EDI
		ADD EDI,arr
		MOV AX,a1
		CMP AX,0
		JNL C
		NEG AX
		MOV a1,AX
		MOV AL, BYTE PTR a1
C:		MOV [EDI], AL
		POP EDI
		ADD EDI,arr1
		MOV AX,a2
		CMP AX,0
		JNL C1
		NEG AX
		MOV a2,AX
		MOV AL, BYTE PTR a2
C1:		MOV [EDI], AL
		//Max veya min degeri diziye yerlestirme bitis
		POP ECX
		LOOP ARA3
		POP ECX
		LOOP ARA1
		
		MOV ESI,arr //img dizisi ile yeni dizi elemanlar� yer degistirir.
		MOV EBX,arr1
		MOV EDI,img	
		MOV ECX,k
L:		MOV AL,BYTE PTR [ESI]
		ADD AL,BYTE PTR [EBX]
		MOV BYTE PTR [EDI], AL
		INC EBX
		INC ESI
		INC EDI
		LOOP L
		
		POP EDI
		POP EDX
		POP EBX
		POP EAX
		POP ESI
		POP ECX
	}
	free(kernel1);
	free(arr1);
	free(kernel);
	free(arr);
}
