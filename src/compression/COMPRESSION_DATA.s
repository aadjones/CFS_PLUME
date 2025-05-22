	.arch armv8.4-a+fp16+sb+ssbs
	.build_version macos,  15, 0
	.text
	.align	2
	.p2align 5,,15
	.globl __ZNKSt5ctypeIcE8do_widenEc
	.weak_definition __ZNKSt5ctypeIcE8do_widenEc
__ZNKSt5ctypeIcE8do_widenEc:
LFB2436:
	mov	w0, w1
	ret
LFE2436:
	.align	2
	.p2align 5,,15
__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0:
LFB9861:
	stp	x29, x30, [sp, -32]!
LCFI0:
	mov	x29, sp
LCFI1:
	stp	x19, x20, [sp, 16]
LCFI2:
	mov	x19, x0
	ldr	x0, [x0]
	ldr	x0, [x0, -24]
	add	x0, x19, x0
	ldr	x20, [x0, 240]
	cbz	x20, L9
	ldrsb	w0, [x20, 56]
	cbz	w0, L5
	ldrsb	w1, [x20, 67]
L6:
	mov	x0, x19
	bl	__ZNSo3putEc
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
LCFI3:
	b	__ZNSo5flushEv
	.p2align 2,,3
L5:
LCFI4:
	mov	x0, x20
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x20]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L6
	mov	x0, x20
	blr	x2
	mov	w1, w0
	b	L6
L9:
	bl	__ZSt16__throw_bad_castv
LFE9861:
	.section __TEXT,__text_cold,regular,pure_instructions
	.align	2
	.globl __ZN5Eigen8internal19throw_std_bad_allocEv
	.weak_definition __ZN5Eigen8internal19throw_std_bad_allocEv
__ZN5Eigen8internal19throw_std_bad_allocEv:
LFB4264:
	stp	x29, x30, [sp, -16]!
LCFI5:
	mov	x0, 8
	mov	x29, sp
LCFI6:
	bl	___cxa_allocate_exception
	adrp	x3, __ZTVSt9bad_alloc@GOTPAGE
	ldr	x3, [x3, __ZTVSt9bad_alloc@GOTPAGEOFF]
	adrp	x2, __ZNSt9bad_allocD1Ev@GOTPAGE
	ldr	x2, [x2, __ZNSt9bad_allocD1Ev@GOTPAGEOFF]
	adrp	x1, __ZTISt9bad_alloc@GOTPAGE
	ldr	x1, [x1, __ZTISt9bad_alloc@GOTPAGEOFF]
	add	x3, x3, 16
	str	x3, [x0]
	bl	___cxa_throw
LFE4264:
	.cstring
	.align	3
lC0:
	.ascii "Cleaning up COMPRESSION_DATA resources...\0"
	.align	3
lC1:
	.ascii "  Destroyed FFTW plan\0"
	.align	3
lC2:
	.ascii "  Freed FFTW input buffer\0"
	.align	3
lC3:
	.ascii "  Freed FFTW output buffer\0"
	.align	3
lC4:
	.ascii "  Cleaned up compression matrices\0"
	.text
	.align	2
	.p2align 5,,15
	.globl __ZN16COMPRESSION_DATAD1Ev
__ZN16COMPRESSION_DATAD1Ev:
LFB8604:
	stp	x29, x30, [sp, -48]!
LCFI7:
	mov	x29, sp
LCFI8:
	adrp	x1, lC0@PAGE
	mov	x2, 41
	add	x1, x1, lC0@PAGEOFF;
	stp	x19, x20, [sp, 16]
LCFI9:
	mov	x20, x0
	adrp	x19, __ZSt4cout@GOTPAGE
	ldr	x19, [x19, __ZSt4cout@GOTPAGEOFF]
	stp	x21, x22, [sp, 32]
LCFI10:
	mov	x0, x19
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	x0, [x19]
	ldr	x0, [x0, -24]
	add	x0, x19, x0
	ldr	x21, [x0, 240]
	cbz	x21, L17
	ldrsb	w0, [x21, 56]
	cbz	w0, L14
	ldrsb	w1, [x21, 67]
L15:
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	ldr	x0, [x20, 544]
	cbz	x0, L16
	bl	_fftw_destroy_plan
	adrp	x1, lC1@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 21
	add	x1, x1, lC1@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	x0, [x19]
	ldr	x0, [x0, -24]
	add	x0, x19, x0
	ldr	x21, [x0, 240]
	cbz	x21, L17
	ldrsb	w0, [x21, 56]
	cbz	w0, L18
	ldrsb	w1, [x21, 67]
L19:
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
L16:
	ldr	x0, [x20, 528]
	cbz	x0, L20
	bl	_fftw_free
	adrp	x1, lC2@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 25
	add	x1, x1, lC2@PAGEOFF;
	str	xzr, [x20, 528]
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	x0, [x19]
	ldr	x0, [x0, -24]
	add	x0, x19, x0
	ldr	x21, [x0, 240]
	cbz	x21, L17
	ldrsb	w0, [x21, 56]
	cbz	w0, L21
	ldrsb	w1, [x21, 67]
L22:
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
L20:
	ldr	x0, [x20, 536]
	cbz	x0, L23
	bl	_fftw_free
	adrp	x1, lC3@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 26
	add	x1, x1, lC3@PAGEOFF;
	str	xzr, [x20, 536]
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	x0, [x19]
	ldr	x0, [x0, -24]
	add	x0, x19, x0
	ldr	x21, [x0, 240]
	cbz	x21, L17
	ldrsb	w0, [x21, 56]
	cbz	w0, L24
	ldrsb	w1, [x21, 67]
L25:
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
L23:
	adrp	x1, lC4@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 33
	add	x1, x1, lC4@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	x0, [x19]
	ldr	x0, [x0, -24]
	add	x19, x19, x0
	ldr	x19, [x19, 240]
	cbz	x19, L17
	ldrsb	w0, [x19, 56]
	cbz	w0, L26
	ldrsb	w1, [x19, 67]
L27:
	add	x22, x20, 272
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	add	x0, x20, 504
	bl	__ZN16INTEGER_FIELD_3DD1Ev
	add	x0, x20, 480
	bl	__ZN16INTEGER_FIELD_3DD1Ev
	add	x0, x20, 296
	bl	__ZN8FIELD_3DD1Ev
	ldr	x21, [x22, 8]
	ldr	x19, [x20, 272]
	cmp	x19, x21
	beq	L28
	.p2align 5,,15
L29:
	mov	x0, x19
	add	x19, x19, 184
	bl	__ZN8FIELD_3DD1Ev
	cmp	x21, x19
	bne	L29
	ldr	x21, [x20, 272]
L28:
	cbz	x21, L30
	ldr	x1, [x22, 16]
	mov	x0, x21
	sub	x1, x1, x21
	bl	__ZdlPvm
L30:
	ldr	x0, [x20, 248]
	cbz	x0, L31
	ldr	x1, [x20, 264]
	sub	x1, x1, x0
	bl	__ZdlPvm
L31:
	ldr	x0, [x20, 224]
	cbz	x0, L32
	ldr	x1, [x20, 240]
	sub	x1, x1, x0
	bl	__ZdlPvm
L32:
	ldr	x0, [x20, 200]
	bl	_free
	ldr	x0, [x20, 176]
	bl	_free
	ldr	x0, [x20, 152]
	bl	_free
	ldr	x0, [x20, 128]
	bl	_free
	ldr	x0, [x20, 112]
	bl	_free
	ldr	x0, [x20, 96]
	bl	_free
	ldr	x0, [x20, 80]
	bl	_free
	ldr	x0, [x20, 64]
	ldp	x21, x22, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
LCFI11:
	b	_free
	.p2align 2,,3
L21:
LCFI12:
	mov	x0, x21
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x21]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L22
	mov	x0, x21
	blr	x2
	mov	w1, w0
	b	L22
	.p2align 2,,3
L18:
	mov	x0, x21
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x21]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L19
	mov	x0, x21
	blr	x2
	mov	w1, w0
	b	L19
	.p2align 2,,3
L24:
	mov	x0, x21
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x21]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L25
	mov	x0, x21
	blr	x2
	mov	w1, w0
	b	L25
	.p2align 2,,3
L26:
	mov	x0, x19
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x19]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L27
	mov	x0, x19
	blr	x2
	mov	w1, w0
	b	L27
	.p2align 2,,3
L14:
	mov	x0, x21
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x21]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L15
	mov	x0, x21
	blr	x2
	mov	w1, w0
	b	L15
L17:
	bl	__ZSt16__throw_bad_castv
LFE8604:
	.section __TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table0:
LLSDA8604:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 LLSDACSE8604-LLSDACSB8604
LLSDACSB8604:
LLSDACSE8604:
	.text
	.align	2
	.p2align 5,,15
	.globl __ZN16COMPRESSION_DATAD2Ev
__ZN16COMPRESSION_DATAD2Ev:
LFB9859:
	b	__ZN16COMPRESSION_DATAD1Ev
LFE9859:
	.align	2
	.p2align 5,,15
	.globl __ZN16COMPRESSION_DATA9dct_setupEi
__ZN16COMPRESSION_DATA9dct_setupEi:
LFB8606:
	sub	sp, sp, #48
LCFI13:
	stp	x29, x30, [sp, 16]
LCFI14:
	add	x29, sp, 16
LCFI15:
	stp	x19, x20, [sp, 32]
LCFI16:
	mov	x19, x0
	mov	x0, 4096
	mov	w20, w1
	bl	_fftw_malloc
	mov	x1, x0
	mov	x0, 4096
	str	x1, [x19, 528]
	bl	_fftw_malloc
	str	x0, [x19, 536]
	mov	x4, x0
	mov	w7, 5
	ldr	x3, [x19, 528]
	cmp	w20, 1
	beq	L75
	mov	w7, 4
