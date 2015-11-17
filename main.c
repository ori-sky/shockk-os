#include <stdint.h>
#include <ports.h>
#include <pic.h>

#define IRQ0 0x20        /* programmable interval timer */
#define IRQ1 (IRQ0 + 1)  /* keyboard */
#define IRQ2 (IRQ0 + 2)
#define IRQ3 (IRQ0 + 3)
#define IRQ4 (IRQ0 + 4)
#define IRQ5 (IRQ0 + 5)
#define IRQ6 (IRQ0 + 6)
#define IRQ7 (IRQ0 + 7)
#define IRQ8 0x28
#define IRQ9  (IRQ8 + 1)
#define IRQ10 (IRQ8 + 2)
#define IRQ11 (IRQ8 + 3)
#define IRQ12 (IRQ8 + 4)
#define IRQ13 (IRQ8 + 5)
#define IRQ14 (IRQ8 + 6)
#define IRQ15 (IRQ8 + 7)

extern void isr_stub_0(void);
extern void isr_stub_1(void);
extern void isr_stub_2(void);
extern void isr_stub_3(void);
extern void isr_stub_4(void);
extern void isr_stub_5(void);
extern void isr_stub_6(void);
extern void isr_stub_7(void);
extern void isr_stub_8(void);
extern void isr_stub_9(void);
extern void isr_stub_10(void);
extern void isr_stub_11(void);
extern void isr_stub_12(void);
extern void isr_stub_13(void);
extern void isr_stub_14(void);
extern void isr_stub_15(void);
extern void isr_stub_16(void);
extern void isr_stub_17(void);
extern void isr_stub_18(void);
extern void isr_stub_19(void);
extern void isr_stub_20(void);
extern void isr_stub_21(void);
extern void isr_stub_22(void);
extern void isr_stub_23(void);
extern void isr_stub_24(void);
extern void isr_stub_25(void);
extern void isr_stub_26(void);
extern void isr_stub_27(void);
extern void isr_stub_28(void);
extern void isr_stub_29(void);
extern void isr_stub_30(void);
extern void isr_stub_31(void);
extern void isr_stub_32(void);
extern void isr_stub_33(void);
extern void isr_stub_34(void);
extern void isr_stub_35(void);
extern void isr_stub_36(void);
extern void isr_stub_37(void);
extern void isr_stub_38(void);
extern void isr_stub_39(void);
extern void isr_stub_40(void);
extern void isr_stub_41(void);
extern void isr_stub_42(void);
extern void isr_stub_43(void);
extern void isr_stub_44(void);
extern void isr_stub_45(void);
extern void isr_stub_46(void);
extern void isr_stub_47(void);
extern void isr_stub_48(void);
extern void isr_stub_49(void);
extern void isr_stub_50(void);
extern void isr_stub_51(void);
extern void isr_stub_52(void);
extern void isr_stub_53(void);
extern void isr_stub_54(void);
extern void isr_stub_55(void);
extern void isr_stub_56(void);
extern void isr_stub_57(void);
extern void isr_stub_58(void);
extern void isr_stub_59(void);
extern void isr_stub_60(void);
extern void isr_stub_61(void);
extern void isr_stub_62(void);
extern void isr_stub_63(void);
extern void isr_stub_64(void);
extern void isr_stub_65(void);
extern void isr_stub_66(void);
extern void isr_stub_67(void);
extern void isr_stub_68(void);
extern void isr_stub_69(void);
extern void isr_stub_70(void);
extern void isr_stub_71(void);
extern void isr_stub_72(void);
extern void isr_stub_73(void);
extern void isr_stub_74(void);
extern void isr_stub_75(void);
extern void isr_stub_76(void);
extern void isr_stub_77(void);
extern void isr_stub_78(void);
extern void isr_stub_79(void);
extern void isr_stub_80(void);
extern void isr_stub_81(void);
extern void isr_stub_82(void);
extern void isr_stub_83(void);
extern void isr_stub_84(void);
extern void isr_stub_85(void);
extern void isr_stub_86(void);
extern void isr_stub_87(void);
extern void isr_stub_88(void);
extern void isr_stub_89(void);
extern void isr_stub_90(void);
extern void isr_stub_91(void);
extern void isr_stub_92(void);
extern void isr_stub_93(void);
extern void isr_stub_94(void);
extern void isr_stub_95(void);
extern void isr_stub_96(void);
extern void isr_stub_97(void);
extern void isr_stub_98(void);
extern void isr_stub_99(void);
extern void isr_stub_100(void);
extern void isr_stub_101(void);
extern void isr_stub_102(void);
extern void isr_stub_103(void);
extern void isr_stub_104(void);
extern void isr_stub_105(void);
extern void isr_stub_106(void);
extern void isr_stub_107(void);
extern void isr_stub_108(void);
extern void isr_stub_109(void);
extern void isr_stub_110(void);
extern void isr_stub_111(void);
extern void isr_stub_112(void);
extern void isr_stub_113(void);
extern void isr_stub_114(void);
extern void isr_stub_115(void);
extern void isr_stub_116(void);
extern void isr_stub_117(void);
extern void isr_stub_118(void);
extern void isr_stub_119(void);
extern void isr_stub_120(void);
extern void isr_stub_121(void);
extern void isr_stub_122(void);
extern void isr_stub_123(void);
extern void isr_stub_124(void);
extern void isr_stub_125(void);
extern void isr_stub_126(void);
extern void isr_stub_127(void);
extern void isr_stub_128(void);
extern void isr_stub_129(void);
extern void isr_stub_130(void);
extern void isr_stub_131(void);
extern void isr_stub_132(void);
extern void isr_stub_133(void);
extern void isr_stub_134(void);
extern void isr_stub_135(void);
extern void isr_stub_136(void);
extern void isr_stub_137(void);
extern void isr_stub_138(void);
extern void isr_stub_139(void);
extern void isr_stub_140(void);
extern void isr_stub_141(void);
extern void isr_stub_142(void);
extern void isr_stub_143(void);
extern void isr_stub_144(void);
extern void isr_stub_145(void);
extern void isr_stub_146(void);
extern void isr_stub_147(void);
extern void isr_stub_148(void);
extern void isr_stub_149(void);
extern void isr_stub_150(void);
extern void isr_stub_151(void);
extern void isr_stub_152(void);
extern void isr_stub_153(void);
extern void isr_stub_154(void);
extern void isr_stub_155(void);
extern void isr_stub_156(void);
extern void isr_stub_157(void);
extern void isr_stub_158(void);
extern void isr_stub_159(void);
extern void isr_stub_160(void);
extern void isr_stub_161(void);
extern void isr_stub_162(void);
extern void isr_stub_163(void);
extern void isr_stub_164(void);
extern void isr_stub_165(void);
extern void isr_stub_166(void);
extern void isr_stub_167(void);
extern void isr_stub_168(void);
extern void isr_stub_169(void);
extern void isr_stub_170(void);
extern void isr_stub_171(void);
extern void isr_stub_172(void);
extern void isr_stub_173(void);
extern void isr_stub_174(void);
extern void isr_stub_175(void);
extern void isr_stub_176(void);
extern void isr_stub_177(void);
extern void isr_stub_178(void);
extern void isr_stub_179(void);
extern void isr_stub_180(void);
extern void isr_stub_181(void);
extern void isr_stub_182(void);
extern void isr_stub_183(void);
extern void isr_stub_184(void);
extern void isr_stub_185(void);
extern void isr_stub_186(void);
extern void isr_stub_187(void);
extern void isr_stub_188(void);
extern void isr_stub_189(void);
extern void isr_stub_190(void);
extern void isr_stub_191(void);
extern void isr_stub_192(void);
extern void isr_stub_193(void);
extern void isr_stub_194(void);
extern void isr_stub_195(void);
extern void isr_stub_196(void);
extern void isr_stub_197(void);
extern void isr_stub_198(void);
extern void isr_stub_199(void);
extern void isr_stub_200(void);
extern void isr_stub_201(void);
extern void isr_stub_202(void);
extern void isr_stub_203(void);
extern void isr_stub_204(void);
extern void isr_stub_205(void);
extern void isr_stub_206(void);
extern void isr_stub_207(void);
extern void isr_stub_208(void);
extern void isr_stub_209(void);
extern void isr_stub_210(void);
extern void isr_stub_211(void);
extern void isr_stub_212(void);
extern void isr_stub_213(void);
extern void isr_stub_214(void);
extern void isr_stub_215(void);
extern void isr_stub_216(void);
extern void isr_stub_217(void);
extern void isr_stub_218(void);
extern void isr_stub_219(void);
extern void isr_stub_220(void);
extern void isr_stub_221(void);
extern void isr_stub_222(void);
extern void isr_stub_223(void);
extern void isr_stub_224(void);
extern void isr_stub_225(void);
extern void isr_stub_226(void);
extern void isr_stub_227(void);
extern void isr_stub_228(void);
extern void isr_stub_229(void);
extern void isr_stub_230(void);
extern void isr_stub_231(void);
extern void isr_stub_232(void);
extern void isr_stub_233(void);
extern void isr_stub_234(void);
extern void isr_stub_235(void);
extern void isr_stub_236(void);
extern void isr_stub_237(void);
extern void isr_stub_238(void);
extern void isr_stub_239(void);
extern void isr_stub_240(void);
extern void isr_stub_241(void);
extern void isr_stub_242(void);
extern void isr_stub_243(void);
extern void isr_stub_244(void);
extern void isr_stub_245(void);
extern void isr_stub_246(void);
extern void isr_stub_247(void);
extern void isr_stub_248(void);
extern void isr_stub_249(void);
extern void isr_stub_250(void);
extern void isr_stub_251(void);
extern void isr_stub_252(void);
extern void isr_stub_253(void);
extern void isr_stub_254(void);
extern void isr_stub_255(void);

