BITS 32
section .text
global _start


_start:
	mov [hi], eax
while_loop_label0:
	mov eax, [hi]
	cmp eax, ebx
	setz al
	not al
	and eax, 1
	cmp eax, 0
	jz while_statement_end0
	mov ebx, 7
	mov [hello], ebx
	mov [hi], ebx
	jmp while_loop_label0
while_statement_end0:
	mov eax, 1
	xor ebx, ebx
	int 0x80

print_num:
	xor ecx, ecx
counter_label:
	inc ecx
	xor edx, edx
	div word [tenDividerForPrintingNumbers]
	push edx
	cmp eax, 0
	jg counter_label

putChar:
	pop edx
	add edx, '0'
	mov [savedMemoryForPrinting], edx
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, savedMemoryForPrinting
	mov edx, 2
	int 0x80
	popa
	loop putChar
	ret
print_new_line_label:
	mov	edx,1
	mov	ecx, newLineSavedMemory
	mov	ebx,1
	mov	eax,4
	int	0x80
	ret

section .data
	savedMemoryForPrinting db 100 dup(0)
	tenDividerForPrintingNumbers dw 10
	messageToPrint1 db 's', 0
	messageToPrint2 db 'v', 0
	messageToPrint3 db 'v', 0
	newLineSavedMemory db 10, 0
	hello dd 0
	hi db 0
