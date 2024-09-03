.org 0x800
.data
	var:.byte -1
	var2: .byte 127 #se 255 non salta, se 127 salta, allarga la rappr



.text
main:
movb var, %al
andb $0x7F, %al
cmpb var,var2
jc .label
return:
hlt

.label:
movb $10,%cl




