BITS 32
section .text
global _start
section .data
	savedMemoryForPrinting db 100 dup(0)
	tenDividerForPrintingNumbers dw 10
	ch dd 0
_start:
	mov eax, 21
	mov [ch], eax
	mov eax, 5
loop_start0:
	mov edx, 0
	div word [tenDividerForPrintingNumbers]
	add edx, '0'
	cmp eax, 0
	jne loop_start0
	mov eax, 4
	mov ebx, 1
	int 0x80
	mov ebx, ch
	mov [savedMemoryForPrinting], ebx
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
