/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	fsm.h - Finite state machine.
 */

#ifndef BLACKFIN_IP_PHONE_FSM_H
#define BLACKFIN_IP_PHONE_FSM_H

typedef enum {
	FSM_ST_IDLE,

	FSM_ST_CALL_STATUS,

	FSM_ST_INCOMING_CALL,
	FSM_ST_DIALING,

	FSM_ST_MENU,
	FSM_ST_MENU_CONTACTS,
	FSM_ST_MENU_CALL_LOGS,
	FSM_ST_MENU_SETTINGS,

	FSM_ST_CONTACTS_LIST,
	FSM_ST_CONTACTS_EDIT,
	FSM_ST_CONTACTS_EDIT_FIELDS,
	FSM_ST_CONTACT_ADD,
	FSM_ST_CONTACT_DELETE,

	FSM_ST_CALL_LOGS_MISSED,
	FSM_ST_CALL_LOGS_RECEIVED,
	FSM_ST_CALL_LOGS_OUTGOING,

	FSM_ST_RECENT_LOST_CALLS,

	FSM_ST_SETTINGS_FW_VERSION,
	FSM_ST_SETTINGS_FW_UPDATE,
	FSM_ST_SETTINGS_SIP_SERVER_TEST,
	FSM_ST_SETTINGS_CLEAR_CONTACT_LIST,
	FSM_ST_SETTINGS_FACTORY_SETTINGS,
	
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

	FSM_EVNT_QUEUE_IS_EMPTY,

	FSM_EVNT_CALL_IN_INVITE,
	FSM_EVNT_USER_BUSY,
	FSM_EVNT_USER_NO_WANT_DISTURBED,
	FSM_EVNT_COULD_NOT_REACH_DESTINATION,
	FSM_EVNT_CALL_END,
	FSM_EVNT_USER_UNAVAILABLE,
	FSM_EVNT_USER_CANNOT_FOUND,
	FSM_EVNT_CALL_DECLINED,
	FSM_EVNT_REG_FAILED,
	FSM_EVNT_CALL_OUT_CONNECTED,

	FSM_EVNT_LINPHONE_CALL_FAIL,
	
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
fsm_state_t fsm_st_call_status(fsm_evnt_t evnt);
fsm_state_t fsm_st_contacts_list(fsm_evnt_t evnt);
fsm_state_t fsm_st_contacts_edit(fsm_evnt_t evnt);
fsm_state_t fsm_st_contacts_edit_fields(fsm_evnt_t evnt);
fsm_state_t fsm_st_contact_add(fsm_evnt_t evnt);
fsm_state_t fsm_st_contact_delete(fsm_evnt_t evnt);
fsm_state_t fsm_st_menu_call_logs(fsm_evnt_t evnt);
fsm_state_t fsm_st_menu_settings(fsm_evnt_t evnt);
fsm_state_t fsm_st_dialing(fsm_evnt_t evnt);

#endif /* BLACKFIN_IP_PHONE_FSM_H */
