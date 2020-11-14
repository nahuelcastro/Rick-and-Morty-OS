; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

extern GDT_DESC
%define CS_LVL_0 0x50
%define DATO_LVL_0 0x58
%define EBP_BASE 0x25000
%define VIDEO_LVL_0 0x70


global start

extern GDT_DESC
extern idt_init
extern IDT_DESC

extern pic_enable
extern pic_reset

BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:

    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0



    ; Habilitar A20
    call A20_disable
    call A20_check
    call A20_enable
    call A20_check

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, CR0
    or eax, 1
    mov CR0, eax

    ; Saltar a modo protegido
    jmp CS_LVL_0:modo_protegido

    ; sacar lo de abajo
    ; [15 - 3] = Índice de la tabla en el que se encuentra el descriptor del segmento al que quiero saltar
    ; [2] = Bit que indica si trabajo con la GDT o con la LDT.
    ; [1 - 0] = Nivel de privilegio (0 = Kernel, 3 = Usuario)

BITS 32 ;A partir de aca codifica en 32 bits
modo_protegido:
    xchg bx, bx
    ; Establecer selectores de segmentos
    xor eax, eax
    mov ax, DATO_LVL_0
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov ss, ax
    mov ax, VIDEO_LVL_0
    mov fs, ax


    ; Establecer la base de la pila
    mov ebp, EBP_BASE
    mov esp, EBP_BASE

    ; Imprimir mensaje de bienvenida
    print_text_rm start_pm_msg, start_pm_len, 0x07, 0, 0

    ; Inicializar pantalla
    call init_pantalla


    ; Inicializar el manejador de memoria

    ; Inicializar el directorio de paginas

    ; Cargar directorio de paginas

    ; Habilitar paginacion

    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    xchg bx, bx

    ; Inicializar la IDT
    call idt_init

    ; Cargar IDT
    lidt [IDT_DESC]


    ;Configuarar controlador de interrupciones
    call pic_reset
    call pic_enable

    ; Cargar tarea inicial


    ;Habilitar interrupciones
    sti

    ;xor eax, eax
    ;idiv eax    ;division por 0


    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    ; mov eax, 0xFFFF
    ; mov ebx, 0xFFFF
    ; mov ecx, 0xFFFF
    ; mov edx, 0xFFFF
    jmp $

%define BLACK              (0x0 << 12)
%define BLUE               (0x1 << 12)
%define GREEN              (0x2 << 12)
%define CYAN               (0x3 << 12)
%define RED                (0x4 << 12)
%define MAGENTA            (0x5 << 12)
%define BROWN              (0x6 << 12)
%define LIGHT_GREY         (0x7 << 12)


init_pantalla:

xor ecx, ecx

primer_fila_negra:
    mov word [fs:2*ecx], BLACK
    inc ecx
    cmp ecx, 80
    jne primer_fila_negra

filas_verdes:
    mov word [fs:2*ecx], GREEN
    inc ecx
    cmp ecx, 3200
    jne filas_verdes

xor ebx, ebx
mov ebx, ecx

filas_negras:
    mov word [fs:2*ecx], BLACK
    inc ecx
    cmp ecx, 4000
    jne filas_negras


add ebx, 168  ; dejo una fila mas de margen
mov ecx, ebx
xor edx, edx

tableros:

    mov ebx, ecx
    add ebx, 12
    fila_tablero_rojo:
        mov word [fs:2*ecx], RED
        inc ecx
        cmp ecx, ebx
        jne fila_tablero_rojo

    add ecx, 40
    mov ebx, ecx
    add ebx, 12

    fila_tablero_azul:
        mov word [fs:2*ecx], BLUE
        inc ecx
        cmp ecx, ebx
        jne fila_tablero_azul

    add ecx, 16
    inc edx
    cmp edx, 3
    jne tableros

ret





;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
