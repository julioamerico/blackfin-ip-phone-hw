/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	fsm.h - Finite state machine.
 */

#ifndef BLACKFIN_IP_PHONE_FSM_H
#define BLACKFIN_IP_PHONE_FSM_H

typedef enum {
	FSM_ST_IDLE,

	FSM_ST_INCOMING_CALL,
	FSM_ST_DIALING,
	FSM_ST_CALL_STARTED,

	FSM_ST_FILLING_FIELD,
	FSM_ST_DELETE_CONTACT,	

	FSM_ST_MENU,
	FSM_ST_MENU_CONTACTS,
	FSM_ST_MENU_CALL_LOGS,
	FSM_ST_MENU_SETTINGS,

	FSM_ST_CONTACTS_LIST,
	FSM_ST_CONTACTS_EDIT,
	FSM_ST_CONTACT_FIELDS,

	FSM_ST_CALL_LOGS_MISSED,
	FSM_ST_CALL_LOGS_RECEIVED,
	FSM_ST_CALL_LOGS_OUTGOING,

	FSM_ST_RECENT_LOST_CALLS,

	FSM_ST_SETTINGS_ACCOUNT,
	FSM_ST_SETTINGS_TIME,
	FSM_ST_SETTINGS_NETWORK,
	FSM_ST_SETTINGS_AUDIO,
	FSM_ST_SETTINGS_INFO,
	
	FSM_ST_NULL
} fsm_state_t;

typedef enum {
	FSM_EVNT_KEYPAD_1,
  FSM_EVNT_KEYPAD_2,
  FSM_EVNT_KEYPAD_3,
  FSM_EVNT_KEYPAD_4,
  FSM_EVNT_KEYPAD_5,
  FSM_EVNT_KEYPAD_6,
  FSM_EVNT_KEYPAD_7,
  FSM_EVNT_KEYPAD_8,
  FSM_EVNT_KEYPAD_9,
  FSM_EVNT_KEYPAD_ASTERISK,
  FSM_EVNT_KEYPAD_0,
  FSM_EVNT_KEYPAD_SHARP,

  FSM_EVNT_NAVSWITCH_LEFT,
	FSM_EVNT_NAVSWITCH_RIGHT,
	FSM_EVNT_NAVSWITCH_DOWN,
	FSM_EVNT_NAVSWITCH_UP,
	FSM_EVNT_NAVSWITCH_SELECT,

	FSM_EVNT_GPBUTTON_RIGHT,
	FSM_EVNT_GPBUTTON_LEFT,

	FSM_EVNT_LINPHONE,
	
	FSM_EVNT_NULL
} fsm_evnt_t;

#define EVNT_QTY	FSM_EVNT_NULL

typedef fsm_state_t (*fsm_func_t)(fsm_evnt_t evnt);

typedef struct state_machine {
	fsm_state_t		state;
	fsm_func_t		function[EVNT_QTY];
} fsm_t;

/*
**	Function prototypes
*/
void fsm_init(fsm_t *fsm);
fsm_state_t fsm_st_idle(fsm_evnt_t evnt);
fsm_state_t fsm_st_menu(fsm_evnt_t evnt);
fsm_state_t fsm_st_menu_contacts(fsm_evnt_t evnt);
fsm_state_t fsm_st_menu_call_logs(fsm_evnt_t evnt);
fsm_state_t fsm_st_menu_settings(fsm_evnt_t evnt);

#endif /* BLACKFIN_IP_PHONE_FSM_H */