L75:
	mov	w2, 8
	mov	w6, w7
	str	wzr, [sp]
	mov	w5, w7
	mov	w1, w2
	mov	w0, w2
	bl	_fftw_plan_r2r_3d
	str	x0, [x19, 544]
	ldp	x29, x30, [sp, 16]
	ldp	x19, x20, [sp, 32]
	add	sp, sp, 48
LCFI17:
	ret
LFE8606:
	.align	2
	.p2align 5,,15
	.globl __ZN16COMPRESSION_DATA11dct_cleanupEv
__ZN16COMPRESSION_DATA11dct_cleanupEv:
LFB8607:
	stp	x29, x30, [sp, -32]!
LCFI18:
	mov	x29, sp
LCFI19:
	str	x19, [sp, 16]
LCFI20:
	mov	x19, x0
	ldr	x0, [x0, 544]
	bl	_fftw_destroy_plan
	ldr	x0, [x19, 528]
	bl	_fftw_free
	ldr	x0, [x19, 536]
	bl	_fftw_free
	add	x0, x19, 528
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
LCFI21:
	stp	xzr, xzr, [x0]
	b	_fftw_cleanup
LFE8607:
	.align	2
	.p2align 5,,15
	.globl __ZNSt6vectorI8FIELD_3DSaIS0_EED1Ev
	.weak_definition __ZNSt6vectorI8FIELD_3DSaIS0_EED1Ev
__ZNSt6vectorI8FIELD_3DSaIS0_EED1Ev:
LFB9054:
	stp	x29, x30, [sp, -48]!
LCFI22:
	mov	x29, sp
LCFI23:
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
LCFI24:
	mov	x21, x0
	ldp	x19, x20, [x0]
	cmp	x19, x20
	beq	L79
	.p2align 5,,15
L80:
	mov	x0, x19
	add	x19, x19, 184
	bl	__ZN8FIELD_3DD1Ev
	cmp	x20, x19
	bne	L80
	ldr	x20, [x21]
L79:
	cbz	x20, L78
	ldr	x1, [x21, 16]
	mov	x0, x20
	ldr	x21, [sp, 32]
	sub	x1, x1, x20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
LCFI25:
	b	__ZdlPvm
	.p2align 2,,3
L78:
LCFI26:
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
LCFI27:
	ret
LFE9054:
	.align	2
	.p2align 5,,15
	.globl __ZN16COMPRESSION_DATAC1Ev
__ZN16COMPRESSION_DATAC1Ev:
LFB8598:
	movi	v31.4s, 0
	stp	x29, x30, [sp, -48]!
LCFI28:
	mov	x29, sp
LCFI29:
	stp	x19, x20, [sp, 16]
LCFI30:
	mov	x19, x0
	add	x20, x0, 272
	stp	x21, x22, [sp, 32]
LCFI31:
	add	x21, x0, 296
	stp	xzr, xzr, [x0]
	mov	x0, x21
	str	xzr, [x19, 16]
	strh	wzr, [x19, 56]
	stp	xzr, xzr, [x19, 64]
	stp	xzr, xzr, [x19, 80]
	stp	xzr, xzr, [x19, 96]
	stp	xzr, xzr, [x19, 112]
	str	xzr, [x19, 128]
	str	q31, [x19, 136]
	str	xzr, [x19, 152]
	str	q31, [x19, 160]
	str	xzr, [x19, 176]
	str	q31, [x19, 184]
	str	xzr, [x19, 200]
	stp	q31, q31, [x19, 208]
	stp	q31, q31, [x19, 240]
	str	q31, [x19, 272]
	str	xzr, [x20, 16]
LEHB0:
	bl	__ZN8FIELD_3DC1Ev
LEHE0:
	add	x22, x19, 480
	mov	x0, x22
LEHB1:
	bl	__ZN16INTEGER_FIELD_3DC1Ev
LEHE1:
	add	x0, x19, 504
LEHB2:
	bl	__ZN16INTEGER_FIELD_3DC1Ev
LEHE2:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
LCFI32:
	ret
L90:
LCFI33:
	mov	x22, x0
	b	L87
L92:
	mov	x1, x0
	mov	x0, x22
	mov	x22, x1
	bl	__ZN16INTEGER_FIELD_3DD1Ev
L86:
	mov	x0, x21
	bl	__ZN8FIELD_3DD1Ev
L87:
	mov	x0, x20
	bl	__ZNSt6vectorI8FIELD_3DSaIS0_EED1Ev
	ldr	x0, [x19, 248]
	ldr	x1, [x19, 264]
	sub	x1, x1, x0
	cbz	x0, L88
	bl	__ZdlPvm
L88:
	ldr	x0, [x19, 224]
	ldr	x1, [x19, 240]
	sub	x1, x1, x0
	cbz	x0, L89
	bl	__ZdlPvm
L89:
	ldr	x0, [x19, 200]
	bl	_free
	ldr	x0, [x19, 176]
	bl	_free
	ldr	x0, [x19, 152]
	bl	_free
	ldr	x0, [x19, 128]
	bl	_free
	ldr	x0, [x19, 112]
	bl	_free
	ldr	x0, [x19, 96]
	bl	_free
	ldr	x0, [x19, 80]
	bl	_free
	ldr	x0, [x19, 64]
	bl	_free
	mov	x0, x22
LEHB3:
	bl	__Unwind_Resume
LEHE3:
L91:
	mov	x22, x0
	b	L86
LFE8598:
	.section __TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table1:
LLSDA8598:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 LLSDACSE8598-LLSDACSB8598
LLSDACSB8598:
	.uleb128 LEHB0-LFB8598
	.uleb128 LEHE0-LEHB0
	.uleb128 L90-LFB8598
	.uleb128 0
	.uleb128 LEHB1-LFB8598
	.uleb128 LEHE1-LEHB1
	.uleb128 L91-LFB8598
	.uleb128 0
	.uleb128 LEHB2-LFB8598
	.uleb128 LEHE2-LEHB2
	.uleb128 L92-LFB8598
	.uleb128 0
	.uleb128 LEHB3-LFB8598
	.uleb128 LEHE3-LEHB3
	.uleb128 0
	.uleb128 0
LLSDACSE8598:
	.text
	.cstring
	.align	3
lC5:
	.ascii "Initializing COMPRESSION_DATA:\0"
	.align	3
lC6:
	.ascii "  Dimensions: \0"
	.align	3
lC7:
	.ascii "x\0"
	.align	3
lC8:
	.ascii "  Number of columns: \0"
	.align	3
lC9:
	.ascii "  Bits per value: \0"
	.align	3
lC10:
	.ascii "  Compression percentage: \0"
	.align	3
lC11:
	.ascii "  Initialized matrices for compression data\0"
	.align	3
lC12:
	.ascii "Error: Failed to allocate FFTW buffers\0"
	.align	3
lC13:
	.ascii "  Allocated FFTW buffers\0"
	.align	3
lC14:
	.ascii "Error: Failed to create FFTW plan\0"
	.align	3
lC15:
	.ascii "  Created FFTW plan\0"
	.text
	.align	2
	.p2align 5,,15
	.globl __ZN16COMPRESSION_DATAC1ERK5TVEC3IiEiid
__ZN16COMPRESSION_DATAC1ERK5TVEC3IiEiid:
LFB8601:
	sub	sp, sp, #144
LCFI34:
	scvtf	d31, w3
	movi	v30.4s, 0
	stp	x29, x30, [sp, 16]
LCFI35:
	add	x29, sp, 16
LCFI36:
	stp	x21, x22, [sp, 48]
LCFI37:
	mov	x21, x1
	mov	w22, w3
	stp	x19, x20, [sp, 32]
LCFI38:
	mov	x19, x0
	stp	x23, x24, [sp, 64]
LCFI39:
	mov	w23, w2
	stp	x25, x26, [sp, 80]
LCFI40:
	add	x26, x0, 272
	add	x0, x0, 296
	stp	x27, x28, [sp, 96]
	ldr	x3, [x21]
	str	x0, [x29, 120]
	ldr	w1, [x1, 8]
	str	d15, [sp, 112]
LCFI41:
	fmov	d15, d0
	str	xzr, [x19, 12]
	str	x3, [x19]
	stp	wzr, w2, [x19, 20]
	str	w1, [x19, 8]
	stp	d31, d0, [x19, 40]
	strh	wzr, [x19, 56]
	stp	xzr, xzr, [x19, 64]
	stp	xzr, xzr, [x19, 80]
	stp	xzr, xzr, [x19, 96]
	stp	xzr, xzr, [x19, 112]
	str	xzr, [x19, 128]
	str	q30, [x19, 136]
	str	xzr, [x19, 152]
	str	q30, [x19, 160]
	str	xzr, [x19, 176]
	str	q30, [x19, 184]
	str	xzr, [x19, 200]
	stp	q30, q30, [x19, 208]
	stp	q30, q30, [x19, 240]
	str	q30, [x19, 272]
	str	xzr, [x26, 16]
LEHB4:
	bl	__ZN8FIELD_3DC1Ev
LEHE4:
	add	x27, x19, 480
	mov	x0, x27
LEHB5:
	bl	__ZN16INTEGER_FIELD_3DC1Ev
LEHE5:
	add	x28, x19, 504
	mov	x0, x28
LEHB6:
	bl	__ZN16INTEGER_FIELD_3DC1Ev
LEHE6:
	adrp	x1, lC5@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 30
	add	x1, x1, lC5@PAGEOFF;
LEHB7:
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	adrp	x20, __ZSt4cout@GOTPAGE
	ldr	x20, [x20, __ZSt4cout@GOTPAGEOFF]
	ldr	x0, [x20]
	ldr	x0, [x0, -24]
	add	x0, x20, x0
	ldr	x24, [x0, 240]
	cbz	x24, L173
	ldrsb	w0, [x24, 56]
	cbz	w0, L103
	ldrsb	w1, [x24, 67]