struct IDTEntry {
	uint16_t baseAddressLow;
	uint16_t selector;
	uint8_t zero;
	uint8_t attributes;
	uint16_t baseAddressHigh;
} __attribute__((packed));

struct IDTDesc {
	uint16_t limiter;
	uint32_t baseAddress;
} __attribute__((packed));

struct CPUState {
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t interrupt;
	uint32_t error;
};

inline void set_entry(volatile struct IDTEntry *entry, void (*handler)(void), uint8_t attributes) {
	entry->baseAddressLow = handler;
	entry->selector = 0x8;
	entry->zero = 0;
	entry->attributes = attributes;
	entry->baseAddressHigh = 0;
}

void entry(void) {
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0xFC, 0xFF);
	//pic_set_masks(0, 0);

	volatile struct IDTEntry *idt = (struct IDTEntry *)0x800;
	set_entry(&idt[0], isr_stub_0, 0x8E);
	set_entry(&idt[1], isr_stub_1, 0x8E);
	set_entry(&idt[2], isr_stub_2, 0x8E);
	set_entry(&idt[3], isr_stub_3, 0x8E);
	set_entry(&idt[4], isr_stub_4, 0x8E);
	set_entry(&idt[5], isr_stub_5, 0x8E);
	set_entry(&idt[6], isr_stub_6, 0x8E);
	set_entry(&idt[7], isr_stub_7, 0x8E);
	set_entry(&idt[8], isr_stub_8, 0x8E);
	set_entry(&idt[9], isr_stub_9, 0x8E);
	set_entry(&idt[10], isr_stub_10, 0x8E);
	set_entry(&idt[11], isr_stub_11, 0x8E);
	set_entry(&idt[12], isr_stub_12, 0x8E);
	set_entry(&idt[13], isr_stub_13, 0x8E);
	set_entry(&idt[14], isr_stub_14, 0x8E);
	set_entry(&idt[15], isr_stub_15, 0x8E);
	set_entry(&idt[16], isr_stub_16, 0x8E);
	set_entry(&idt[17], isr_stub_17, 0x8E);
	set_entry(&idt[18], isr_stub_18, 0x8E);
	set_entry(&idt[19], isr_stub_19, 0x8E);
	set_entry(&idt[20], isr_stub_20, 0x8E);
	set_entry(&idt[21], isr_stub_21, 0x8E);
	set_entry(&idt[22], isr_stub_22, 0x8E);
	set_entry(&idt[23], isr_stub_23, 0x8E);
	set_entry(&idt[24], isr_stub_24, 0x8E);
	set_entry(&idt[25], isr_stub_25, 0x8E);
	set_entry(&idt[26], isr_stub_26, 0x8E);
	set_entry(&idt[27], isr_stub_27, 0x8E);
	set_entry(&idt[28], isr_stub_28, 0x8E);
	set_entry(&idt[29], isr_stub_29, 0x8E);
	set_entry(&idt[30], isr_stub_30, 0x8E);
	set_entry(&idt[31], isr_stub_31, 0x8E);
	set_entry(&idt[32], isr_stub_32, 0x8E);
	set_entry(&idt[33], isr_stub_33, 0x8E);
	set_entry(&idt[34], isr_stub_34, 0x8E);
	set_entry(&idt[35], isr_stub_35, 0x8E);
	set_entry(&idt[36], isr_stub_36, 0x8E);
	set_entry(&idt[37], isr_stub_37, 0x8E);
	set_entry(&idt[38], isr_stub_38, 0x8E);
	set_entry(&idt[39], isr_stub_39, 0x8E);
	set_entry(&idt[40], isr_stub_40, 0x8E);
	set_entry(&idt[41], isr_stub_41, 0x8E);
	set_entry(&idt[42], isr_stub_42, 0x8E);
	set_entry(&idt[43], isr_stub_43, 0x8E);
	set_entry(&idt[44], isr_stub_44, 0x8E);
	set_entry(&idt[45], isr_stub_45, 0x8E);
	set_entry(&idt[46], isr_stub_46, 0x8E);
	set_entry(&idt[47], isr_stub_47, 0x8E);
	set_entry(&idt[48], isr_stub_48, 0x8E);
	set_entry(&idt[49], isr_stub_49, 0x8E);
	set_entry(&idt[50], isr_stub_50, 0x8E);
	set_entry(&idt[51], isr_stub_51, 0x8E);
	set_entry(&idt[52], isr_stub_52, 0x8E);
	set_entry(&idt[53], isr_stub_53, 0x8E);
	set_entry(&idt[54], isr_stub_54, 0x8E);
	set_entry(&idt[55], isr_stub_55, 0x8E);
	set_entry(&idt[56], isr_stub_56, 0x8E);
	set_entry(&idt[57], isr_stub_57, 0x8E);
	set_entry(&idt[58], isr_stub_58, 0x8E);
	set_entry(&idt[59], isr_stub_59, 0x8E);
	set_entry(&idt[60], isr_stub_60, 0x8E);
	set_entry(&idt[61], isr_stub_61, 0x8E);
	set_entry(&idt[62], isr_stub_62, 0x8E);
	set_entry(&idt[63], isr_stub_63, 0x8E);
	set_entry(&idt[64], isr_stub_64, 0x8E);
	set_entry(&idt[65], isr_stub_65, 0x8E);
	set_entry(&idt[66], isr_stub_66, 0x8E);
	set_entry(&idt[67], isr_stub_67, 0x8E);
	set_entry(&idt[68], isr_stub_68, 0x8E);
	set_entry(&idt[69], isr_stub_69, 0x8E);
	set_entry(&idt[70], isr_stub_70, 0x8E);
	set_entry(&idt[71], isr_stub_71, 0x8E);
	set_entry(&idt[72], isr_stub_72, 0x8E);
	set_entry(&idt[73], isr_stub_73, 0x8E);
	set_entry(&idt[74], isr_stub_74, 0x8E);
	set_entry(&idt[75], isr_stub_75, 0x8E);
	set_entry(&idt[76], isr_stub_76, 0x8E);
	set_entry(&idt[77], isr_stub_77, 0x8E);
	set_entry(&idt[78], isr_stub_78, 0x8E);
	set_entry(&idt[79], isr_stub_79, 0x8E);
	set_entry(&idt[80], isr_stub_80, 0x8E);
	set_entry(&idt[81], isr_stub_81, 0x8E);
	set_entry(&idt[82], isr_stub_82, 0x8E);
	set_entry(&idt[83], isr_stub_83, 0x8E);
	set_entry(&idt[84], isr_stub_84, 0x8E);
	set_entry(&idt[85], isr_stub_85, 0x8E);
	set_entry(&idt[86], isr_stub_86, 0x8E);
	set_entry(&idt[87], isr_stub_87, 0x8E);
	set_entry(&idt[88], isr_stub_88, 0x8E);
	set_entry(&idt[89], isr_stub_89, 0x8E);
	set_entry(&idt[90], isr_stub_90, 0x8E);
	set_entry(&idt[91], isr_stub_91, 0x8E);
	set_entry(&idt[92], isr_stub_92, 0x8E);
	set_entry(&idt[93], isr_stub_93, 0x8E);
	set_entry(&idt[94], isr_stub_94, 0x8E);
	set_entry(&idt[95], isr_stub_95, 0x8E);
	set_entry(&idt[96], isr_stub_96, 0x8E);
	set_entry(&idt[97], isr_stub_97, 0x8E);
	set_entry(&idt[98], isr_stub_98, 0x8E);
	set_entry(&idt[99], isr_stub_99, 0x8E);
	set_entry(&idt[100], isr_stub_100, 0x8E);
	set_entry(&idt[101], isr_stub_101, 0x8E);
	set_entry(&idt[102], isr_stub_102, 0x8E);
	set_entry(&idt[103], isr_stub_103, 0x8E);
	set_entry(&idt[104], isr_stub_104, 0x8E);
	set_entry(&idt[105], isr_stub_105, 0x8E);
	set_entry(&idt[106], isr_stub_106, 0x8E);
	set_entry(&idt[107], isr_stub_107, 0x8E);
	set_entry(&idt[108], isr_stub_108, 0x8E);
	set_entry(&idt[109], isr_stub_109, 0x8E);
	set_entry(&idt[110], isr_stub_110, 0x8E);
	set_entry(&idt[111], isr_stub_111, 0x8E);
	set_entry(&idt[112], isr_stub_112, 0x8E);
	set_entry(&idt[113], isr_stub_113, 0x8E);
	set_entry(&idt[114], isr_stub_114, 0x8E);
	set_entry(&idt[115], isr_stub_115, 0x8E);
	set_entry(&idt[116], isr_stub_116, 0x8E);
	set_entry(&idt[117], isr_stub_117, 0x8E);
	set_entry(&idt[118], isr_stub_118, 0x8E);
	set_entry(&idt[119], isr_stub_119, 0x8E);
	set_entry(&idt[120], isr_stub_120, 0x8E);
	set_entry(&idt[121], isr_stub_121, 0x8E);
	set_entry(&idt[122], isr_stub_122, 0x8E);
	set_entry(&idt[123], isr_stub_123, 0x8E);
	set_entry(&idt[124], isr_stub_124, 0x8E);
	set_entry(&idt[125], isr_stub_125, 0x8E);
	set_entry(&idt[126], isr_stub_126, 0x8E);
	set_entry(&idt[127], isr_stub_127, 0x8E);
	set_entry(&idt[128], isr_stub_128, 0x8E);
	set_entry(&idt[129], isr_stub_129, 0x8E);
	set_entry(&idt[130], isr_stub_130, 0x8E);
	set_entry(&idt[131], isr_stub_131, 0x8E);
	set_entry(&idt[132], isr_stub_132, 0x8E);
	set_entry(&idt[133], isr_stub_133, 0x8E);
	set_entry(&idt[134], isr_stub_134, 0x8E);
	set_entry(&idt[135], isr_stub_135, 0x8E);
	set_entry(&idt[136], isr_stub_136, 0x8E);
	set_entry(&idt[137], isr_stub_137, 0x8E);
	set_entry(&idt[138], isr_stub_138, 0x8E);
	set_entry(&idt[139], isr_stub_139, 0x8E);
	set_entry(&idt[140], isr_stub_140, 0x8E);
	set_entry(&idt[141], isr_stub_141, 0x8E);
	set_entry(&idt[142], isr_stub_142, 0x8E);
	set_entry(&idt[143], isr_stub_143, 0x8E);
	set_entry(&idt[144], isr_stub_144, 0x8E);
	set_entry(&idt[145], isr_stub_145, 0x8E);
	set_entry(&idt[146], isr_stub_146, 0x8E);
	set_entry(&idt[147], isr_stub_147, 0x8E);
	set_entry(&idt[148], isr_stub_148, 0x8E);
	set_entry(&idt[149], isr_stub_149, 0x8E);
	set_entry(&idt[150], isr_stub_150, 0x8E);
	set_entry(&idt[151], isr_stub_151, 0x8E);
	set_entry(&idt[152], isr_stub_152, 0x8E);
	set_entry(&idt[153], isr_stub_153, 0x8E);
	set_entry(&idt[154], isr_stub_154, 0x8E);
	set_entry(&idt[155], isr_stub_155, 0x8E);
	set_entry(&idt[156], isr_stub_156, 0x8E);
	set_entry(&idt[157], isr_stub_157, 0x8E);
	set_entry(&idt[158], isr_stub_158, 0x8E);
	set_entry(&idt[159], isr_stub_159, 0x8E);
	set_entry(&idt[160], isr_stub_160, 0x8E);
	set_entry(&idt[161], isr_stub_161, 0x8E);
	set_entry(&idt[162], isr_stub_162, 0x8E);
	set_entry(&idt[163], isr_stub_163, 0x8E);
	set_entry(&idt[164], isr_stub_164, 0x8E);
	set_entry(&idt[165], isr_stub_165, 0x8E);
	set_entry(&idt[166], isr_stub_166, 0x8E);
	set_entry(&idt[167], isr_stub_167, 0x8E);
	set_entry(&idt[168], isr_stub_168, 0x8E);
	set_entry(&idt[169], isr_stub_169, 0x8E);
	set_entry(&idt[170], isr_stub_170, 0x8E);
	set_entry(&idt[171], isr_stub_171, 0x8E);
	set_entry(&idt[172], isr_stub_172, 0x8E);
	set_entry(&idt[173], isr_stub_173, 0x8E);
	set_entry(&idt[174], isr_stub_174, 0x8E);
	set_entry(&idt[175], isr_stub_175, 0x8E);
	set_entry(&idt[176], isr_stub_176, 0x8E);
	set_entry(&idt[177], isr_stub_177, 0x8E);
	set_entry(&idt[178], isr_stub_178, 0x8E);
	set_entry(&idt[179], isr_stub_179, 0x8E);
	set_entry(&idt[180], isr_stub_180, 0x8E);
	set_entry(&idt[181], isr_stub_181, 0x8E);
	set_entry(&idt[182], isr_stub_182, 0x8E);
	set_entry(&idt[183], isr_stub_183, 0x8E);
	set_entry(&idt[184], isr_stub_184, 0x8E);
	set_entry(&idt[185], isr_stub_185, 0x8E);
	set_entry(&idt[186], isr_stub_186, 0x8E);
	set_entry(&idt[187], isr_stub_187, 0x8E);
	set_entry(&idt[188], isr_stub_188, 0x8E);
	set_entry(&idt[189], isr_stub_189, 0x8E);
	set_entry(&idt[190], isr_stub_190, 0x8E);
	set_entry(&idt[191], isr_stub_191, 0x8E);
	set_entry(&idt[192], isr_stub_192, 0x8E);
	set_entry(&idt[193], isr_stub_193, 0x8E);
	set_entry(&idt[194], isr_stub_194, 0x8E);
	set_entry(&idt[195], isr_stub_195, 0x8E);
	set_entry(&idt[196], isr_stub_196, 0x8E);
	set_entry(&idt[197], isr_stub_197, 0x8E);
	set_entry(&idt[198], isr_stub_198, 0x8E);
	set_entry(&idt[199], isr_stub_199, 0x8E);
	set_entry(&idt[200], isr_stub_200, 0x8E);
	set_entry(&idt[201], isr_stub_201, 0x8E);
	set_entry(&idt[202], isr_stub_202, 0x8E);
	set_entry(&idt[203], isr_stub_203, 0x8E);
	set_entry(&idt[204], isr_stub_204, 0x8E);
	set_entry(&idt[205], isr_stub_205, 0x8E);
	set_entry(&idt[206], isr_stub_206, 0x8E);
	set_entry(&idt[207], isr_stub_207, 0x8E);
	set_entry(&idt[208], isr_stub_208, 0x8E);
	set_entry(&idt[209], isr_stub_209, 0x8E);
	set_entry(&idt[210], isr_stub_210, 0x8E);
	set_entry(&idt[211], isr_stub_211, 0x8E);
	set_entry(&idt[212], isr_stub_212, 0x8E);
	set_entry(&idt[213], isr_stub_213, 0x8E);
	set_entry(&idt[214], isr_stub_214, 0x8E);
	set_entry(&idt[215], isr_stub_215, 0x8E);
	set_entry(&idt[216], isr_stub_216, 0x8E);
	set_entry(&idt[217], isr_stub_217, 0x8E);
	set_entry(&idt[218], isr_stub_218, 0x8E);
	set_entry(&idt[219], isr_stub_219, 0x8E);
	set_entry(&idt[220], isr_stub_220, 0x8E);
	set_entry(&idt[221], isr_stub_221, 0x8E);
	set_entry(&idt[222], isr_stub_222, 0x8E);
	set_entry(&idt[223], isr_stub_223, 0x8E);
	set_entry(&idt[224], isr_stub_224, 0x8E);
	set_entry(&idt[225], isr_stub_225, 0x8E);
	set_entry(&idt[226], isr_stub_226, 0x8E);
	set_entry(&idt[227], isr_stub_227, 0x8E);
	set_entry(&idt[228], isr_stub_228, 0x8E);
	set_entry(&idt[229], isr_stub_229, 0x8E);
	set_entry(&idt[230], isr_stub_230, 0x8E);
	set_entry(&idt[231], isr_stub_231, 0x8E);
	set_entry(&idt[232], isr_stub_232, 0x8E);
	set_entry(&idt[233], isr_stub_233, 0x8E);
	set_entry(&idt[234], isr_stub_234, 0x8E);
	set_entry(&idt[235], isr_stub_235, 0x8E);
	set_entry(&idt[236], isr_stub_236, 0x8E);
	set_entry(&idt[237], isr_stub_237, 0x8E);
	set_entry(&idt[238], isr_stub_238, 0x8E);
	set_entry(&idt[239], isr_stub_239, 0x8E);
	set_entry(&idt[240], isr_stub_240, 0x8E);
	set_entry(&idt[241], isr_stub_241, 0x8E);
	set_entry(&idt[242], isr_stub_242, 0x8E);
	set_entry(&idt[243], isr_stub_243, 0x8E);
	set_entry(&idt[244], isr_stub_244, 0x8E);
	set_entry(&idt[245], isr_stub_245, 0x8E);
	set_entry(&idt[246], isr_stub_246, 0x8E);
	set_entry(&idt[247], isr_stub_247, 0x8E);
	set_entry(&idt[248], isr_stub_248, 0x8E);
	set_entry(&idt[249], isr_stub_249, 0x8E);
	set_entry(&idt[250], isr_stub_250, 0x8E);
	set_entry(&idt[251], isr_stub_251, 0x8E);
	set_entry(&idt[252], isr_stub_252, 0x8E);
	set_entry(&idt[253], isr_stub_253, 0x8E);
	set_entry(&idt[254], isr_stub_254, 0x8E);
	set_entry(&idt[255], isr_stub_255, 0x8E);

	volatile struct IDTDesc *idt_desc = (struct IDTDesc *)0x7FA;
	idt_desc->limiter = 0x800;
	idt_desc->baseAddress = 0x800;

	__asm__ __volatile__ ("lidt 0x7FA" ::: "memory");
	__asm__ __volatile__ ("sti");

	volatile unsigned char *ptr = (unsigned char *)0xB8000;
	for(unsigned short col = 0; col < 80 * 25; ++col) {
		ptr[col << 1] = 'A' + col % 80 % 26;
	}

	for(;;) {
		__asm__ __volatile__ ("hlt");
	}
}

