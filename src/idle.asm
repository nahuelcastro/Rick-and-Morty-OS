; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

ORG 0x00018000 ; TASK_IDLE_CODE

BITS 32

%include "print.mac"

idle:
    .loop:
        inc dword [number]
        cmp dword [number], 0x4
        jb .print

    .counter_reset:
        mov dword [number], 0x0

    .print:
        mov ebx, dword [number]
        add ebx, clock
        print_text_pm ebx, 1, 0x0f, 49, 76
        mov ebx, open
        print_text_pm ebx, 1, 0x0f, 49, 76-1
        mov ebx, close
        print_text_pm ebx, 1, 0x0f, 49, 76+1

    jmp .loop

number:   dd 0x00000000

clock: db '|/-\'

open: db '('
close: db ')'
