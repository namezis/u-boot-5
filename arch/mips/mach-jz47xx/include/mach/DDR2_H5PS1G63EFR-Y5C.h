#ifndef __DDR2_CONFIG_H
#define __DDR2_CONFIG_H

/*
 * This file contains the memory configuration parameters for the cygnus board.
 */
/*--------------------------------------------------------------------------------
 * DDR2 info
 */

/*
 * DDR2 controller timing1 register
 */
#define DDR_tRAS 45  /* tRAS: ACTIVE to PRECHARGE command period to the same bank. */
#define DDR_tRTP 8   /* 7.5ns READ to PRECHARGE command period. */
#define DDR_tRP  15  /* tRP: PRECHARGE command period to the same bank */
#define DDR_tRCD 15  /* ACTIVE to READ or WRITE command period to the same bank. */
#define DDR_tRC  60  /* ACTIVE to ACTIVE command period to the same bank.*/
#define DDR_tRRD 10  /* ACTIVE bank A to ACTIVE bank B command period. */
#define DDR_tWR  15  /* WRITE Recovery Time defined by register MR of DDR2 memory */
#define DDR_tWTR 10  /* WRITE to READ command delay. */

/*
 * DDR2 controller timing2 register
 */
#define DDR_tRFC 198 /* ns,  AUTO-REFRESH command period. */
#define DDR_tMINSR 6 /* Minimum Self-Refresh / Deep-Power-Down */
#define DDR_tXP 2    /* tCK EXIT-POWER-DOWN to next valid command period: 1 to 8 tCK. */
#define DDR_tMRD 2   /* unit: tCK. Load-Mode-Register to next valid command period: 1 to 4 tCK */

/*
 * DDR2 controller refcnt register
 */
#define DDR_tREFI	        7800	/* Refresh period: ns */

#define DDR_CLK_DIV 1    /* Clock Divider. auto refresh
						  *	cnt_clk = memclk/(16*(2^DDR_CLK_DIV))
						  */

#endif /* __DDR2_CONFIG_H */