char * uitoa(unsigned int value, char *str, int base) {
	unsigned char log = 0;
	for(unsigned int n=value; n>=base; n/=base) { ++log; }

	str[log+1] = 0;

	unsigned short div = 1;
	for(unsigned char i=log; i!=(unsigned char)(-1); --i, div*=base) {
		unsigned char offset = value / div % base;
		unsigned char base = offset < 10 ? '0' : 'a' - 10;
		str[i] = base + offset;
	}

	return str;
}

void isr_main(struct CPUState cpu_state) {
	if(cpu_state.interrupt == IRQ7) { return; }
	if(cpu_state.interrupt >= IRQ0) {
		ports_outb(PIC_PORT_MASTER, 0x20);
		if(cpu_state.interrupt == IRQ15) { return; }
		if(cpu_state.interrupt >= IRQ8) {
			ports_outb(PIC_PORT_SLAVE, 0x20);
		}
	}

	switch(cpu_state.interrupt) {
	case IRQ0:
		return;
	case IRQ1:
		ports_inb(0x60);
		break;
	}

	volatile unsigned char *ptr = (unsigned char *)0xB8000;
	volatile unsigned int *i = (unsigned char *)0x4000;
	ptr[0] = 'a' + (*i)++ % 26;

	char interrupt_string[3];
	uitoa(cpu_state.interrupt, interrupt_string, 16);
	ptr[80 * 2] = '0';
	ptr[81 * 2] = 'x';
	ptr[82 * 2] = interrupt_string[0];
	ptr[83 * 2] = interrupt_string[1];
	ptr[84 * 2] = interrupt_string[2];
}