L104:
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	adrp	x1, lC6@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 14
	add	x1, x1, lC6@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	ldr	w1, [x21]
	bl	__ZNSolsEi
	adrp	x25, lC7@PAGE
	mov	x2, 1
	add	x1, x25, lC7@PAGEOFF;
	mov	x24, x0
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	w1, [x21, 4]
	mov	x0, x24
	bl	__ZNSolsEi
	add	x1, x25, lC7@PAGEOFF;
	mov	x2, 1
	mov	x24, x0
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	w1, [x21, 8]
	mov	x0, x24
	bl	__ZNSolsEi
	mov	x21, x0
	ldr	x0, [x0]
	ldr	x0, [x0, -24]
	add	x0, x21, x0
	ldr	x24, [x0, 240]
	cbz	x24, L174
	ldrsb	w0, [x24, 56]
	cbz	w0, L106
	ldrsb	w1, [x24, 67]
L107:
	mov	x0, x21
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	adrp	x1, lC8@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 21
	add	x1, x1, lC8@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	w1, w23
	bl	__ZNSolsEi
	mov	x21, x0
	ldr	x0, [x0]
	ldr	x0, [x0, -24]
	add	x0, x21, x0
	ldr	x23, [x0, 240]
	cbz	x23, L175
	ldrsb	w0, [x23, 56]
	cbz	w0, L109
	ldrsb	w1, [x23, 67]
L110:
	mov	x0, x21
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	adrp	x1, lC9@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 18
	add	x1, x1, lC9@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	w1, w22
	bl	__ZNSolsEi
	mov	x21, x0
	ldr	x0, [x0]
	ldr	x0, [x0, -24]
	add	x0, x21, x0
	ldr	x22, [x0, 240]
	cbz	x22, L176
	ldrsb	w0, [x22, 56]
	cbz	w0, L112
	ldrsb	w1, [x22, 67]
L113:
	mov	x0, x21
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	adrp	x1, lC10@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 26
	add	x1, x1, lC10@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	fmov	d0, d15
	bl	__ZNSo9_M_insertIdEERSoT_
	mov	x21, x0
	ldr	x0, [x0]
	ldr	x0, [x0, -24]
	add	x0, x21, x0
	ldr	x22, [x0, 240]
	cbz	x22, L177
	ldrsb	w0, [x22, 56]
	cbz	w0, L115
	ldrsb	w1, [x22, 67]
L116:
	mov	x0, x21
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	add	x0, x19, 176
	ldp	x0, x1, [x0, 8]
	mul	x0, x0, x1
	cbz	x0, L117
	ldr	x0, [x19, 176]
	bl	_free
	str	xzr, [x19, 176]
L117:
	add	x0, x19, 200
	stp	xzr, xzr, [x19, 184]
	ldp	x0, x1, [x0, 8]
	mul	x0, x0, x1
	cbz	x0, L118
	ldr	x0, [x19, 200]
	bl	_free
	str	xzr, [x19, 200]
L118:
	add	x0, x19, 128
	stp	xzr, xzr, [x19, 208]
	ldp	x0, x1, [x0, 8]
	mul	x0, x0, x1
	cbz	x0, L119
	ldr	x0, [x19, 128]
	bl	_free
	str	xzr, [x19, 128]
L119:
	add	x0, x19, 152
	stp	xzr, xzr, [x19, 136]
	ldp	x0, x1, [x0, 8]
	mul	x0, x0, x1
	cbz	x0, L120
	ldr	x0, [x19, 152]
	bl	_free
	str	xzr, [x19, 152]
L120:
	adrp	x1, lC11@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 43
	add	x1, x1, lC11@PAGEOFF;
	stp	xzr, xzr, [x19, 160]
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	x0, [x20]
	ldr	x0, [x0, -24]
	add	x0, x20, x0
	ldr	x21, [x0, 240]
	cbz	x21, L178
	ldrsb	w0, [x21, 56]
	cbz	w0, L122
	ldrsb	w1, [x21, 67]
L123:
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	mov	x0, 4096
	bl	_fftw_malloc
	mov	x1, x0
	mov	x0, 4096
	str	x1, [x19, 528]
	bl	_fftw_malloc
	ldr	x1, [x19, 528]
	str	x0, [x19, 536]
	cmp	x1, 0
	ccmp	x0, 0, 4, ne
	beq	L179
	adrp	x1, lC13@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 24
	add	x1, x1, lC13@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	x0, [x20]
	ldr	x0, [x0, -24]
	add	x0, x20, x0
	ldr	x21, [x0, 240]
	cbz	x21, L180
	ldrsb	w0, [x21, 56]
	cbz	w0, L126
	ldrsb	w1, [x21, 67]
L127:
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	mov	w8, 64
	mov	w7, 5
	ldr	x3, [x19, 528]
	mov	w2, 8
	mov	w6, w7
	ldr	x4, [x19, 536]
	mov	w5, w7
	mov	w1, w2
	mov	w0, w2
	str	w8, [sp]
	bl	_fftw_plan_r2r_3d
	str	x0, [x19, 544]
	cbz	x0, L181
	adrp	x1, lC15@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 19
	add	x1, x1, lC15@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldr	x0, [x20]
	ldr	x0, [x0, -24]
	add	x20, x20, x0
	ldr	x20, [x20, 240]
	cbz	x20, L182
	ldrsb	w0, [x20, 56]
	cbz	w0, L131
	ldrsb	w1, [x20, 67]
L132:
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	ldr	d15, [sp, 112]
	ldp	x29, x30, [sp, 16]
	ldp	x19, x20, [sp, 32]
	ldp	x21, x22, [sp, 48]
	ldp	x23, x24, [sp, 64]
	ldp	x25, x26, [sp, 80]
	ldp	x27, x28, [sp, 96]
	add	sp, sp, 144
LCFI42:
	ret
	.p2align 2,,3
L115:
LCFI43:
	mov	x0, x22
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x22]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L116
	mov	x0, x22
	blr	x2
	mov	w1, w0
	b	L116
	.p2align 2,,3
L122:
	mov	x0, x21
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x21]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L123
	mov	x0, x21
	blr	x2
	mov	w1, w0
	b	L123
	.p2align 2,,3
L112:
	mov	x0, x22
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x22]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L113
	mov	x0, x22
	blr	x2
	mov	w1, w0
	b	L113
	.p2align 2,,3
L109:
	mov	x0, x23
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x23]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L110
	mov	x0, x23
	blr	x2
	mov	w1, w0
	b	L110
	.p2align 2,,3
L106:
	mov	x0, x24
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x24]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L107
	mov	x0, x24
	blr	x2
	mov	w1, w0
	b	L107
	.p2align 2,,3
L103:
	mov	x0, x24
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x24]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L104
	mov	x0, x24
	blr	x2
	mov	w1, w0
	b	L104
	.p2align 2,,3
L131:
	mov	x0, x20
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x20]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L132
	mov	x0, x20
	blr	x2
	mov	w1, w0
	b	L132
	.p2align 2,,3
L126:
	mov	x0, x21
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x21]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L127
	mov	x0, x21
	blr	x2
	mov	w1, w0
	b	L127
L179:
	adrp	x1, lC12@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 38
	add	x1, x1, lC12@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
L172:
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0
	mov	w0, 1
	bl	_exit
L181:
	adrp	x1, lC14@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 33
	add	x1, x1, lC14@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	b	L172
L173:
	bl	__ZSt16__throw_bad_castv
L176:
	bl	__ZSt16__throw_bad_castv
L175:
	bl	__ZSt16__throw_bad_castv
LEHE7:
L150:
	mov	x20, x0
	mov	x0, x28
	bl	__ZN16INTEGER_FIELD_3DD1Ev
L134:
	mov	x0, x27
	bl	__ZN16INTEGER_FIELD_3DD1Ev
L135:
	ldr	x0, [x29, 120]
	bl	__ZN8FIELD_3DD1Ev
L136:
	mov	x0, x26
	bl	__ZNSt6vectorI8FIELD_3DSaIS0_EED1Ev
	ldr	x0, [x19, 248]
	ldr	x1, [x19, 264]
	sub	x1, x1, x0
	cbz	x0, L137
	bl	__ZdlPvm
L137:
	ldr	x0, [x19, 224]
	ldr	x1, [x19, 240]
	sub	x1, x1, x0
	cbz	x0, L138
	bl	__ZdlPvm
L138:
	ldr	x0, [x19, 200]
	bl	_free
	ldr	x0, [x19, 176]
	bl	_free
	ldr	x0, [x19, 152]
	bl	_free
	ldr	x0, [x19, 128]
	bl	_free
	ldr	x0, [x19, 112]
	bl	_free
	ldr	x0, [x19, 96]
	bl	_free
	ldr	x0, [x19, 80]
	bl	_free
	ldr	x0, [x19, 64]
	bl	_free
	mov	x0, x20
LEHB8:
	bl	__Unwind_Resume
LEHE8:
L174:
LEHB9:
	bl	__ZSt16__throw_bad_castv
L178:
	bl	__ZSt16__throw_bad_castv
L149:
	mov	x20, x0
	b	L134
L148:
	mov	x20, x0
	b	L135
L147:
	mov	x20, x0
	b	L136
L180:
	bl	__ZSt16__throw_bad_castv
L182:
	bl	__ZSt16__throw_bad_castv
L177:
	bl	__ZSt16__throw_bad_castv
LEHE9:
LFE8601:
	.section __TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table2:
