/* romstrings.c */

/* $Id: romstrings.h,v 1.4 2013/03/05 20:43:46 kpettit1 Exp $ */

/*
 * Copyright 2004 Stephen Hurd and Ken Pettit
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

enum {
	/* Variables */
	R_FUN_VCTR_TBL,
	R_BASIC_TBL,
	R_FUN_TBL_TAB,
	R_FUN_TBL_SGN,
	R_BASIC_VCTR_TBL,
	R_MATH_VCTR_TBL,
	R_BASIC_ERR_TXT,
	R_INIT_IMAGE,
	R_XROM_DET_IMAGE,
	R_BASIC_STRINGS,
	R_CAS_FREQ_CNT,
	R_TERM_FKEY_VCTR_TBL,
	R_DIR_DISP_ORDER_TBL,
	R_TEXT_FKEY_VCTR_TBL,
	R_ROM_CAT_ENTRIES,
	R_INT_EXIT_FUN,
	R_8155_PIO_PAT1,
	R_8155_PIO_PAT2,
	R_INIT_CLK_VALUES,
	R_MENU_STRINGS,
	R_MODEL_NUM_STRING,
	R_ACTIVE_SIGNATURE,
	R_HIMEM,
	R_RST_5_5_VCTR,
	R_RST_6_5_VCTR,
	R_RST_7_5_VCTR,
	R_TRAP_VCTR,
	R_OPTION_ROM_FLAG,
	R_DIAL_SPEED,
	R_FKEY_STAT_TBL,
	R_NEW_CONSOLE_FLAG,
	R_CURSOR_ROW,
	R_CURSOR_COL,
	R_ACTIVE_ROW_CNT,
	R_ACTIVE_COL_CNT,
	R_LABEL_LINE_PROT,
	R_SCROLL_DISABLE,
	R_CURSOR_STAT,
	R_CURSOR_ROW2,
	R_CURSOR_COL2,
	R_ESC_MODE_FLAG,
	R_REV_VID_SWITCH,
	R_LAST_PLOT_X,
	R_LAST_PLOT_Y,
	R_PWR_OFF_STAT,
	R_DUPLEX_SWITCH,
	R_RS232_LF_SWITCH,
	R_RS232_PARAM_TBL,
	R_ADDRESS_LAST_CALLED,
	R_OUT_STMT_HOOK,
	R_INP_STMT_HOOK,
	R_LAST_ERROR_CODE,
	R_LPT_HEAD_POS,
	R_OUTPUT_DEVICE,
	R_BASIC_STR_BUF_PTR,
	R_CUR_BASIC_LINE_NUM,
	R_START_BASIC_PGM_PTR,
	R_END_OF_STMT_MARKER,
	R_KEYBOARD_BUF,
	R_CURSOR_H_POS,
	R_FKEY_DEF_BUF,
	R_BASIC_FKEY_DEF_BUF,
	R_FILE_RAM_END,
	R_SEC_ONES,
	R_SEC_TENS,
	R_MIN_ONES,
	R_MIN_TENS,
	R_HR_ONES,
	R_HR_TENS,
	R_DATE_ONES,
	R_DATE_TENS,
	R_DAY_CODE,
	R_MONTH,
	R_YEAR_ONES,
	R_YEAR_TENS,
	R_SEC_ONES2,
	R_SEC_TENS2,
	R_MIN_ONES2,
	R_MIN_TENS2,
	R_HR_ONES2,
	R_HR_TENS2,
	R_DATE_ONES2,
	R_DATE_TENS2,
	R_DAY_CODE2,
	R_ONTIME_TIME,
	R_ONCOM_FLAG,
	R_ONCOM_ADDRESS,
	R_ONTIME_FLAG,
	R_ONTIME_ADDRESS,
	R_FKEY_VCTR_TBL,
	R_DIR_RAM_START,
	R_UNSAVED_BASIC_PTR,
	R_PASTE_RAM_START,
	R_LAST_CHAR_PRINTED,
	R_LABEL_ENABLE_FLAG,
	R_PORT_A8H,
	R_IPL_FILENAME_START,
	R_BASIC_LIST_START,
	R_SP_SAVE_BUF,
	R_LOWEST_RAM,
	R_RST_38H_OFFSET,
	R_LAST_PGRM_LEN,
	R_RST_38H_VCTR_TBL,
	R_LCD_OUTPUT_HOOK,
	R_LPT_OUTPUT_HOOK,
	R_EOF_FUN_HOOK,
	R_TERM_F6_HOOK,
	R_TERM_F7_HOOK,
	R_WIDTH_STMT_HOOK,
	R_CRT_OPEN_HOOK,
	R_CRT_OUTPUT_HOOK,
	R_WAND_OPEN_HOOK,
	R_WAND_CLOSE_HOOK,
	R_WAND_GET_HOOK,
	R_WAND_IO_HOOK,
	R_LOF_HOOK,
	R_LOC_HOOK,
	R_LFILES_HOOK,
	R_DSKI_HOOK,
	R_DSKO_HOOK,
	R_VAR_CREATE_LOC_FLAG,
	R_LAST_VAR_TYPE,
	R_FILE_BUF_PTR,
	R_DATA_STMT_LINE_NUM,
	R_FOR_NEXT_ACTIVE_FLAG,
	R_LAST_VAR_ASSIGNED_ADDR,
	R_RUNNING_LINE_NUM_ADDR,
	R_BASIC_SP_BUF,
	R_LAST_ERR_LINE_NUM,
	R_LAST_ENTERED_LINE_NUM,
	R_ERR_PTR,
	R_ONERROR_ADDRESS,
	R_BREAK_LINE_NUM,
	R_BREAK_ADDRESS,
	R_DO_FILES_PTR,
	R_CO_FILES_PTR,
	R_VAR_PTR,
	R_ARRAY_TBL_PTR,
	R_UNUSED_MEM_PTR,
	R_DATA_SEARCH_ADDR,
	R_DEF_TBL,
	R_FP_TEMP1,
	R_FP_TEMP2,
	R_FP_TEMP3,
	R_FP_FAC1,
	R_INT_FAC1,
	R_FP_FAC2,
	R_INT_FAC2,
	R_MAXFILES,
	R_FILE_NUM_TBL_PTR,
	R_BASIC_FILENAME,
	R_LAST_LOAD_FILENAME,
	R_ALT_LCD_CHAR_BUF,
	R_MENU_TO_RAM_MAP,
	R_MENU_OR_CMD_FLAG,
	R_CUR_MENU_DIR_LOC,
	R_MAX_MENU_DIR_LOC,
	R_LCD_CHAR_BUF,
	R_XON_XOFF_CTRL,
	R_XON_XOFF_ENABLE,
	R_RS232_INIT_STAT,
	R_SOUND_FLAG,
	R_PORT_E8H,
	R_RS232_CHAR_BUF,
	R_RS232_BUF_CNT,
	R_RS232_BUF_OUT,
	R_RS232_BUF_IN,
	R_CTRL_S_STAT,
	R_UART_BAUD_TIMER_VAL,
	R_RS232_PARITY_CTRL,
	R_CAS_PULSE_CTRL,
	R_SPEC_KEY_STAT,
	R_FKEY_STAT,
	R_KEY_SCAN_STORAGE1,
	R_KEY_SCAN_STORAGE2,
	R_SHIFT_KEY_STAT,
	R_KEY_REPT_START,
	R_KEY_POSITION,
	R_2ND_KEY_BUF_PTR,
	R_KEY_BUF_CNT,
	R_KEY_TYPEAHEAD_BUF,
	R_CURSOR_BIT_PAT_BUF,
	R_CUR_PROG_LOAD_ADDR,

	/* ROM Functions */

	R_RESET_VECTOR,
	R_COMP_BYTE_M,
	R_GET_NONWHITE,
	R_COMP_DE_HL,
	R_PRINT_SPACE,
	R_PRINT_CHAR,
	R_PWR_DOWN_TRAP,
	R_DET_LAST_VAR_TYPE,
	R_RST_5_5,
	R_GET_FAC1_SIGN,
	R_RST_6_5,
	R_RAM_VCTR_TBL_DRIVER,
	R_RST_7_5,
	R_BASIC_KEYWORD_TBL,
	R_FUN_KEYWORD_TBL1,
	R_FUN_KEYWORD_TBL2,
	R_BASIC_VECTOR_TBL,
	R_MATH_PRIORITY_TBL,
	R_BASIC_ERR_MSG_TXT,
	R_FUN_INIT_IMAGE,
	R_FUN_XROM_IMAGE,
	R_FUN_BASIC_STRINGS,
	R_POP_FOR_NEXT,
	R_INIT_AND_READY,
	R_GEN_SN_ERROR,
	R_GEN_d0_ERROR,
	R_GEN_NF_ERROR,
	R_GEN_DD_ERROR,
	R_GEN_RW_ERROR,
	R_GEN_OV_ERROR,
	R_GEN_MO_ERROR,
	R_GEN_TM_ERROR,
	R_GEN_ERR_IN_E,
	R_RESTORE_JMP_BC,
	R_PRINT_BASIC_ERR,
	R_POP_GO_BASIC_RDY,
	R_GO_BASIC_RDY_OK,
	R_GO_BASIC_RDY,
	R_PERFORM_M_GO_RDY,
	R_UPDATE_LINE_ADDR,
	R_EVAL_LIST_ARGS,
	R_FIND_LINE_IN_DE,
	R_FIND_LINE_DE_AT_HL,
	R_TOKEN_COMPRESS,
	R_FOR_STMT,
	R_TO_STMT,
	R_STEP_STMT,
	R_RUN_BASIC_PGRM,
	R_RUN_BASIC_AT_HL,
	R_EXEC_INST_IN_A,
	R_RST_10H_INC_HL,
	R_RST_10H,
	R_DEF_STMT,
	R_DEFDBL_STMT,
	R_DEFINT_STMT,
	R_DEFSNG_STMT,
	R_DEFSTR_STMT,
	R_DECL_VAR_TYPE_E,
	R_GEN_FC_ERROR,
	R_EVAL_LINE_NUM,
	R_ASCII_TO_BIN,
	R_ASCII_TO_BIN_PREINC,
	R_RUN_STMT,
	R_GOSUB_STMT,
	R_GOTO_STMT,
	R_GEN_UL_ERROR,
	R_RETURN_STMT,
	R_DATA_STMT,
	R_REM_STMT,
	R_LET_STMT,
	R_ON_STMT,
	R_ON_ERROR_STMT,
	R_ON_KEY_STMT,
	R_ON_TIME_STMT,
	R_RESUME_STMT,
	R_ERROR_STMT,
	R_IF_STMT,
	R_LPRINT_STMT,
	R_PRINT_STMT,
	R_TAB_STMT,
	R_LINE_STMT,
	R_INPUT_NO_STMT,
	R_INPUT_STMT,
	R_READ_STMT,
	R_EVAL_BASIC_INST,
	R_INT16_DIV,
	R_EVAL_FUN,
	R_ERR_FUN,
	R_ERL_FUN,
	R_VARPTR_FUN,
	R_VARPTR_BUF_FUN,
	R_VARPTR_VAR_FUN,
	R_EVAL_VAR,
	R_CONV_M_TOUPPER,
	R_CONV_A_TOUPPER,
	R_ASCII_NUM_CONV,
	R_NOT_FUN,
	R_RST_28H,
	R_OR_FUN,
	R_AND_FUN,
	R_XOR_FUN,
	R_EQV_FUN,
	R_IMP_FUN,
	R_LPOS_FUN,
	R_POS_FUN,
	R_LD_FAC1_INT,
	R_CHK_RUNNING_PGRM,
	R_GEN_ID_ERROR,
	R_INP_FUN,
	R_OUT_STMT,
	R_EVAL_EXPR,
	R_EVAL_EXPR_PREDEC,
	R_LLIST_STMT,
	R_LIST_STMT,
	R_BUF_TO_LCD,
	R_PEEK_FUN,
	R_POKE_FUN,
	R_WAIT_KEY,
	R_TOGGLE_LABEL,
	R_CHK_KEY_QUEUE,
	R_POWER_STMT,
	R_LOW_PWR_TRAP,
	R_POWER_DOWN,
	R_POWER_CONT_STMT,
	R_POWER_ON_STMT,
	R_OUT_CH_TO_LPT,
	R_LOAD_CAS_HDR,
	R_GEN_IO_ERROR,
	R_DET_CAS_SYNC_HDR,
	R_CAS_MOTOR_ON,
	R_CAS_MOTOR_OFF,
	R_CAS_READ_BYTE,
	R_CAS_WRITE_BYTE,
	R_LCD_DCB,
	R_LCD_OPEN,
	R_LCD_OUT,
	R_POP_ALL_REGS,
	R_CRT_DCB,
	R_RAM_DCB,
	R_RAM_OPEN,
	R_RAM_CLOSE,
	R_RAM_OUT,
	R_RAM_IN,
	R_RAM_IO,
	R_CAS_DCB,
	R_CAS_OPEN,
	R_CAS_CLOSE,
	R_CAS_OUT,
	R_CAS_IN,
	R_LPT_DCB,
	R_LPT_OUT,
	R_COM_DCB,
	R_MDM_OPEN,
	R_COM_OPEN,
	R_COM_CLOSE,
	R_COM_OUT,
	R_COM_IN,
	R_COM_IO,
	R_MDM_DCB,
	R_MDM_CLOSE,
	R_SET_RS232_PARAMS,
	R_BCR_DCB,
	R_EOF_FUN,
	R_TIME_FUN,
	R_READ_TIME,
	R_DATE_FUN,
	R_DAY_FUN,
	R_READ_DAY,
	R_UPDATE_CLK_VALUES,
	R_TIME_STMT,
	R_UPDATE_CLK_CHIP,
	R_DATE_STMT,
	R_DAY_STMT,
	R_GET_TIME_STRING,
	R_IPL_STMT,
	R_ERASE_IPL_PRGM,
	R_COM_MDM_STMT,
	R_KEY_FUN,
	R_KEY_ON_OFF_STMT,
	R_DET_TIME_ARG,
	R_DET_DEVICE_ARG,
	R_ONTIME_STMT,
	R_ONCOM_STMT,
	R_RST7_5_ISR,
	R_KICK_PWR_OFF_WDT,
	R_KEY_STMT,
	R_KEY_LIST_STMT,
	R_SEND_CHARS_TO_LCD,
	R_PSET_STMT,
	R_PRESET_STMT,
	R_DRAW_FBOX,
	R_DRAW_BOX,
	R_TOKENIZE_XY,
	R_CSRLIN_FUN,
	R_MAX_FUN,
	R_MAXRAM_FUN,
	R_MAXFILES_FUN,
	R_HIMEM_FUN,
	R_WIDTH_STMT,
	R_SOUND_STMT,
	R_SOUND_OFF_STMT,
	R_SOUND_ON_STMT,
	R_MOTOR_STMT,
	R_MOTOR_ON_STMT,
	R_MOTOR_OFF_STMT,
	R_CALL_STMT,
	R_SCREEN_STMT,
	R_LCOPY_STMT,
	R_FILES_STMT,
	R_DISPLAY_CAT,
	R_KILL_STMT,
	R_KILL_TEXT_FILE,
	R_NAME_STMT,
	R_NEW_STMT,
	R_STRLEN,
	R_GET_FIND_DO_FILE,
	R_OPEN_TEXT_FILE,
	R_CSAVE_STMT,
	R_CAS_WRITE_BUF,
	R_SAVEM_STMT,
	R_CSAVEM_STMT,
	R_CLOAD_STMT,
	R_CAS_READ_REC,
	R_GEN_VERIFY_FAIL_ERR,
	R_LOADM_STMT,
	R_CLOADM_STMT,
	R_MOVE_B_BYTES,
	R_EXEC_CO_FILE,
	R_CAS_OPEN_OUT_BA,
	R_CAS_OPEN_OUT_DO,
	R_CAS_OPEN_OUT_CO,
	R_CAS_OPEN_IN_BA,
	R_CAS_OPEN_IN_DO,
	R_CAS_OPEN_IN_CO,
	R_STR_FUN,
	R_PRINT_STRING,
	R_MOVE_L_BYTES,
	R_LEN_FUN,
	R_ASC_FUN,
	R_CHR_FUN,
	R_STRING_FUN,
	R_SPACE_FUN,
	R_LEFT_FUN,
	R_RIGHT_FUN,
	R_MID_FUN,
	R_VAL_FUN,
	R_INSTR_FUN,
	R_FRE_FUN,
	R_DBL_SUB,
	R_DBL_ADD,
	R_BCD_ADD,
	R_DBL_MULT,
	R_DBL_DIV,
	R_MOVE_C_BYTES,
	R_COS_FUN,
	R_SIN_FUN,
	R_TAN_FUN,
	R_ATN_FUN,
	R_LOG_FUN,
	R_SQR_FUN,
	R_EXP_FUN,
	R_RND_FUN,
	R_DBL_SQR,
	R_MULT_M_FAC2,
	R_FAC2_EQ_FAC1,
	R_FAC2_EQ_FP,
	R_FAC1_EQ_FAC2,
	R_FAC1_EQ_FP,
	R_M_EQ_FAC1,
	R_MULT_FAC1_SQR_FAC1,
	R_SWAP_FAC1_FAC2,
	R_SQR_FAC1_MULT_TBL,
	R_TBL_BASED_MATH,
	R_PUSH_FAC2,
	R_PUSH_FAC1,
	R_POP_FAC2,
	R_POP_FAC1,
	R_FP_NUMBERS,
	R_FP_SHARED_NUMBERS,
	R_EXP_MATH_TBL,
	R_LOG_MATH_TBL,
	R_SIN_MATH_TBL,
	R_ATN_MATH_TBL,
	R_RST_30H_FUN,
	R_ABS_FUN,
	R_SGN_FUN,
	R_PUSH_SNGL_FAC1,
	R_SNGL_FAC1_EQ_M,
	R_SNGL_FAC1_EQ_BCDE,
	R_SNGL_BCDE_EQ_FAC1,
	R_SNGL_BCDE_EQ_M,
	R_SNGL_DECB_EQ_M,
	R_SNGL_M_EQ_FAC1,
	R_LOAD_FAC2_FROM_M,
	R_MOVE_B_BYTES_INC,
	R_MOVE_B_BYTES_DEC,
	R_SNGL_CMP_BCDE_FAC1,
	R_SNGL_CMP_BCDE_M,
	R_SINT_CMP,
	R_CMP_FAC1_FAC2,
	R_CINT_FUN,
	R_FAC1_EQ_SINT_HL,
	R_CSNG_FUN,
	R_CONV_SINT_SNGL,
	R_CONV_SINT_HL_SNGL,
	R_CDBL_FUN,
	R_FIX_FUN,
	R_INT_FUN,
	R_SINT_SUB,
	R_SINT_ADD,
	R_SINT_MULT,
	R_SINT_DIV,
	R_SNGL_ADD_BCDE,
	R_SNGL_ADD_FAC2,
	R_SNGL_SUB,
	R_SNGL_MULT_BCDE,
	R_SNGL_MULT_FAC2,
	R_SNGL_DIV,
	R_SNGL_LOAD,
	R_ASCII_TO_DBL,
	R_ASCII_FND_e,
	R_ASCII_FND_E,
	R_ASCII_FND_DOT,
	R_ASCII_FND_PERC,
	R_ASCII_FND_LB,
	R_ASCII_CONV_HELPER,
	R_ASCII_CONV_HELPER2,
	R_PRNT_BASIC_ERR_TERM,
	R_PRINT_HL_ON_LCD,
	R_PRINT_FAC1,
	R_PRINT_FAC1_FORMAT,
	R_FAC1_EQ_ZERO,
	R_SNGL_EXP,
	R_DBL_EXP,
	R_INT_EXP,
	R_TIME_ON_STMT,
	R_TRIG_INTR,
	R_CLEAR_INTR,
	R_TIME_OFF_STMT,
	R_TIME_STOP_STMT,
	R_CLEAR_COM_INT_DEF,
	R_RESTORE_STMT,
	R_STOP_STMT,
	R_END_STMT,
	R_CONT_STMT,
	R_ISALPHA_M,
	R_ISALPHA_A,
	R_CLEAR_STMT,
	R_NEXT_STMT,
	R_SEND_CRLF,
	R_SEND_LF,
	R_BEEP_STMT,
	R_HOME_CURSOR,
	R_CLS_STMT,
	R_PROTECT_LABEL,
	R_UNPROT_LABEL,
	R_STOP_AUTO_SCROLL,
	R_RESUME_AUTO_SCROLL,
	R_TURN_CURSOR_ON,
	R_TURN_CURSOR_OFF,
	R_DEL_CUR_LINE,
	R_INSERT_LINE,
	R_ERASE_TO_EOL,
	R_SEND_ESC_X,
	R_INV_CHAR_ENABLE,
	R_INV_CHAR_DISABLE,
	R_END_ESC_SEQ,
	R_CURSOR_TO_LOW_LEFT,
	R_SET_CURSOR_POS,
	R_ERASE_FKEY_DISP,
	R_SET_DISP_FKEY,
	R_DISP_FKEY_LINE,
	R_PRINT_A_TO_LCD,
	R_CHAR_PLOT_3,
	R_CHAR_PLOT_4,
	R_CHAR_PLOT_5,
	R_LCD_OUT_DRIVER,
	R_RST_20H_LKUP_TBL,
	R_ESC_Y,
	R_LCD_OUT_ESC_FUN,
	R_LCD_ESC_LKUP_TBL,
	R_ESC_SEQ_DRIVER,
	R_ESC_p_FUN,
	R_ESC_q_FUN,
	R_ESC_U_FUN,
	R_ESC_T_FUN,
	R_ESC_V_FUN,
	R_ESC_W_FUN,
	R_ESC_X_FUN,
	R_ESC_C_FUN,
	R_ESC_D_FUN,
	R_BKSPACE_FUN,
	R_ESC_A_FUN,
	R_ESC_B_FUN,
	R_TAB_FUN,
	R_LF_FUN,
	R_ESC_H_FUN,
	R_CR_FUN,
	R_ESC_P_FUN,
	R_ESC_Q_FUN,
	R_ESC_M_FUN,
	R_LCD_SCROLL,
	R_ESC_L_FUN,
	R_GET_LCD_CHAR,
	R_ESC_l_FUN,
	R_ESC_K_FUN,
	R_CLS_FUN,
	R_CHAR_PLOT_6,
	R_ESC_J_FUN,
	R_INP_DISP_LINE,
	R_INP_DISP_LINE_NO_Q,
	R_INP_CTRL_C_HANDLER,
	R_INP_ENTER_HANDLER,
	R_INP_BKSP_HANDLER,
	R_INP_CTRL_U_HANDLER,
	R_INP_TAB_HANDLER,
	R_DIM_STMT,
	R_FIND_VAR_ADDR,
	R_USING_FUN,
	R_SEND_A_LCD_LPT,
	R_PRINT_A_EXPAND,
	R_SET_OUT_DEV_LCD,
	R_LCD_CHAR_OUT_FUN,
	R_LCD_NEW_LINE,
	R_INKEY_FUN,
	R_GET_FILE_DESC,
	R_OPEN_STMT,
	R_LCD_CLOSE_FUN,
	R_RUN_STMT_PARAM,
	R_LOAD_STMT,
	R_MERGE_STMT,
	R_SAVE_STMT,
	R_CLOSE_STMT,
	R_INPUT_FUN,
	R_CLEAR_MEM,
	R_LOAD_MEM,
	R_PRINT_LB_INIT_FUN,
	R_LINE_INPUT_STMT,
	R_GEN_NM_ERR_FUN,
	R_GEN_AO_ERR_FUN,
	R_GEN_DS_ERR_FUN,
	R_GEN_FF_ERR_FUN,
	R_GEN_CF_ERR_FUN,
	R_GEN_BN_ERR_FUN,
	R_GEN_IE_ERR_FUN,
	R_GEN_EF_ERR_FUN,
	R_GEN_FL_ERR_FUN,
	R_LOF_FUN,
	R_LOC_FUN,
	R_LFILES_FUN,
	R_DSKO_FUN,
	R_DSKI_FUN,
	R_DEV_NAME_TBL,
	R_DCB_VCTR_TBL,
	R_TELCOM_ENTRY,
	R_TELCOM_RE_ENTRY,
	R_TELCOM_INST_VCTR_TBL,
	R_TELCOM_LABEL_TXT,
	R_TELCOM_STAT_FUN,
	R_PRINT_TELCOM_STAT,
	R_SET_TELCOM_STAT,
	R_TELCOM_CALL_FUN,
	R_TELCOM_FIND_FUN,
	R_GO_OFFHOOK,
	R_DISCONNECT_PHONE,
	R_CONNECT_PHONE,
	R_GO_OFFHOOK_WAIT,
	R_TELCOM_PAUSE,
	R_EXEC_LOGON_SEQ,
	R_DIALING_FUN,
	R_AUTO_LOGIN_SEQ,
	R_DIAL_DIGIT,
	R_TELCOM_TERM_FUN,
	R_TELCOM_DISPATCH,
	R_TELCOM_PREV_FUN,
	R_TELCOM_FULL_FUN,
	R_TELCOM_ECHO_FUN,
	R_TELCOM_UP_FUN,
	R_TELCOM_DOWN_FUN,
	R_TELCOM_BYE_FUN,
	R_MENU_ENTRY,
	R_DISP_DIR,
	R_MENU_CTRL_U_HANDLER,
	R_MENU_CMD_LOOP,
	R_MENU_BKSP_HANDLER,
	R_MENU_ENTER_HANDLER,
	R_EXEC_ROM_FILE,
	R_DISP_DIR_TYPE_C,
	R_CONV_FILENAME,
	R_NEXT_DIR_ENTRY,
	R_CLS_PRINT_TIME_DAY,
	R_PRINT_TIME_DAY,
	R_PRINT_STRING2,
	R_COPY_MEM_DE_M,
	R_CMP_MEM_DE_M,
	R_CLEAR_FKEY_TBL,
	R_SET_FKEYS,
	R_DISP_FKEYS,
	R_SEARCH_DIR,
	R_GET_FILE_ADDR,
	R_ADDRSS_ENTRY,
	R_ADDRSS_ENTRY_W_FILE,
	R_SCHEDL_ENTRY,
	R_SCHEDL_ENTRY_W_FILE,
	R_ADDRSS_FIND_FUN,
	R_ADDRSS_LFND_FUN,
	R_FIND_TEXT_IN_FILE,
	R_FIND_NEXT_LINE_IN_FILE,
	R_CHECK_FOR_CRLF,
	R_ADDRSS_INST_VCTR_TBL,
	R_GET_KEY_CONV_TOUPPER,
	R_SEND_CURSOR_HOME,
	R_PRINT_TIME_LOOP,
	R_TEXT_ENTRY,
	R_TEXT_FKEY_TBL,
	R_EDIT_STMT,
	R_WAIT_FOR_SPACE_KEY,
	R_EDIT_DO_FILE_FUN,
	R_TEXT_EDIT_LOOP,
	R_TEXT_CTRL_VCTR_TBL,
	R_TEXT_ESC_FUN,
	R_TEXT_CTRL_P_FUN,
	R_TEXT_CTRL_I_FUN,
	R_TEXT_CTRL_M_FUN,
	R_TEXT_CTRL_D_FUN,
	R_TEXT_CTRL_X_FUN,
	R_TEXT_CTRL_H_FUN,
	R_TEXT_CTRL_S_FUN,
	R_TEXT_CTRL_E_FUN,
	R_TEXT_CTRL_F_FUN,
	R_TEXT_CTRL_A_FUN,
	R_TEXT_CTRL_T_FUN,
	R_TEXT_CTRL_B_FUN,
	R_TEXT_CTRL_R_FUN,
	R_TEXT_CTRL_Q_FUN,
	R_TEXT_CTRL_W_FUN,
	R_TEXT_CTRL_Z_FUN,
	R_TEXT_CTRL_L_FUN,
	R_TEXT_CTRL_C_FUN,
	R_TEXT_GET_NEXT_BYTE,
	R_TEXT_CTRL_O_FUN,
	R_TEXT_CTRL_U_FUN,
	R_TEXT_CTRL_N_FUN,
	R_COPY_NULL_STRING,
	R_TEXT_CTRL_Y_FUN,
	R_TEXT_CTRL_G_FUN,
	R_TEXT_CTRL_V_FUN,
	R_INSERT_A_INTO_FILE,
	R_INSERT_SPACES,
	R_DELETE_CHARS,
	R_MOVE_BC_BYTES_INC,
	R_MOVE_BC_BYTES_DEC,
	R_BASIC_ENTRY,
	R_LOAD_BASIC_FKEYS,
	R_RE_INIT_SYSTEM,
	R_WARM_RESET,
	R_SEND_A_TO_LPT,
	R_CHECK_RS232_QUEUE,
	R_READ_RS232_QUEUE,
	R_RST_6_5_FUN,
	R_INC_RS232_QUEUE_IN,
	R_SEND_XON,
	R_DISABLE_XON_XOFF,
	R_SEND_A_USING_XON,
	R_SEND_C_TO_RS232,
	R_XON_XOFF_HANDLER,
	R_SET_RS232_BAUD_RATE,
	R_RS232_BAUD_TIMER_VALS,
	R_INIT_RS232_MDM,
	R_UNINIT_RS232_MDM,
	R_CLICK_SND_PORT,
	R_CHECK_CD,
	R_ENABLE_XON_XOFF,
	R_INIT_SER_BUF_PARAMS,
	R_CAS_WRITE_HEADER,
	R_CAS_WRITE_NO_CHKSUM,
	R_CAS_WRITE_NO_SYNC,
	R_CAS_WRITE_BIT,
	R_CAS_READ_HEADER,
	R_CAS_READ_BIT,
	R_CAS_COUNT_BITS,
	R_CAS_READ_NO_CHKSUM,
	R_CAS_REMOTE_FUN,
	R_KEYSCAN_MGT_FUN,
	R_KEY_DETECTION,
	R_KEY_REPEAT_DET,
	R_GET_KEY_MATRIX,
	R_INIT_KEY_RPT_CNT,
	R_KEY_DECODE,
	R_KEY_FIRST_IN_BUF,
	R_KEY_ADD_TO_BUF,
	R_ISR_EXIT_FUN,
	R_UNSHIFTED_KEY,
	R_ARROW_KEY,
	R_CAPS_LOCK_KEY,
	R_NUM_KEY,
	R_SCAN_KEYBOARD,
	R_ENABLE_INTERRUPTS,
	R_CHK_PENDING_KEYS,
	R_CHK_BREAK,
	R_CHK_SHIFT_BREAK,
	R_SCAN_SPECIAL_KEYS,
	R_GEN_TONE,
	R_GET_CLK_CHIP_REGS,
	R_PUT_CLK_CHIP_REGS,
	R_READ_CLK_CHIP_BIT,
	R_SET_CLK_CHIP_MODE,
	R_BLINK_CURSOR,
	R_PLOT_POINT,
	R_CHAR_PLOT_7,
	R_CLEAR_POINT,
	R_LCD_BYTE_PLOT,
	R_ENABLE_LCD_DRIVER,
	R_WAIT_LCD_DRIVER,
	R_LCD_BIT_PATTERNS,
	R_DELAY_FUN,
	R_SET_INTR_1DH,
	R_BEEP_FUN,
	R_CLICK_SOUND_PORT,
	R_CHK_XTRNL_CNTRLER,
	R_XTRNL_CNTRLER_DRIVER,
	R_LCD_CHAR_SHAPE_TBL1,
	R_LCD_CHAR_SHAPE_TBL2,
	R_KEYBOARD_CONV_MATRIX,
	R_BOOT_ROUTINE,
	R_COLD_BOOT,
	R_DISP_MODEL,
	R_DISP_FREE_BYTES,
	R_INIT_RST_38H_TBL,
	R_CALC_FREE_RAM,
	R_INIT_CLK_CHIP_REGS,
	R_MENU_TEXT_STRINGS,
	R_RST_38H_DRIVER,
	R_RST_5_5_VECTOR,
	R_RST_6_5_VECTOR,
	R_RST_7_5_VECTOR,
	R_TRAP_VECTOR,
	R_DETECT_OPTION_ROM,
	R_BASIC_RUNNING,
	R_INIT_BASIC_VARS,
	R_INIT_TEMP3,
	R_RENUM_STMT,
	R_TEST_COM_TOKEN,
	R_CMD_STMT,
	R_LOCATE_STMT,
	R_COLOR_STMT,
	R_EXEC_STMT,
	R_BSAVE_STMT,
	R_BLOAD_STMT,
	R_DSKF_FUN,
	R_FORMAT_FUN,
	R_PORT_90H,
	R_RST0_HOOK,
	R_EXEC_2ND_ROM,
	R_TELCOM_FKEY_VCTR,
	R_UNUSED,
	R_UPDATE_SYS_PTRS,
	R_SAVE_TO_CATALOG,
	R_PROC_SAVEM_ARGS,
	R_CLOAD_ONERR,
	R_LOAD_CO_HEADER,
	R_MEMCPY_CALL_ARGS,
	R_RST_1,
	R_RST_2,
	R_RST_3,
	R_RST_4,
	R_RST_5,
	R_RST_6,
	R_RST_7
};

#ifdef __cplusplus
extern "C" {
#endif

extern Std_ROM_Strings_t gDisStrings[];

#ifdef __cplusplus
}
#endif
