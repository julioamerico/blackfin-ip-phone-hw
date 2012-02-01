/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	fsm.h - Finite state machine.
 */

typedef enum {
	FSM_ST_IDLE,
	FSM_ST_SETTINGS,  
	FSM_ST_SPEED_DIAL,
  FSM_ST_CONTACT_LIST,
	FSM_ST_LOST_CALLS,
  
	FSM_ST_INCOMING_CALL,
	FSM_ST_CALL_IN_PROGRESS,
	
	FSM_ST_SETTINGS_HISTORY,
	FSM_ST_SETTINGS_HISTORY_INCOMING,
	FSM_ST_SETTINGS_HISTORY_OUTCOMING,
	FSM_ST_SETTINGS_HISTORY_LOST,
  
	FSM_ST_SETTINGS_CONFIG,
	FSM_ST_SETTINGS_CONFIG_ACCOUNT,
  FSM_ST_SETTINGS_CONFIG_TIME,
	FSM_ST_SETTINGS_CONFIG_NETWORK,
	FSM_ST_SETTINGS_CONFIG_SPEED_DIAL,
	FSM_ST_SETTINGS_CONFIG_AUDIO,
	
	FSM_ST_SETTINGS_INFO,
	
	FSM_ST_NULL
} fsm_state_t;

typedef enum {
	FSM_EVNT_KEYPAD,
  FSM_EVNT_JOYSTICK,
	FSM_EVNT_RIGHT_BUTTON,
	FSM_EVNT_LEFT_BUTTON,
	FSM_EVNT_SD_DETECT,
	FSM_EVNT_PHY_ETHERNET,
	FSM_EVNT_LINPHONE,
	
	FSM_EVNT_NULL
} fsm_evnt_t;

#define EVNT_QTY	FSM_EVNT_NULL

typedef fsm_state_t (*fsm_func_t)(fsm_state_t st, fsm_evnt_t evnt);

typedef struct state_machine {
	fsm_state_t		state;
	fsm_func_t		function[EVNT_QTY];
} fsm_t;

/*
**	Function prototypes
*/
void fsm_init(fsm_t *fsm);
fsm_state_t fsm_st_idle(fsm_state_t st, fsm_evnt_t evnt);
fsm_state_t fsm_st_settings(fsm_state_t st, fsm_evnt_t evnt);