LLSDA8601:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 LLSDACSE8601-LLSDACSB8601
LLSDACSB8601:
	.uleb128 LEHB4-LFB8601
	.uleb128 LEHE4-LEHB4
	.uleb128 L147-LFB8601
	.uleb128 0
	.uleb128 LEHB5-LFB8601
	.uleb128 LEHE5-LEHB5
	.uleb128 L148-LFB8601
	.uleb128 0
	.uleb128 LEHB6-LFB8601
	.uleb128 LEHE6-LEHB6
	.uleb128 L149-LFB8601
	.uleb128 0
	.uleb128 LEHB7-LFB8601
	.uleb128 LEHE7-LEHB7
	.uleb128 L150-LFB8601
	.uleb128 0
	.uleb128 LEHB8-LFB8601
	.uleb128 LEHE8-LEHB8
	.uleb128 0
	.uleb128 0
	.uleb128 LEHB9-LFB8601
	.uleb128 LEHE9-LEHB9
	.uleb128 L150-LFB8601
	.uleb128 0
LLSDACSE8601:
	.text
	.align	2
	.p2align 5,,15
	.globl __ZN16COMPRESSION_DATAC2ERK5TVEC3IiEiid
__ZN16COMPRESSION_DATAC2ERK5TVEC3IiEiid:
LFB9855:
	b	__ZN16COMPRESSION_DATAC1ERK5TVEC3IiEiid
LFE9855:
	.align	2
	.p2align 5,,15
	.globl __ZN16COMPRESSION_DATAC2Ev
__ZN16COMPRESSION_DATAC2Ev:
LFB9857:
	movi	v31.4s, 0
	stp	x29, x30, [sp, -48]!
LCFI44:
	mov	x29, sp
LCFI45:
	stp	x19, x20, [sp, 16]
LCFI46:
	mov	x19, x0
	add	x20, x0, 272
	stp	x21, x22, [sp, 32]
LCFI47:
	add	x21, x0, 296
	stp	xzr, xzr, [x0]
	mov	x0, x21
	str	xzr, [x19, 16]
	strh	wzr, [x19, 56]
	stp	xzr, xzr, [x19, 64]
	stp	xzr, xzr, [x19, 80]
	stp	xzr, xzr, [x19, 96]
	stp	xzr, xzr, [x19, 112]
	str	xzr, [x19, 128]
	str	q31, [x19, 136]
	str	xzr, [x19, 152]
	str	q31, [x19, 160]
	str	xzr, [x19, 176]
	str	q31, [x19, 184]
	str	xzr, [x19, 200]
	stp	q31, q31, [x19, 208]
	stp	q31, q31, [x19, 240]
	str	q31, [x19, 272]
	str	xzr, [x20, 16]
LEHB10:
	bl	__ZN8FIELD_3DC1Ev
LEHE10:
	add	x22, x19, 480
	mov	x0, x22
LEHB11:
	bl	__ZN16INTEGER_FIELD_3DC1Ev
LEHE11:
	add	x0, x19, 504
LEHB12:
	bl	__ZN16INTEGER_FIELD_3DC1Ev
LEHE12:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
LCFI48:
	ret
L191:
LCFI49:
	mov	x22, x0
	b	L188
L193:
	mov	x1, x0
	mov	x0, x22
	mov	x22, x1
	bl	__ZN16INTEGER_FIELD_3DD1Ev
L187:
	mov	x0, x21
	bl	__ZN8FIELD_3DD1Ev
L188:
	mov	x0, x20
	bl	__ZNSt6vectorI8FIELD_3DSaIS0_EED1Ev
	ldr	x0, [x19, 248]
	ldr	x1, [x19, 264]
	sub	x1, x1, x0
	cbz	x0, L189
	bl	__ZdlPvm
L189:
	ldr	x0, [x19, 224]
	ldr	x1, [x19, 240]
	sub	x1, x1, x0
	cbz	x0, L190
	bl	__ZdlPvm
L190:
	ldr	x0, [x19, 200]
	bl	_free
	ldr	x0, [x19, 176]
	bl	_free
	ldr	x0, [x19, 152]
	bl	_free
	ldr	x0, [x19, 128]
	bl	_free
	ldr	x0, [x19, 112]
	bl	_free
	ldr	x0, [x19, 96]
	bl	_free
	ldr	x0, [x19, 80]
	bl	_free
	ldr	x0, [x19, 64]
	bl	_free
	mov	x0, x22
LEHB13:
	bl	__Unwind_Resume
LEHE13:
L192:
	mov	x22, x0
	b	L187
LFE9857:
	.section __TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table3:
LLSDA9857:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 LLSDACSE9857-LLSDACSB9857
LLSDACSB9857:
	.uleb128 LEHB10-LFB9857
	.uleb128 LEHE10-LEHB10
	.uleb128 L191-LFB9857
	.uleb128 0
	.uleb128 LEHB11-LFB9857
	.uleb128 LEHE11-LEHB11
	.uleb128 L192-LFB9857
	.uleb128 0
	.uleb128 LEHB12-LFB9857
	.uleb128 LEHE12-LEHB12
	.uleb128 L193-LFB9857
	.uleb128 0
	.uleb128 LEHB13-LFB9857
	.uleb128 LEHE13-LEHB13
	.uleb128 0
	.uleb128 0
LLSDACSE9857:
	.text
	.cstring
	.align	3
lC16:
	.ascii "vector::_M_default_append\0"
	.text
	.align	2
	.p2align 5,,15
	.globl __ZNSt6vectorI8FIELD_3DSaIS0_EE17_M_default_appendEm
	.weak_definition __ZNSt6vectorI8FIELD_3DSaIS0_EE17_M_default_appendEm
__ZNSt6vectorI8FIELD_3DSaIS0_EE17_M_default_appendEm:
LFB9325:
	cbz	x1, L246
	stp	x29, x30, [sp, -96]!
LCFI50:
	mov	x29, sp
LCFI51:
	stp	x19, x20, [sp, 16]
LCFI52:
	mov	x19, x1
	mov	x1, 14247
	movk	x1, 0xe9bd, lsl 16
	movk	x1, 0x6f4d, lsl 32
	stp	x21, x22, [sp, 32]
	movk	x1, 0xd37a, lsl 48
	stp	x23, x24, [sp, 48]
LCFI53:
	mov	x23, x0
	ldp	x22, x0, [x0, 8]
	sub	x0, x0, x22
	asr	x0, x0, 3
	mul	x0, x0, x1
	cmp	x19, x0
	bls	L249
	stp	x25, x26, [x29, 64]
LCFI54:
	mov	x2, 2849
	movk	x2, 0xc859, lsl 16
	movk	x2, 0x1642, lsl 32
	stp	x27, x28, [x29, 80]
LCFI55:
	movk	x2, 0xb2, lsl 48
	ldr	x27, [x23]
	sub	x28, x22, x27
	asr	x26, x28, 3
	mul	x26, x26, x1
	sub	x0, x2, x26
	cmp	x0, x19
	bcc	L250
	cmp	x19, x26
	csel	x0, x19, x26, cs
	add	x0, x0, x26
	cmp	x0, x2
	csel	x0, x0, x2, ls
	add	x24, x0, x0, lsl 1
	mov	x21, x19
	lsl	x24, x24, 3
	sub	x24, x24, x0
	lsl	x24, x24, 3
	mov	x0, x24
LEHB14:
	bl	__Znwm
LEHE14:
	add	x28, x0, x28
	mov	x25, x0
	mov	x20, x28
	.p2align 5,,15
L211:
	mov	x0, x20
LEHB15:
	bl	__ZN8FIELD_3DC1Ev
LEHE15:
	subs	x21, x21, #1
	add	x20, x20, 184
	bne	L211
	mov	x20, x27
	mov	x21, x25
	cmp	x27, x22
	beq	L227
	.p2align 5,,15
L218:
	mov	x1, x20
	mov	x0, x21
LEHB16:
	bl	__ZN8FIELD_3DC1ERKS_
LEHE16:
	add	x20, x20, 184
	add	x21, x21, 184
	cmp	x22, x20
	bne	L218
	mov	x20, x27
	.p2align 5,,15
L221:
	mov	x0, x20
	add	x20, x20, 184
	bl	__ZN8FIELD_3DD1Ev
	cmp	x22, x20
	bne	L221
L227:
	cbz	x27, L220
	ldr	x1, [x23, 16]
	mov	x0, x27
	sub	x1, x1, x27
	bl	__ZdlPvm
L220:
	add	x19, x19, x26
	add	x24, x25, x24
	add	x0, x19, x19, lsl 1
	str	x24, [x23, 16]
	lsl	x0, x0, 3
	sub	x0, x0, x19
	add	x0, x25, x0, lsl 3
	stp	x25, x0, [x23]
	ldp	x25, x26, [x29, 64]
LCFI56:
	ldp	x27, x28, [x29, 80]
LCFI57:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
LCFI58:
	ret
	.p2align 2,,3
L246:
	ret
	.p2align 2,,3
L249:
LCFI59:
	mov	x20, x22
	.p2align 5,,15
L205:
	mov	x0, x20
LEHB17:
	bl	__ZN8FIELD_3DC1Ev
LEHE17:
	subs	x19, x19, #1
	add	x20, x20, 184
	bne	L205
	str	x20, [x23, 8]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
LCFI60:
	ret
L231:
LCFI61:
	stp	x25, x26, [x29, 64]
LCFI62:
	stp	x27, x28, [x29, 80]
LCFI63:
	bl	___cxa_begin_catch
L207:
	cmp	x22, x20
	beq	L251
	mov	x0, x22
	add	x22, x22, 184
	bl	__ZN8FIELD_3DD1Ev
	b	L207
L250:
	adrp	x0, lC16@PAGE
	add	x0, x0, lC16@PAGEOFF;
LEHB18:
	bl	__ZSt20__throw_length_errorPKc
LEHE18:
L251:
LEHB19:
	bl	___cxa_rethrow
LEHE19:
L230:
	mov	x19, x0
	bl	___cxa_end_catch
	mov	x0, x19
