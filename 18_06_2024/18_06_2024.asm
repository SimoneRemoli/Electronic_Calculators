.org 0x800
.data
  .equ $pulsante_irq, 0x00 #periferica IVN ma non programmabile, senza STATUS, solo IRQ
  .equ $sorgente_reg, 0xabc
  .equ $sorgente_status, 0xfff
  .equ $destinazione_reg, 0xabc
  .equ $destinazione_status, 0xbvc
  buffer: .fill 131072,8. #1 MB (megabyte) = 1.048.576 byte e trasferisco 131072 volte 8 byte
  indice: .byte 0 #indice che conta fino a 1024 MB di scrittura
.text
main:
  sti
  .loop:
    jmp .loop



  setting:
    push %rax (#eseguita all'interno di un contesto di interrupt)
    push %rcx (#eseguita all'interno di un contesto di interrupt)

    push %r8
    movb %dl, %cl
    out %al, %cl (#zittisco la richiesta di interruzione)
    xorq %r8, %r8

  .ricomincia:
    .acquisizione:
    xorq %rax, %rax
    movb $1, %al
    outb %al, $sorgente_status (#avvio la periferica sorgente di produrre nuovi dati da 8 byte)
    .bw1:
      inb $sorgente_status, %al
      btb $0, $sorgente_status
      jnc .bw1
    inq $sorgente_reg, %rax #prelevo il dato da 8 byte dalla sorgente
    movq %rax, %rcx #i dati stanno in rcx
    movq %rcx, buffer(, %r8, 8)
    addq $1, %r8
    cmpq $131072, %r8 #devo arrivare ad 1 MB
    jnz .acquisizione
    (#ho copiato un megabyte di dati in un buffer contenuto in memoria RAM)

    #ora devo copiare questi dati nella destinazione
    outb %al, $destinazione_status #attivo la periferica destinazione
    #faccio capire alla periferica di output destinazione che intendo effettuare
    #un'interazione con lei di scambio dati, in modo particolare scrittura

    xorq %r8, %r8 #riazzero il registro indice

    .ciclo:
     xorq %rax, %rax #ripulisco il registro ad ogni scrittura
     movq buffer(,%r8,8), %rax

     outq %rax, $destinazione_reg #mando il dato

     .bw2
       inb $destinazione_status, %al
       btb $0, %al
       jnc .bw2
     #il dato quì sarà sicuramente consumato
     addq $1, %r8
     cmpq $131072, %r8 #vediamo se abbiamo copiato effettivamente un megabyte
     jnz .ciclo
     #ho copiato 1MB da src a dst.
     #però dobbiamo copiare fino ad 1 GB = 1024 MB

     addb $1, indice
     cmpq $1024, indice #ho copiato 1GB?
     jnz .ricomincia
     pop %r8
     pop %rcx
     pop %rax
     ret

  .driver 0 (#driver pulsante)
    push %rdx
    movq $pulsante_irq, %rdx
    call setting
    pop %rdx
    iret
