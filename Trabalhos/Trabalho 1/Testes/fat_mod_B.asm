MOD_B: BEGIN
SECTION TEXT
	FAT: EXTERN
	N: EXTERN
	PUBLIC MOD_A
	STORE AUX
	MULT N
	STORE N
	LOAD AUX
	JMP FAT
SECTION BSS
	AUX: SPACE
END
