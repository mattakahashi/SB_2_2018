section .data
incorreto	db	'Valor de entrada incorreto!', 0ah
newline	db	0Ah
NWLINESIZE	EQU $-newline

ONE:	dd	1

section .bss
AUX	resd	1
N	resd	1

section .text
global _start
_start: 
	push N
	call LerInteiro
	mov ebx,[N]
FAT: 	sub ebx,[ONE]
	cmp ebx,0
	je FIM
	mov [AUX],ebx
	mov eax,ebx
	imul dword [N]
	mov ebx,eax
	mov [N],ebx
	mov ebx,[AUX]
	jmp FAT
FIM: 	push N
	call EscreverInteiro
	mov eax,1
	mov ebx,0
	int 80h

LerInteiro:
	enter 24,0
	xor eax,eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	mov eax,3
	mov ebx,0
	mov edx,12
	mov ecx,ebp
	sub ecx,18
	int 80h
	mov [ebp-24],eax
	mov ecx,eax
	sub ecx,1 
	mov edi,0
	mov esi,10
	mov eax,0
	cmp byte [ebp-18],0x2D
	je negativo
	mov word [ebp-20], 0
retorna:
	cmp byte [ebp-18+edi],0x39
	jg erro
	cmp byte [ebp-18+edi],0x30
	jl erro
	mov bl,[ebp-18+edi]
	cmp  edi,0
	je soma
	cmp word [ebp-20],1
	je if_neg
retorna2:
	mul esi
soma:
	add eax,ebx
	sub eax,0x30
	sub ecx,1
	cmp ecx,0
	je conf_neg
	inc edi
	jmp retorna
if_neg:
	cmp edi,1
	je soma
	jmp retorna2
negativo:
	mov word [ebp-20], 1
	inc edi
	dec ecx
	jmp retorna
conf_neg:
	cmp word [ebp-20],0
	je guarda
	neg eax
guarda:
	mov ebx,[EBP+8]
	mov dword [ebx],eax
	jmp final
erro:
	mov eax,4
	mov ebx,1
	mov ecx,incorreto
	mov edx,28
	int 80h
	mov eax,1
	mov ebx,0
	int 80h
final:
	mov eax,[ebp-24]
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	leave
	ret 4

EscreverInteiro:
	enter 16,0
	xor eax,eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	mov edi,[ebp+8]
	mov eax,[edi]
	mov edi,0
	mov ecx,0
	mov ebx,10
	cmp eax,0
	je zero
	cmp eax,0
	jg positivo_EI
negativo_EI:
	mov ecx,1
	neg eax
positivo_EI:
	cdq
	cmp eax,0
	je conf_neg_EI
	div ebx
	add edx,0x30
	mov [ebp-12+edi],edx
	inc edi
	mov [ebp-16],edi
	jmp positivo_EI
conf_neg_EI:
	cmp ecx,0
	je imprime_EI
	mov byte [ebp-12+edi],0x2D
	inc edi
	jmp inicio_imprime
zero:
	add eax,0x30
	mov [ebp-12],eax
	inc edi
	mov [ebp-16],edi
	jmp inicio_imprime
inicio_imprime:
	mov esi,edi
imprime_EI:
	mov eax,4
	mov ebx,1
	mov ecx,ebp
	sub ecx,12
	add ecx,edi
	mov edx,1
	int 80h
	cmp edi,0
	je fim_imprime
	dec edi
	jmp imprime_EI
fim_imprime:
	cmp ecx,1
	je final_EI
	mov eax,4
	mov ebx,1
	mov ecx,newline
	mov edx,NWLINESIZE
	int 80h
final_EI:
	mov eax,[ebp-16]
	inc eax
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	leave
	ret 4

