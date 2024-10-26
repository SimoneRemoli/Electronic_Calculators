.org 0x800
.data
  .equ $reg_ricevitore, 0x01
  .equ $status_ricevitore, 0x02
  .equ $irq_ricevitore, 0x03
  .equ $reg_ventilatore, 0x04
  .equ $status_ventilatore, 0x05
  .equ $status_termometro, 0x06
  .equ $reg_termometro, 0x07
  .equ $deumidificatore, 0x08
  .equ $condizionatore, 0x09
  temperatura: .byte 0
  impostazione: .byte 0
  ventilatore_switch: .byte 0
  deumidificatore_switch: .byte 0
  condizionatore_switch: .byte 0
  temperatura_richiesta: .byte 0
  velocita_ventilatore: .byte 0


  
.text
main:
  outb %al, $status_ricevitore #attivo la periferica ricevitore 
  .acquisizione
  outb %al, $status_termometro (#comunico che voglio effettuare un trasferimento dati su term.)
  .bw1:
    inb $status_termometro, %al
    btb $0, %al 
    jnc .bw1
  #il dato è pronto e leggo la temperatura 
  inb $reg_termometro, %al 
  movb %al, temperatura
  
  
  jmp .acquisizione
  
  .velocita_1:
    movb $1, velocita_ventilatore
    jmp .attivazione 
  .velocita_2:
    movb $2, velocita_ventilatore
    jmp .attivazione
    
  .attivazione:
    outb %al, $status_ventilatore #voglio effettuare interazione con ventilatore
    movb velocita_ventilatore, %al #scrivo il dato 
    outb %al, $reg_ventilatore #scrivo il dato 
    .bw2:
      inb $status_ventilatore, %al
      btb $0, %al 
      jnc .bw2
      #il dato è consumato e quindi il ventilatore è stato attivato con una certa velocità
      jmp .return
  
  
  
    
  .attiva_deumidificatore:
    outb %al, $deumidificatore #interfaccia booleana semplice 
    jmp .return
  .attiva_condizionatore:
    outb %al, $condizionatore
    jmp .return
  .attiva_ventilatore:
    subb temperatura_richiesta, temperatura (#temperatura = temperatura - temperatura_richiesta)
   (si suppone che le temperature siano numeri positivi)
   cmpb $4, temperatura (#se dest<sorg)
   js .velocita_1
   cmpb $8, temperatura
   jz .velocita_2 (#perchè anche se è uguale ad 8 deve essere attivato a velocità 2)
   js .velocita_2

   
    
  setting:
    push %rax
    push %rdx
    movb %dil, %dl
    outb %al, %dl #zittisco la richiesta di interruzione 
    inb $reg_ricevitore, %al 
    movb %al, impostazione
    movb impostazione, ventilatore_switch
    movb impostazione, deumidificatore_switch
    movb impostazione, condizionatore_switch
    movb impostazione, temperatura_richiesta
    andb $0x80, ventilatore_switch #bit sette = 1 -> attivo la modalità ventilatore
    andb $0x40, deumidificatore_switch #bit sei = 1 -> attivo deumidificatore
    andb $0x20, condizionatore_switch #bit cinque = 1 -> attivo condizionatore
    andb $0x1F, temperatura_richiesta #temperatura richiesta
    cmpb $1, deumidificatore_switch
    jz .attiva_deumidificatore
    cmpb $1, condizionatore_switch
    jz .attiva_condizionatore
    cmpb $1, $status_ventilatore
    jz .attiva_ventilatore 
    .return:
    pop %rdx
    pop %rax
    ret
    
    
    
    
    
    
    
  .driver 0 (#driver del ricevitore)
    push %rdi
    push %rsi 
    movq $irq_ricevitore, %rdi
    movq $status_ricevitore, %rsi 
    call setting
    pop %rsi
    pop %rdi
    iret
  

    
    