LEHB20:
	bl	__Unwind_Resume
LEHE20:
L233:
	bl	___cxa_begin_catch
L214:
	cmp	x20, x28
	beq	L252
	mov	x0, x28
	add	x28, x28, 184
	bl	__ZN8FIELD_3DD1Ev
	b	L214
L234:
	mov	x20, x25
	bl	___cxa_begin_catch
L223:
	cmp	x21, x20
	beq	L253
	mov	x0, x20
	add	x20, x20, 184
	bl	__ZN8FIELD_3DD1Ev
	b	L223
L252:
LEHB21:
	bl	___cxa_rethrow
LEHE21:
L232:
	mov	x19, x0
	bl	___cxa_end_catch
L217:
	mov	x0, x25
	mov	x1, x24
	bl	__ZdlPvm
	mov	x0, x19
LEHB22:
	bl	__Unwind_Resume
LEHE22:
L253:
LEHB23:
	bl	___cxa_rethrow
LEHE23:
L235:
	mov	x20, x0
	bl	___cxa_end_catch
	mov	x0, 184
	madd	x19, x19, x0, x28
L226:
	cmp	x19, x28
	beq	L254
	mov	x0, x28
	add	x28, x28, 184
	bl	__ZN8FIELD_3DD1Ev
	b	L226
L254:
	mov	x19, x20
	b	L217
LFE9325:
	.section __TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table4:
	.align	2
LLSDA9325:
	.byte	0xff
	.byte	0x9b
	.uleb128 LLSDATT9325-LLSDATTD9325
LLSDATTD9325:
	.byte	0x1
	.uleb128 LLSDACSE9325-LLSDACSB9325
LLSDACSB9325:
	.uleb128 LEHB14-LFB9325
	.uleb128 LEHE14-LEHB14
	.uleb128 0
	.uleb128 0
	.uleb128 LEHB15-LFB9325
	.uleb128 LEHE15-LEHB15
	.uleb128 L233-LFB9325
	.uleb128 0x1
	.uleb128 LEHB16-LFB9325
	.uleb128 LEHE16-LEHB16
	.uleb128 L234-LFB9325
	.uleb128 0x1
	.uleb128 LEHB17-LFB9325
	.uleb128 LEHE17-LEHB17
	.uleb128 L231-LFB9325
	.uleb128 0x1
	.uleb128 LEHB18-LFB9325
	.uleb128 LEHE18-LEHB18
	.uleb128 0
	.uleb128 0
	.uleb128 LEHB19-LFB9325
	.uleb128 LEHE19-LEHB19
	.uleb128 L230-LFB9325
	.uleb128 0
	.uleb128 LEHB20-LFB9325
	.uleb128 LEHE20-LEHB20
	.uleb128 0
	.uleb128 0
	.uleb128 LEHB21-LFB9325
	.uleb128 LEHE21-LEHB21
	.uleb128 L232-LFB9325
	.uleb128 0
	.uleb128 LEHB22-LFB9325
	.uleb128 LEHE22-LEHB22
	.uleb128 0
	.uleb128 0
	.uleb128 LEHB23-LFB9325
	.uleb128 LEHE23-LEHB23
	.uleb128 L235-LFB9325
	.uleb128 0
LLSDACSE9325:
	.byte	0x1
	.byte	0
	.align	2
	.long	0

LLSDATT9325:
	.text
	.cstring
	.align	3
lC17:
	.ascii "set_dampingArrayList\0"
	.align	3
lC18:
	.ascii "_dampingArrayBuilt\0"
	.align	3
lC19:
	.ascii "COMPRESSION_DATA.cpp\0"
	.align	3
lC20:
	.ascii "Inside set_dampingArrayList!\0"
	.align	3
lC21:
	.ascii "total number: \0"
	.align	3
lC22:
	.ascii "array list built: \0"
	.text
	.align	2
	.p2align 5,,15
	.globl __ZN16COMPRESSION_DATA20set_dampingArrayListEv
__ZN16COMPRESSION_DATA20set_dampingArrayListEv:
LFB8605:
	stp	x29, x30, [sp, -304]!
LCFI64:
	mov	x29, sp
LCFI65:
	mov	x3, 20
	mov	x2, 0
	stp	x19, x20, [sp, 16]
LCFI66:
	add	x20, x29, 120
	add	x19, x29, 136
	stp	x25, x26, [sp, 64]
LCFI67:
	add	x26, x29, 104
	mov	x1, x26
	stp	x21, x22, [sp, 32]
LCFI68:
	mov	x22, x0
	mov	x0, x20
	adrp	x21, lC17@PAGE
	str	d15, [sp, 88]
	str	x3, [x29, 104]
	str	x19, [x29, 120]
LEHB24:
LCFI69:
	bl	__ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm
LEHE24:
	mov	x2, x0
	ldr	x6, [x29, 104]
	add	x3, x21, lC17@PAGEOFF;
	ldp	x4, x5, [x3]
	mov	x1, x20
	mov	x0, x26
	str	x2, [x29, 120]
	ldr	w3, [x3, 16]
	str	x6, [x29, 136]
	stp	x4, x5, [x2]
	str	w3, [x2, 16]
	ldr	x2, [x29, 104]
	ldr	x3, [x29, 120]
	str	x2, [x29, 128]
	strb	wzr, [x3, x2]
LEHB25:
	bl	__ZN5TIMERC1ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
LEHE25:
	ldr	x0, [x29, 120]
	cmp	x0, x19
	beq	L256
	ldr	x1, [x29, 136]
	add	x1, x1, 1
	bl	__ZdlPvm
L256:
	ldrb	w0, [x22, 56]
	tbz	x0, 0, L284
	ldrb	w0, [x22, 57]
	tbnz	x0, 0, L285
	adrp	x0, lC20@PAGE
	add	x0, x0, lC20@PAGEOFF;
LEHB26:
	bl	_puts
	add	x25, x22, 296
	mov	x0, x20
	mov	x1, x25
	bl	__ZN8FIELD_3DC1ERKS_
LEHE26:
	stp	x23, x24, [x29, 48]
LCFI70:
	fmov	d30, 4.0e+0
	fmov	d29, 1.0e+0
	mov	x1, 14247
	add	x24, x22, 272
	movk	x1, 0xe9bd, lsl 16
	ldr	d31, [x22, 40]
	movk	x1, 0x6f4d, lsl 32
	movk	x1, 0xd37a, lsl 48
	ldr	x21, [x24, 8]
	ldr	x2, [x22, 272]
	fmadd	d29, d31, d30, d29
	sub	x0, x21, x2
	asr	x0, x0, 3
	mul	x0, x0, x1
	fcvtzs	w23, d29
	sxtw	x1, w23
	cmp	x1, x0
	bhi	L286
	bcs	L261
	str	x27, [x29, 80]
LCFI71:
	mov	w27, 184
	smaddl	x27, w23, w27, x2
	cmp	x21, x27
	beq	L283
	mov	x19, x27
	.p2align 5,,15
L263:
	mov	x0, x19
	add	x19, x19, 184
	bl	__ZN8FIELD_3DD1Ev
	cmp	x21, x19
	bne	L263
	str	x27, [x24, 8]
	ldr	x27, [x29, 80]
LCFI72:
	b	L261
	.p2align 2,,3
L286:
	sub	x1, x1, x0
	mov	x0, x24
LEHB27:
	bl	__ZNSt6vectorI8FIELD_3DSaIS0_EE17_M_default_appendEm
L261:
	adrp	x1, lC21@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	x2, 14
	add	x1, x1, lC21@PAGEOFF;
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	mov	w1, w23
	bl	__ZNSolsEi
	mov	x19, x0
	ldr	x0, [x0]
	ldr	x0, [x0, -24]
	add	x0, x19, x0
	ldr	x21, [x0, 240]
	cbz	x21, L287
	ldrsb	w0, [x21, 56]
	cbz	w0, L265
	ldrsb	w1, [x21, 67]
L266:
	mov	x0, x19
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	cmp	w23, 0
	ble	L267
	fmov	d15, 2.5e-1
	mov	x21, 0
	mov	w19, 0
	.p2align 5,,15
L268:
	scvtf	d0, w19
	mov	x0, x20
	fmul	d0, d0, d15
	bl	__ZN8FIELD_3D11toFastPowerEd
	ldr	x0, [x24]
	mov	x1, x20
	add	x0, x0, x21
	bl	__ZN8FIELD_3DaSERKS_
	mov	x1, x25
	mov	x0, x20
	bl	__ZN8FIELD_3DaSERKS_
	add	w19, w19, 1
	add	x21, x21, 184
	cmp	w23, w19
	bne	L268
L267:
	mov	w3, 1
	adrp	x1, lC22@PAGE
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	add	x1, x1, lC22@PAGEOFF;
	mov	x2, 18
	strb	w3, [x22, 57]
	bl	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	ldrb	w1, [x22, 57]
	adrp	x0, __ZSt4cout@GOTPAGE
	ldr	x0, [x0, __ZSt4cout@GOTPAGEOFF]
	bl	__ZNSo9_M_insertIbEERSoT_
	mov	x19, x0
	ldr	x0, [x0]
	ldr	x0, [x0, -24]
	add	x0, x19, x0
	ldr	x21, [x0, 240]
	cbz	x21, L288
	ldrsb	w0, [x21, 56]
	cbz	w0, L270
	ldrsb	w1, [x21, 67]
L271:
	mov	x0, x19
	bl	__ZNSo3putEc
	bl	__ZNSo5flushEv
	mov	x0, x20
	bl	__ZN8FIELD_3DD1Ev
	mov	x0, x26
	bl	__ZN5TIMERD1Ev
	ldr	d15, [sp, 88]
	ldp	x23, x24, [x29, 48]
