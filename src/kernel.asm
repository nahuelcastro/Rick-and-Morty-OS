; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

extern GDT_DESC

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
    or  eax, 0x1
    mov CR0, eax
    
    ; Saltar a modo protegido
    jmp 0x50:modo_protedigo     ;[0000000001010-0-00]
    ;explicacion, ver definicion de selecctor de segmento, tamaño 16 bits
    ;salta al primer segmento de codigo, habria que ver bien porque, pero saltan al de cogido 
    ;y abajo setean el de datos (DS)  y el resto de los registros de segmento
    ;buscar buena explicacion en teorica

    ; sacar lo de abajo
    ; [15 - 3] = Índice de la tabla en el que se encuentra el descriptor del segmento al que quiero saltar
    ; [2] = Bit que indica si trabajo con la GDT o con la LDT.
    ; [1 - 0] = Nivel de privilegio (0 = Kernel, 3 = Usuario)


BITS 32
modo_protedigo:

    ; Establecer selectores de segmentos
    xor eax, eax    ;Vacío eax
    mov ax, 0x58    ;Muevo a AX un selector para el descriptor del segmento 11, pero dejando los primeros 3 bits en 0
    mov ds, ax      ;Pongo el resto de los registros de segmento en el mismo segmento.
    mov es, ax      ;;;;; VER PORQUE SE LES PONE A TODOS EL MISMO VALOR, CREO QUE ES PORQUE NO IMPORTA DEMASIADO QUE TIENEN
    mov gs, ax      
    mov ss, ax      
    ;mov fs, ax    
    mov ax, 1110000b ; 14 shifetado 3 a izquierda por el segmento de video 
    mov fs, ax

;el reg CS no lo modifico acá, sino que lo hice con el far jump de arriba.

    ; Establecer la base de la pila
    mov ebp, 0x25000    
    mov esp, ebp        ;Pongo el tope de la pila en la base de la pila.
    
    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 0, 0    ;chequear si es asi

    xchg bx,bx 

    ; Inicializar pantalla
    call init_pantalla  ; solo le puse el nombre hay que implementarla jeje    

    
    ; Inicializar el manejador de memoria
 
    ; Inicializar el directorio de paginas
    
    ; Cargar directorio de paginas

    ; Habilitar paginacion
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    
    ; Cargar IDT
 
    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $



%define C_BG_BLACK              (0x0 << 12)
%define C_BG_BLUE               (0x1 << 12)
%define C_BG_GREEN              (0x2 << 12)
%define C_BG_CYAN               (0x3 << 12)
%define C_BG_RED                (0x4 << 12)
%define C_BG_MAGENTA            (0x5 << 12)
%define C_BG_BROWN              (0x6 << 12)
%define C_BG_LIGHT_GREY         (0x7 << 12)


init_pantalla:

xor ecx, ecx

primer_fila_negra:               
    mov word [fs:2*ecx], C_BG_BLACK
    inc ecx
    cmp ecx, 80       
    jne primer_fila_negra

filas_verdes:
    mov word [fs:2*ecx], C_BG_GREEN
    inc ecx
    cmp ecx, 3200 ;3120
    jne filas_verdes

xor ebx, ebx
mov ebx, ecx

filas_negras:
    mov word [fs:2*ecx], C_BG_BLACK
    inc ecx
    cmp ecx, 4000 ;3120
    jne filas_negras


add ebx, 168  ; dejo una fila mas de margen 
mov ecx, ebx
xor edx, edx

tableros:   
    
    mov ebx, ecx
    add ebx, 12
    fila_tablero_rojo:
        mov word [fs:2*ecx], C_BG_RED
        inc ecx
        cmp ecx, ebx
        jne fila_tablero_rojo
    
    add ecx, 40
    mov ebx, ecx
    add ebx, 12

    fila_tablero_azul:
        mov word [fs:2*ecx], C_BG_BLUE
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