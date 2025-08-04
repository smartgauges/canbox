#ifndef QCAR_H
#define QCAR_H

enum e_qcar_t
{
	e_fl_door = 0,
	e_fr_door,
	e_rl_door,
	e_rr_door,
	e_bonnet,
	e_tailgate,

	e_belt,

	e_qcar_nums
};
extern uint8_t qcar_state[e_qcar_nums];

#endif