LCFI73:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 304
LCFI74:
	ret
	.p2align 2,,3
L285:
LCFI75:
	mov	x0, x26
	bl	__ZN5TIMERD1Ev
	ldr	d15, [sp, 88]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 304
LCFI76:
	ret
	.p2align 2,,3
L265:
LCFI77:
	mov	x0, x21
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x21]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L266
	mov	x0, x21
	blr	x2
	mov	w1, w0
	b	L266
	.p2align 2,,3
L270:
	mov	x0, x21
	bl	__ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x21]
	mov	w1, 10
	adrp	x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGE
	ldr	x0, [x0, __ZNKSt5ctypeIcE8do_widenEc@GOTPAGEOFF]
	ldr	x2, [x2, 48]
	cmp	x2, x0
	beq	L271
	mov	x0, x21
	blr	x2
LEHE27:
	mov	w1, w0
	b	L271
	.p2align 2,,3
L283:
LCFI78:
	ldr	x27, [x29, 80]
LCFI79:
	b	L261
L284:
LCFI80:
	adrp	x3, lC18@PAGE
	adrp	x1, lC19@PAGE
	add	x0, x21, lC17@PAGEOFF;
	add	x3, x3, lC18@PAGEOFF;
	add	x1, x1, lC19@PAGEOFF;
	mov	w2, 150
LEHB28:
	bl	___assert_rtn
LEHE28:
L287:
LCFI81:
LEHB29:
	bl	__ZSt16__throw_bad_castv
L288:
	bl	__ZSt16__throw_bad_castv
LEHE29:
L279:
	mov	x19, x0
	mov	x0, x20
	bl	__ZN8FIELD_3DD1Ev
L274:
	mov	x0, x26
	str	x27, [x29, 80]
LCFI82:
	bl	__ZN5TIMERD1Ev
	mov	x0, x19
LEHB30:
	bl	__Unwind_Resume
L277:
LCFI83:
	mov	x19, x0
	mov	x0, x20
	stp	x23, x24, [x29, 48]
LCFI84:
	str	x27, [x29, 80]
LCFI85:
	bl	__ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	mov	x0, x19
	bl	__Unwind_Resume
LEHE30:
L278:
LCFI86:
	mov	x19, x0
	stp	x23, x24, [x29, 48]
LCFI87:
	b	L274
LFE8605:
	.section __TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table5:
LLSDA8605:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 LLSDACSE8605-LLSDACSB8605
LLSDACSB8605:
	.uleb128 LEHB24-LFB8605
	.uleb128 LEHE24-LEHB24
	.uleb128 0
	.uleb128 0
	.uleb128 LEHB25-LFB8605
	.uleb128 LEHE25-LEHB25
	.uleb128 L277-LFB8605
	.uleb128 0
	.uleb128 LEHB26-LFB8605
	.uleb128 LEHE26-LEHB26
	.uleb128 L278-LFB8605
	.uleb128 0
	.uleb128 LEHB27-LFB8605
	.uleb128 LEHE27-LEHB27
	.uleb128 L279-LFB8605
	.uleb128 0
	.uleb128 LEHB28-LFB8605
	.uleb128 LEHE28-LEHB28
	.uleb128 L278-LFB8605
	.uleb128 0
	.uleb128 LEHB29-LFB8605
	.uleb128 LEHE29-LEHB29
	.uleb128 L279-LFB8605
	.uleb128 0
	.uleb128 LEHB30-LFB8605
	.uleb128 LEHE30-LEHB30
	.uleb128 0
	.uleb128 0
LLSDACSE8605:
	.text
	.section	__TEXT,__StaticInit,regular,pure_instructions
	.align	2
	.p2align 5,,15
__GLOBAL__sub_I_COMPRESSION_DATA.cpp:
LFB9849:
	stp	x29, x30, [sp, -32]!
LCFI88:
	mov	x29, sp
LCFI89:
	str	x19, [sp, 16]
LCFI90:
	adrp	x19, __ZStL8__ioinit@PAGE
	add	x19, x19, __ZStL8__ioinit@PAGEOFF;
	mov	x0, x19
	bl	__ZNSt8ios_base4InitC1Ev
	adrp	x2, ___dso_handle@PAGE
	mov	x1, x19
	ldr	x19, [sp, 16]
	add	x2, x2, ___dso_handle@PAGEOFF;
	ldp	x29, x30, [sp], 32
LCFI91:
	adrp	x0, __ZNSt8ios_base4InitD1Ev@GOTPAGE
	ldr	x0, [x0, __ZNSt8ios_base4InitD1Ev@GOTPAGEOFF]
	b	___cxa_atexit
LFE9849:
	.zerofill __DATA,__bss,__ZStL8__ioinit,1,0
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$0,LECIE1-LSCIE1
	.long L$set$0
LSCIE1:
	.long	0
	.byte	0x3
	.ascii "zPLR\0"
	.uleb128 0x1
	.sleb128 -8
	.uleb128 0x1e
	.uleb128 0x7
	.byte	0x9b
L_got_pcr0:
	.long	___gxx_personality_v0@GOT-L_got_pcr0
	.byte	0x10
	.byte	0x10
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.align	3
LECIE1:
LSFDE1:
	.set L$set$1,LEFDE1-LASFDE1
	.long L$set$1
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB2436-.
	.set L$set$2,LFE2436-LFB2436
	.quad L$set$2
	.uleb128 0x8
	.quad	0
	.align	3
LEFDE1:
LSFDE3:
	.set L$set$3,LEFDE3-LASFDE3
	.long L$set$3
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB9861-.
	.set L$set$4,LFE9861-LFB9861
	.quad L$set$4
	.uleb128 0x8
	.quad	0
	.byte	0x4
	.set L$set$5,LCFI0-LFB9861
	.long L$set$5
	.byte	0xe
	.uleb128 0x20
	.byte	0x9d
	.uleb128 0x4
	.byte	0x9e
	.uleb128 0x3
	.byte	0x4
	.set L$set$6,LCFI1-LCFI0
	.long L$set$6
	.byte	0xd
	.uleb128 0x1d
	.byte	0x4
	.set L$set$7,LCFI2-LCFI1
	.long L$set$7
	.byte	0x93
	.uleb128 0x2
	.byte	0x94
	.uleb128 0x1
	.byte	0x4
	.set L$set$8,LCFI3-LCFI2
	.long L$set$8
	.byte	0xa
	.byte	0xde
	.byte	0xdd
	.byte	0xd3
	.byte	0xd4
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$9,LCFI4-LCFI3
	.long L$set$9
	.byte	0xb
	.align	3
LEFDE3:
LSFDE5:
	.set L$set$10,LEFDE5-LASFDE5
	.long L$set$10
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB4264-.
	.set L$set$11,LFE4264-LFB4264
	.quad L$set$11
	.uleb128 0x8
	.quad	0
	.byte	0x4
	.set L$set$12,LCFI5-LFB4264
	.long L$set$12
	.byte	0xe
	.uleb128 0x10
	.byte	0x9d
	.uleb128 0x2
	.byte	0x9e
	.uleb128 0x1
	.byte	0x4
	.set L$set$13,LCFI6-LCFI5
	.long L$set$13
	.byte	0xd
	.uleb128 0x1d
	.align	3
LEFDE5:
LSFDE7:
	.set L$set$14,LEFDE7-LASFDE7
	.long L$set$14
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB8604-.
	.set L$set$15,LFE8604-LFB8604
	.quad L$set$15
	.uleb128 0x8
	.quad	LLSDA8604-.
	.byte	0x4
	.set L$set$16,LCFI7-LFB8604
	.long L$set$16
	.byte	0xe
	.uleb128 0x30
	.byte	0x9d
	.uleb128 0x6
	.byte	0x9e
	.uleb128 0x5
	.byte	0x4
	.set L$set$17,LCFI8-LCFI7
	.long L$set$17
	.byte	0xd
	.uleb128 0x1d
	.byte	0x4
	.set L$set$18,LCFI9-LCFI8
	.long L$set$18
	.byte	0x93
	.uleb128 0x4
	.byte	0x94
	.uleb128 0x3
	.byte	0x4
	.set L$set$19,LCFI10-LCFI9
	.long L$set$19
	.byte	0x95
	.uleb128 0x2
	.byte	0x96
	.uleb128 0x1
	.byte	0x4
	.set L$set$20,LCFI11-LCFI10
	.long L$set$20
	.byte	0xa
	.byte	0xde
	.byte	0xdd
	.byte	0xd5
	.byte	0xd6
	.byte	0xd3
	.byte	0xd4
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$21,LCFI12-LCFI11
	.long L$set$21
	.byte	0xb
	.align	3
LEFDE7:
LSFDE9:
	.set L$set$22,LEFDE9-LASFDE9
	.long L$set$22
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB9859-.
	.set L$set$23,LFE9859-LFB9859
	.quad L$set$23
	.uleb128 0x8
	.quad	0
	.align	3
LEFDE9:
LSFDE11:
	.set L$set$24,LEFDE11-LASFDE11
	.long L$set$24
LASFDE11:
	.long	LASFDE11-EH_frame1
	.quad	LFB8606-.
	.set L$set$25,LFE8606-LFB8606
	.quad L$set$25
	.uleb128 0x8
	.quad	0
	.byte	0x4
	.set L$set$26,LCFI13-LFB8606
	.long L$set$26
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.set L$set$27,LCFI14-LCFI13
	.long L$set$27
	.byte	0x9d
	.uleb128 0x4
	.byte	0x9e
	.uleb128 0x3
	.byte	0x4
	.set L$set$28,LCFI15-LCFI14
	.long L$set$28
	.byte	0xc
	.uleb128 0x1d
	.uleb128 0x20
	.byte	0x4
	.set L$set$29,LCFI16-LCFI15
	.long L$set$29
	.byte	0x93
	.uleb128 0x2
	.byte	0x94
	.uleb128 0x1
	.byte	0x4
	.set L$set$30,LCFI17-LCFI16
	.long L$set$30
	.byte	0xd3
	.byte	0xd4
	.byte	0xdd
	.byte	0xde
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.align	3
LEFDE11:
LSFDE13:
	.set L$set$31,LEFDE13-LASFDE13
	.long L$set$31
