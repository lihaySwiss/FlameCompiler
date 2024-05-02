BITS 32
section .text
global _start
section .data
	savedMemoryForPrinting db 100 dup(0)
	tenDividerForPrintingNumbers dw 10
	g_ dd 0
	gg db 0
_start:
	mov eax, 'd'
	mov [gg], eax
	mov eax, 0
	mov [g_], eax
	mov eax, gg
	mov [g_], eax
	mov eax, ':(dsd'
	mov [savedMemoryForPrinting], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, savedMemoryForPrinting
	mov edx, 2
	int 0x80
	popa
	mov eax, ' h'
	mov [savedMemoryForPrinting], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, savedMemoryForPrinting
	mov edx, 2
	int 0x80
	popa
	mov eax, 'e'
	mov [savedMemoryForPrinting], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, savedMemoryForPrinting
	mov edx, 2
	int 0x80
	popa
	mov eax, 'l'
	mov [savedMemoryForPrinting], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, savedMemoryForPrinting
	mov edx, 2
	int 0x80
	popa
	mov eax, 'l'
	mov [savedMemoryForPrinting], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, savedMemoryForPrinting
	mov edx, 2
	int 0x80
	popa
	mov eax, 'o '
	mov [savedMemoryForPrinting], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, savedMemoryForPrinting
	mov edx, 2
	int 0x80
	popa
	mov eax, 1
	mov ebx, 0
	int 0x80
