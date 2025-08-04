#include "qcar.h"

uint8_t qcar_state[e_qcar_nums] = { 0 };

static void qcar_process(void)
{
	carstate.fl_door  = qcar_state[e_fl_door] ? 1 : 0;
	carstate.fr_door  = qcar_state[e_fr_door] ? 1 : 0;
	carstate.rl_door  = qcar_state[e_rl_door] ? 1 : 0;
	carstate.rr_door  = qcar_state[e_rr_door] ? 1 : 0;
	carstate.bonnet   = qcar_state[e_bonnet] ? 1 : 0;
	carstate.tailgate = qcar_state[e_tailgate] ? 1 : 0;

	carstate.ds_belt = qcar_state[e_belt] ? 1 : 0;
}