LASFDE13:
	.long	LASFDE13-EH_frame1
	.quad	LFB8607-.
	.set L$set$32,LFE8607-LFB8607
	.quad L$set$32
	.uleb128 0x8
	.quad	0
	.byte	0x4
	.set L$set$33,LCFI18-LFB8607
	.long L$set$33
	.byte	0xe
	.uleb128 0x20
	.byte	0x9d
	.uleb128 0x4
	.byte	0x9e
	.uleb128 0x3
	.byte	0x4
	.set L$set$34,LCFI19-LCFI18
	.long L$set$34
	.byte	0xd
	.uleb128 0x1d
	.byte	0x4
	.set L$set$35,LCFI20-LCFI19
	.long L$set$35
	.byte	0x93
	.uleb128 0x2
	.byte	0x4
	.set L$set$36,LCFI21-LCFI20
	.long L$set$36
	.byte	0xde
	.byte	0xdd
	.byte	0xd3
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.align	3
LEFDE13:
LSFDE15:
	.set L$set$37,LEFDE15-LASFDE15
	.long L$set$37
LASFDE15:
	.long	LASFDE15-EH_frame1
	.quad	LFB9054-.
	.set L$set$38,LFE9054-LFB9054
	.quad L$set$38
	.uleb128 0x8
	.quad	0
	.byte	0x4
	.set L$set$39,LCFI22-LFB9054
	.long L$set$39
	.byte	0xe
	.uleb128 0x30
	.byte	0x9d
	.uleb128 0x6
	.byte	0x9e
	.uleb128 0x5
	.byte	0x4
	.set L$set$40,LCFI23-LCFI22
	.long L$set$40
	.byte	0xd
	.uleb128 0x1d
	.byte	0x4
	.set L$set$41,LCFI24-LCFI23
	.long L$set$41
	.byte	0x93
	.uleb128 0x4
	.byte	0x94
	.uleb128 0x3
	.byte	0x95
	.uleb128 0x2
	.byte	0x4
	.set L$set$42,LCFI25-LCFI24
	.long L$set$42
	.byte	0xa
	.byte	0xde
	.byte	0xdd
	.byte	0xd5
	.byte	0xd3
	.byte	0xd4
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$43,LCFI26-LCFI25
	.long L$set$43
	.byte	0xb
	.byte	0x4
	.set L$set$44,LCFI27-LCFI26
	.long L$set$44
	.byte	0xde
	.byte	0xdd
	.byte	0xd5
	.byte	0xd3
	.byte	0xd4
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.align	3
LEFDE15:
LSFDE17:
	.set L$set$45,LEFDE17-LASFDE17
	.long L$set$45
LASFDE17:
	.long	LASFDE17-EH_frame1
	.quad	LFB8598-.
	.set L$set$46,LFE8598-LFB8598
	.quad L$set$46
	.uleb128 0x8
	.quad	LLSDA8598-.
	.byte	0x4
	.set L$set$47,LCFI28-LFB8598
	.long L$set$47
	.byte	0xe
	.uleb128 0x30
	.byte	0x9d
	.uleb128 0x6
	.byte	0x9e
	.uleb128 0x5
	.byte	0x4
	.set L$set$48,LCFI29-LCFI28
	.long L$set$48
	.byte	0xd
	.uleb128 0x1d
	.byte	0x4
	.set L$set$49,LCFI30-LCFI29
	.long L$set$49
	.byte	0x93
	.uleb128 0x4
	.byte	0x94
	.uleb128 0x3
	.byte	0x4
	.set L$set$50,LCFI31-LCFI30
	.long L$set$50
	.byte	0x95
	.uleb128 0x2
	.byte	0x96
	.uleb128 0x1
	.byte	0x4
	.set L$set$51,LCFI32-LCFI31
	.long L$set$51
	.byte	0xa
	.byte	0xde
	.byte	0xdd
	.byte	0xd5
	.byte	0xd6
	.byte	0xd3
	.byte	0xd4
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$52,LCFI33-LCFI32
	.long L$set$52
	.byte	0xb
	.align	3
LEFDE17:
LSFDE19:
	.set L$set$53,LEFDE19-LASFDE19
	.long L$set$53
LASFDE19:
	.long	LASFDE19-EH_frame1
	.quad	LFB8601-.
	.set L$set$54,LFE8601-LFB8601
	.quad L$set$54
	.uleb128 0x8
	.quad	LLSDA8601-.
	.byte	0x4
	.set L$set$55,LCFI34-LFB8601
	.long L$set$55
	.byte	0xe
	.uleb128 0x90
	.byte	0x4
	.set L$set$56,LCFI35-LCFI34
	.long L$set$56
	.byte	0x9d
	.uleb128 0x10
	.byte	0x9e
	.uleb128 0xf
	.byte	0x4
	.set L$set$57,LCFI36-LCFI35
	.long L$set$57
	.byte	0xc
	.uleb128 0x1d
	.uleb128 0x80
	.byte	0x4
	.set L$set$58,LCFI37-LCFI36
	.long L$set$58
	.byte	0x95
	.uleb128 0xc
	.byte	0x96
	.uleb128 0xb
	.byte	0x4
	.set L$set$59,LCFI38-LCFI37
	.long L$set$59
	.byte	0x93
	.uleb128 0xe
	.byte	0x94
	.uleb128 0xd
	.byte	0x4
	.set L$set$60,LCFI39-LCFI38
	.long L$set$60
	.byte	0x97
	.uleb128 0xa
	.byte	0x98
	.uleb128 0x9
	.byte	0x4
	.set L$set$61,LCFI40-LCFI39
	.long L$set$61
	.byte	0x99
	.uleb128 0x8
	.byte	0x9a
	.uleb128 0x7
	.byte	0x4
	.set L$set$62,LCFI41-LCFI40
	.long L$set$62
	.byte	0x9b
	.uleb128 0x6
	.byte	0x9c
	.uleb128 0x5
	.byte	0x5
	.uleb128 0x4f
	.uleb128 0x4
	.byte	0x4
	.set L$set$63,LCFI42-LCFI41
	.long L$set$63
	.byte	0xa
	.byte	0xdb
	.byte	0xdc
	.byte	0xd9
	.byte	0xda
	.byte	0xd7
	.byte	0xd8
	.byte	0xd5
	.byte	0xd6
	.byte	0xd3
	.byte	0xd4
	.byte	0xdd
	.byte	0xde
	.byte	0x6
	.uleb128 0x4f
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$64,LCFI43-LCFI42
	.long L$set$64
	.byte	0xb
	.align	3
LEFDE19:
LSFDE21:
	.set L$set$65,LEFDE21-LASFDE21
	.long L$set$65
LASFDE21:
	.long	LASFDE21-EH_frame1
	.quad	LFB9855-.
	.set L$set$66,LFE9855-LFB9855
	.quad L$set$66
	.uleb128 0x8
	.quad	0
	.align	3
LEFDE21:
LSFDE23:
	.set L$set$67,LEFDE23-LASFDE23
	.long L$set$67
LASFDE23:
	.long	LASFDE23-EH_frame1
	.quad	LFB9857-.
	.set L$set$68,LFE9857-LFB9857
	.quad L$set$68
	.uleb128 0x8
	.quad	LLSDA9857-.
	.byte	0x4
	.set L$set$69,LCFI44-LFB9857
	.long L$set$69
	.byte	0xe
	.uleb128 0x30
	.byte	0x9d
	.uleb128 0x6
	.byte	0x9e
	.uleb128 0x5
	.byte	0x4
	.set L$set$70,LCFI45-LCFI44
	.long L$set$70
	.byte	0xd
	.uleb128 0x1d
	.byte	0x4
	.set L$set$71,LCFI46-LCFI45
	.long L$set$71
	.byte	0x93
	.uleb128 0x4
	.byte	0x94
	.uleb128 0x3
	.byte	0x4
	.set L$set$72,LCFI47-LCFI46
	.long L$set$72
	.byte	0x95
	.uleb128 0x2
	.byte	0x96
	.uleb128 0x1
	.byte	0x4
	.set L$set$73,LCFI48-LCFI47
	.long L$set$73
	.byte	0xa
	.byte	0xde
	.byte	0xdd
	.byte	0xd5
	.byte	0xd6
	.byte	0xd3
	.byte	0xd4
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$74,LCFI49-LCFI48
	.long L$set$74
	.byte	0xb
	.align	3
LEFDE23:
LSFDE25:
	.set L$set$75,LEFDE25-LASFDE25
	.long L$set$75
LASFDE25:
	.long	LASFDE25-EH_frame1
	.quad	LFB9325-.
	.set L$set$76,LFE9325-LFB9325
	.quad L$set$76
	.uleb128 0x8
	.quad	LLSDA9325-.
	.byte	0x4
	.set L$set$77,LCFI50-LFB9325
	.long L$set$77
	.byte	0xe
	.uleb128 0x60
	.byte	0x9d
	.uleb128 0xc
	.byte	0x9e
	.uleb128 0xb
	.byte	0x4
	.set L$set$78,LCFI51-LCFI50
	.long L$set$78
	.byte	0xd
	.uleb128 0x1d
	.byte	0x4
	.set L$set$79,LCFI52-LCFI51
	.long L$set$79
	.byte	0x93
	.uleb128 0xa
	.byte	0x94
	.uleb128 0x9
	.byte	0x4
	.set L$set$80,LCFI53-LCFI52
	.long L$set$80
	.byte	0x95
	.uleb128 0x8
	.byte	0x96
	.uleb128 0x7
	.byte	0x97
	.uleb128 0x6
	.byte	0x98
	.uleb128 0x5
	.byte	0x4
	.set L$set$81,LCFI54-LCFI53
	.long L$set$81
	.byte	0x9a
	.uleb128 0x3
	.byte	0x99
	.uleb128 0x4
	.byte	0x4
	.set L$set$82,LCFI55-LCFI54
	.long L$set$82
	.byte	0x9c
	.uleb128 0x1
	.byte	0x9b
	.uleb128 0x2
	.byte	0x4
	.set L$set$83,LCFI56-LCFI55
	.long L$set$83
	.byte	0xda
	.byte	0xd9
	.byte	0x4
	.set L$set$84,LCFI57-LCFI56
	.long L$set$84
	.byte	0xdc
	.byte	0xdb
	.byte	0x4
	.set L$set$85,LCFI58-LCFI57
	.long L$set$85
	.byte	0xde
	.byte	0xdd
	.byte	0xd7
	.byte	0xd8
	.byte	0xd5
	.byte	0xd6
	.byte	0xd3
	.byte	0xd4
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$86,LCFI59-LCFI58
	.long L$set$86
	.byte	0xc
	.uleb128 0x1d
	.uleb128 0x60
	.byte	0x93
	.uleb128 0xa
	.byte	0x94
	.uleb128 0x9
	.byte	0x95
	.uleb128 0x8
	.byte	0x96
	.uleb128 0x7
	.byte	0x97
	.uleb128 0x6
	.byte	0x98
	.uleb128 0x5
	.byte	0x9d
	.uleb128 0xc
	.byte	0x9e
	.uleb128 0xb
	.byte	0x4
	.set L$set$87,LCFI60-LCFI59
	.long L$set$87
	.byte	0xa
	.byte	0xde
	.byte	0xdd
	.byte	0xd7
	.byte	0xd8
	.byte	0xd5
	.byte	0xd6
	.byte	0xd3
	.byte	0xd4
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$88,LCFI61-LCFI60
	.long L$set$88
	.byte	0xb
	.byte	0x4
	.set L$set$89,LCFI62-LCFI61
	.long L$set$89
	.byte	0x9a
	.uleb128 0x3
	.byte	0x99
	.uleb128 0x4
	.byte	0x4
	.set L$set$90,LCFI63-LCFI62
	.long L$set$90
	.byte	0x9c
	.uleb128 0x1
	.byte	0x9b
	.uleb128 0x2
	.align	3
LEFDE25:
LSFDE27:
	.set L$set$91,LEFDE27-LASFDE27
	.long L$set$91
LASFDE27:
	.long	LASFDE27-EH_frame1
	.quad	LFB8605-.
	.set L$set$92,LFE8605-LFB8605
	.quad L$set$92
	.uleb128 0x8
	.quad	LLSDA8605-.
	.byte	0x4
	.set L$set$93,LCFI64-LFB8605
	.long L$set$93
	.byte	0xe
	.uleb128 0x130
	.byte	0x9d
	.uleb128 0x26
	.byte	0x9e
	.uleb128 0x25
	.byte	0x4
	.set L$set$94,LCFI65-LCFI64
	.long L$set$94
	.byte	0xd
	.uleb128 0x1d
	.byte	0x4
	.set L$set$95,LCFI66-LCFI65
	.long L$set$95
	.byte	0x93
	.uleb128 0x24
	.byte	0x94
	.uleb128 0x23
	.byte	0x4
	.set L$set$96,LCFI67-LCFI66
	.long L$set$96
	.byte	0x99
	.uleb128 0x1e
	.byte	0x9a
	.uleb128 0x1d
	.byte	0x4
	.set L$set$97,LCFI68-LCFI67
	.long L$set$97
	.byte	0x95
	.uleb128 0x22
	.byte	0x96
	.uleb128 0x21
	.byte	0x4
	.set L$set$98,LCFI69-LCFI68
	.long L$set$98
	.byte	0x5
	.uleb128 0x4f
	.uleb128 0x1b
	.byte	0x4
	.set L$set$99,LCFI70-LCFI69
	.long L$set$99
	.byte	0x98
	.uleb128 0x1f
	.byte	0x97
	.uleb128 0x20
	.byte	0x4
	.set L$set$100,LCFI71-LCFI70
	.long L$set$100
	.byte	0x9b
	.uleb128 0x1c
	.byte	0x4
	.set L$set$101,LCFI72-LCFI71
	.long L$set$101
	.byte	0xdb
	.byte	0x4
	.set L$set$102,LCFI73-LCFI72
	.long L$set$102
	.byte	0xd8
	.byte	0xd7
	.byte	0x4
	.set L$set$103,LCFI74-LCFI73
	.long L$set$103
	.byte	0xde
	.byte	0xdd
	.byte	0xd9
	.byte	0xda
	.byte	0xd5
	.byte	0xd6
	.byte	0xd3
	.byte	0xd4
	.byte	0x6
	.uleb128 0x4f
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$104,LCFI75-LCFI74
	.long L$set$104
	.byte	0xc
	.uleb128 0x1d
	.uleb128 0x130
	.byte	0x93
	.uleb128 0x24
	.byte	0x94
	.uleb128 0x23
	.byte	0x95
	.uleb128 0x22
	.byte	0x96
	.uleb128 0x21
	.byte	0x99
	.uleb128 0x1e
	.byte	0x9a
	.uleb128 0x1d
	.byte	0x9d
	.uleb128 0x26
	.byte	0x9e
	.uleb128 0x25
	.byte	0x5
	.uleb128 0x4f
	.uleb128 0x1b
	.byte	0x4
	.set L$set$105,LCFI76-LCFI75
	.long L$set$105
	.byte	0xde
	.byte	0xdd
	.byte	0xd9
	.byte	0xda
	.byte	0xd5
	.byte	0xd6
	.byte	0xd3
	.byte	0xd4
	.byte	0x6
	.uleb128 0x4f
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.byte	0x4
	.set L$set$106,LCFI77-LCFI76
	.long L$set$106
	.byte	0xc
	.uleb128 0x1d
	.uleb128 0x130
	.byte	0x93
	.uleb128 0x24
	.byte	0x94
	.uleb128 0x23
	.byte	0x95
	.uleb128 0x22
	.byte	0x96
	.uleb128 0x21
	.byte	0x97
	.uleb128 0x20
	.byte	0x98
	.uleb128 0x1f
	.byte	0x99
	.uleb128 0x1e
	.byte	0x9a
	.uleb128 0x1d
	.byte	0x9d
	.uleb128 0x26
	.byte	0x9e
	.uleb128 0x25
	.byte	0x5
	.uleb128 0x4f
	.uleb128 0x1b
	.byte	0x4
	.set L$set$107,LCFI78-LCFI77
	.long L$set$107
	.byte	0x9b
	.uleb128 0x1c
	.byte	0x4
	.set L$set$108,LCFI79-LCFI78
	.long L$set$108
	.byte	0xdb
	.byte	0x4
	.set L$set$109,LCFI80-LCFI79
	.long L$set$109
	.byte	0xd7
	.byte	0xd8
	.byte	0x4
	.set L$set$110,LCFI81-LCFI80
	.long L$set$110
	.byte	0x97
	.uleb128 0x20
	.byte	0x98
	.uleb128 0x1f
	.byte	0x4
	.set L$set$111,LCFI82-LCFI81
	.long L$set$111
	.byte	0x9b
	.uleb128 0x1c
	.byte	0x4
	.set L$set$112,LCFI83-LCFI82
	.long L$set$112
	.byte	0xd7
	.byte	0xd8
	.byte	0xdb
	.byte	0x4
	.set L$set$113,LCFI84-LCFI83
	.long L$set$113
	.byte	0xa
	.byte	0x98
	.uleb128 0x1f
	.byte	0x97
	.uleb128 0x20
	.byte	0x4
	.set L$set$114,LCFI85-LCFI84
	.long L$set$114
	.byte	0x9b
	.uleb128 0x1c
	.byte	0x4
	.set L$set$115,LCFI86-LCFI85
	.long L$set$115
	.byte	0xb
	.byte	0x4
	.set L$set$116,LCFI87-LCFI86
	.long L$set$116
	.byte	0x98
	.uleb128 0x1f
	.byte	0x97
	.uleb128 0x20
	.align	3
LEFDE27:
LSFDE29:
	.set L$set$117,LEFDE29-LASFDE29
	.long L$set$117
LASFDE29:
	.long	LASFDE29-EH_frame1
	.quad	LFB9849-.
	.set L$set$118,LFE9849-LFB9849
	.quad L$set$118
	.uleb128 0x8
	.quad	0
	.byte	0x4
	.set L$set$119,LCFI88-LFB9849
	.long L$set$119
	.byte	0xe
	.uleb128 0x20
	.byte	0x9d
	.uleb128 0x4
	.byte	0x9e
	.uleb128 0x3
	.byte	0x4
	.set L$set$120,LCFI89-LCFI88
	.long L$set$120
	.byte	0xd
	.uleb128 0x1d
	.byte	0x4
	.set L$set$121,LCFI90-LCFI89
	.long L$set$121
	.byte	0x93
	.uleb128 0x2
	.byte	0x4
	.set L$set$122,LCFI91-LCFI90
	.long L$set$122
	.byte	0xde
	.byte	0xdd
	.byte	0xd3
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.align	3
LEFDE29:
	.private_extern ___dso_handle
	.ident	"GCC: (Homebrew GCC 14.2.0_1) 14.2.0"
	.mod_init_func
_Mod.init:
	.align	3
	.xword	__GLOBAL__sub_I_COMPRESSION_DATA.cpp
	.subsections_via_symbols
