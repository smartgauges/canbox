#include <string.h>

#include "hw.h"
#include "hw_can.h"
#include "car.h"
#include "protocol/interface.h" // Include the protocol interface header

#define STATE_UNDEF 0xff
typedef struct car_state_t
{
    enum e_car_t car;       // Currently selected car model (e.g., e_car_peugeot_407)
    uint8_t vin[18];        // Vehicle Identification Number (ASCII string, null-terminated)
    uint8_t acc;            // Accessory status (1 = ON, 0 = OFF). Usually indicates key is in first position.
    uint8_t ign;            // Ignition status (1 = ON, 0 = OFF). Usually indicates key is in second position (run).
    uint8_t engine;         // Engine running status (1 = Running, 0 = Not Running). Determined by RPM or specific CAN bit.
    uint32_t taho;          // Engine speed (Tachometer reading). Units: RPM (Revolutions Per Minute).
    uint32_t speed;         // Vehicle speed. Units: km/h (Kilometers Per Hour).
    uint8_t illum;          // Dashboard illumination brightness level (raw value, e.g., 0-15 or 0-100%, depending on source message). Used to determine if lights are generally on.
    uint8_t selector;       // Gear selector position (enum e_selector_t: P, R, N, D, S, M, M+, M-). STATE_UNDEF if unknown.
    struct radar_t radar;   // Parking sensor status and distances (see radar_t struct definition). state can be e_radar_off, e_radar_on, e_radar_on_front, e_radar_on_rear.
    int8_t wheel;           // Steering wheel angle. Units: Percentage (-100% Left to +100% Right). Centered is 0.

    uint8_t fl_door;        // Front Left door status (1 = Open, 0 = Closed). STATE_UNDEF if unknown.
    uint8_t fr_door;        // Front Right door status (1 = Open, 0 = Closed). STATE_UNDEF if unknown.
    uint8_t rl_door;        // Rear Left door status (1 = Open, 0 = Closed). STATE_UNDEF if unknown.
    uint8_t rr_door;        // Rear Right door status (1 = Open, 0 = Closed). STATE_UNDEF if unknown.
    uint8_t bonnet;         // Bonnet (Hood) status (1 = Open, 0 = Closed). STATE_UNDEF if unknown.
    uint8_t tailgate;       // Tailgate (Trunk) status (1 = Open, 0 = Closed). STATE_UNDEF if unknown.

    uint8_t park_lights;    // Parking lights / Sidelights status (1 = ON, 0 = OFF). STATE_UNDEF if unknown.
    uint8_t near_lights;    // Near lights / Low beam headlights status (1 = ON, 0 = OFF). STATE_UNDEF if unknown.
    uint8_t park_break;     // Parking brake status (1 = Engaged/ON, 0 = Released/OFF). STATE_UNDEF if unknown.
    uint8_t low_washer;     // Low washer fluid warning status (1 = Low, 0 = OK). STATE_UNDEF if unknown.
    uint8_t ds_belt;        // Driver's seat belt status (1 = Unfastened, 0 = Fastened). STATE_UNDEF if unknown.

    uint32_t odometer;      // Odometer reading. Units: km (Kilometers).
    uint32_t voltage;       // Battery / System voltage. Units: Scaled integer (e.g., Volts * 100 -> 1250 for 12.5V).
    uint8_t low_voltage;    // Low battery voltage warning status (1 = Low, 0 = OK). STATE_UNDEF if unknown.
    int16_t temp;           // Outside Ambient Temperature. Units: °C (Degrees Celsius). Can be negative.
    uint8_t fuel_lvl;       // Fuel level. Units: Percentage (0% - 100%).
    uint8_t low_fuel_lvl;   // Low fuel level warning status (1 = Low, 0 = OK). STATE_UNDEF if unknown.
    int16_t engine_temp;    // Engine Coolant Temperature. Units: °C (Degrees Celsius). Can be negative.
    int16_t oil_temp;       // Engine Oil Temperature. Units: °C (Degrees Celsius). Can be negative.


	uint16_t inst_consumption_raw; // Instantaneous consumption raw value from CAN (needs scaling)
    uint16_t range_km;             // Distance To Empty (Range). Units: km.

    // Placeholders for Trip 1 data (Values need to be populated from a TBD CAN ID)
    uint16_t avg_speed1;           // Average Speed Trip 1. Units: km/h
    uint16_t avg_consumption1_raw; // Average Consumption Trip 1 raw value (needs scaling)
    uint32_t trip_distance1;       // Distance Trip 1. Units: km

    // Placeholders for Trip 2 data (Values need to be populated from a TBD CAN ID)
    uint16_t avg_speed2;           // Average Speed Trip 2. Units: km/h
    uint16_t avg_consumption2_raw; // Average Consumption Trip 2 raw value (needs scaling)
    uint32_t trip_distance2;       // Distance Trip 2. Units: km

} car_state_t;

// Initialize the global car state structure
static car_state_t carstate =
{
    .car = e_car_nums,          // Initial car type set to invalid/end marker
    .vin = { STATE_UNDEF },     // VIN buffer initialized to undefined
    .acc = STATE_UNDEF,         // ACC status unknown initially
    .ign = STATE_UNDEF,         // IGN status unknown initially
    .engine = STATE_UNDEF,      // Engine status unknown initially
    .taho = 0,                  // Engine RPM starts at 0
    .speed = 0,                 // Vehicle speed starts at 0
    .illum = 0,                 // Illumination brightness starts at 0 (STATE_UNDEF might be better if 0 is a valid brightness)
    .selector = STATE_UNDEF,    // Gear selector unknown initially
    .radar = { .state = STATE_UNDEF, .fl=0, .flm=0, .frm=0, .fr=0, .rl=0, .rlm=0, .rrm=0, .rr=0 }, // Radar state unknown, distances 0
    .wheel = 0,                 // Steering wheel centered initially

    .fl_door = STATE_UNDEF,     // Door statuses unknown initially
    .fr_door = STATE_UNDEF,
    .rl_door = STATE_UNDEF,
    .rr_door = STATE_UNDEF,
    .bonnet = STATE_UNDEF,
    .tailgate = STATE_UNDEF,

    .park_lights = STATE_UNDEF, // Light statuses unknown initially
    .near_lights = STATE_UNDEF,
    .park_break = STATE_UNDEF,  // Park brake status unknown initially
    .low_washer = STATE_UNDEF,  // Low washer status unknown initially
    .ds_belt = STATE_UNDEF,     // Seat belt status unknown initially

    .odometer = 0,              // Odometer starts at 0 (or could be STATE_UNDEF if 0 is valid reading)
    .voltage = 0,               // Voltage starts at 0
    .low_voltage = STATE_UNDEF, // Low voltage status unknown initially
    .temp = 0,                  // Ambient temperature starts at 0 (or could be STATE_UNDEF)
    .fuel_lvl = 0,              // Fuel level starts at 0
    .low_fuel_lvl = STATE_UNDEF,// Low fuel status unknown initially
    .engine_temp = 0,           // Engine coolant temp starts at 0 (or could be STATE_UNDEF)
    .oil_temp = 0,              // Engine oil temp starts at 0 (or could be STATE_UNDEF)

	.inst_consumption_raw = 0,
    .range_km = 0,

    .avg_speed1 = 0,
    .avg_consumption1_raw = 0,
    .trip_distance1 = 0,

    .avg_speed2 = 0,
    .avg_consumption2_raw = 0,
    .trip_distance2 = 0,

};

typedef struct car_air_state_t
{
	uint8_t ac;
	uint8_t ac_max;
	uint8_t recycling;
	uint8_t recycling_max;
	uint8_t recycling_min;
	uint8_t dual;
	uint8_t rear;
	uint8_t rear_lock;
	uint8_t aqs;

	uint8_t wind;
	uint8_t middle;
	uint8_t floor;

	uint8_t powerfull;
	uint8_t fanspeed; // 0 - 7
	uint8_t l_temp;	  // 15 - 32
	uint8_t r_temp;   // 15 - 32

	uint8_t l_seat; // 0 - 3
	uint8_t r_seat; // 0 - 3
} car_air_state_t;

static car_air_state_t car_air_state =
{
	.ac = STATE_UNDEF,
	.ac_max = STATE_UNDEF,
	.recycling = STATE_UNDEF,
	.recycling_max = STATE_UNDEF,
	.recycling_min = STATE_UNDEF,
	.dual = STATE_UNDEF,
	.rear = STATE_UNDEF,
	.rear_lock = STATE_UNDEF,
	.aqs = STATE_UNDEF,
	.wind = STATE_UNDEF,
	.middle = STATE_UNDEF,
	.floor = STATE_UNDEF,
	.powerfull = STATE_UNDEF,
	.fanspeed = STATE_UNDEF,
	.l_temp = STATE_UNDEF,
	.r_temp = STATE_UNDEF,
	.l_seat = STATE_UNDEF,
	.r_seat = STATE_UNDEF,
};

typedef struct key_state_t
{
    uint8_t key_volume;     // Previous state of Volume Up/Down (might track direction: 1=Up pressed, 0=Down pressed, STATE_UNDEF=released/unknown)
    uint8_t key_prev;       // Previous state of Previous Track/Seek Down button (1=pressed, 0=released)
    uint8_t key_next;       // Previous state of Next Track/Seek Up button (1=pressed, 0=released)
    uint8_t key_mode;       // Previous state of Mode/Source button (1=pressed, 0=released)
    uint8_t key_cont;       // Previous state of Phone Continue/Answer/Hangup button (1=pressed, 0=released)
    uint8_t key_navi;       // Previous state of Navigation button (if applicable) (1=pressed, 0=released)
    uint8_t key_mici;       // Previous state of Microphone/Voice Control button (1=pressed, 0=released)

	struct key_cb_t * key_cb;
} key_state_t;

static struct key_state_t key_state =
{
	.key_volume = STATE_UNDEF,
	.key_prev = STATE_UNDEF,
	.key_next = STATE_UNDEF,
	.key_mode = STATE_UNDEF,
	.key_cont = STATE_UNDEF,
	.key_navi = STATE_UNDEF,
	.key_mici = STATE_UNDEF,
	.key_cb = 0,
};

struct msg_desc_t;
typedef struct msg_desc_t
{
	uint32_t id;
	uint16_t period;
	uint16_t tick;
	uint32_t num;
	void (*in_handler)(const uint8_t * msg, struct msg_desc_t * desc);
} msg_desc_t;

uint8_t is_timeout(struct msg_desc_t * desc)
{
	if (desc->tick >= (2 * desc->period)) {

		desc->tick = 2 * desc->period;
		return 1;
	}

	return 0;
}

static void anymsg_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	(void)msg;

	if (is_timeout(desc)) {

		carstate.acc = 0;
		carstate.ign = 0;
		return;
	}

	carstate.acc = 1;
	carstate.ign = 1;
}

struct msg_desc_t anymsg_desc[] =
{
	{ 0x0, 100, 0, 0, anymsg_handler },
};

#ifdef USE_LR2_2007MY
#include "cars/lr2_2007my.c"
#endif

#ifdef USE_LR2_2013MY
#include "cars/lr2_2013my.c"
#endif

#ifdef USE_PEUGEOT_407
#include "cars/peugeot_407.c"
#endif

#ifdef USE_XC90_2007MY
#include "cars/xc90_2007my.c"
#endif

#ifdef USE_SKODA_FABIA
#include "cars/skoda_fabia.c"
#endif

#ifdef USE_Q3_2015
#include "cars/q3_2015.c"
#endif

#ifdef USE_TOYOTA_PREMIO_26X
#include "cars/toyota_premio_26x.c"
#endif

enum e_car_t car_get_next_car(void)
{
	int car = carstate.car;

	car++;
	if (car >= e_car_nums)
		car = e_car_anymsg;

	return car;
}

static void in_process(struct can_t * can, uint8_t ticks, struct msg_desc_t * msg_desc, uint8_t desc_num)
{
	uint8_t msgs_num = hw_can_get_msg_nums(can);
	uint32_t all_packs = 0;
	for (uint8_t i = 0; i < msgs_num; i++) {

		struct msg_can_t msg;
		if (!hw_can_get_msg(can, &msg, i))
			continue;

		all_packs += msg.num;

		for (uint32_t j = 0; j < desc_num; j++) {

			struct msg_desc_t * desc = &msg_desc[j];

			//special purpose - any activity on the bus
			if (0 == desc->id) {

				if (desc->in_handler) {

					//last msg
					if (i == (msgs_num - 1)) {

						if (all_packs == desc->num)
							desc->tick += ticks;
						else
							desc->tick = 0;

						desc->num = all_packs;

						desc->in_handler(msg.data, desc);
					}
				}
			}
			else if (msg.id == desc->id) {

				if (desc->in_handler) {

					//no new packs, increase timeout
					if (msg.num == desc->num)
						desc->tick += ticks;
					else
						desc->tick = 0;

					desc->num = msg.num;

					desc->in_handler(msg.data, desc);
				}

				break;
			}
		}
	}
}

void car_init(enum e_car_t car, struct key_cb_t * cb)
{
	carstate.car = car;

	carstate.vin[0] = STATE_UNDEF,
	carstate.acc = STATE_UNDEF,
	carstate.ign = STATE_UNDEF,
	carstate.engine = STATE_UNDEF,
	carstate.taho = 0,
	carstate.speed = 0,
	carstate.illum = STATE_UNDEF,
	carstate.selector = STATE_UNDEF,
	carstate.radar.state = STATE_UNDEF,
	carstate.wheel = 0,

	carstate.park_lights = STATE_UNDEF,
	carstate.near_lights = STATE_UNDEF,
	carstate.park_break = STATE_UNDEF,

	key_state.key_cb = cb;

	e_speed_t speed = e_speed_125;
	if ((car == e_car_skoda_fabia) || (car == e_car_q3_2015))
		speed = e_speed_100;
	else if (car == e_car_toyota_premio_26x)
		speed = e_speed_500;
	hw_can_set_speed(hw_can_get_mscan(), speed);
}

enum e_car_t car_get_car(void)
{
	return carstate.car;
}

void car_process(uint8_t ticks)
{
	struct can_t * can = hw_can_get_mscan();

	switch (carstate.car) {

		case e_car_anymsg:
			in_process(can, ticks, anymsg_desc, sizeof(anymsg_desc)/sizeof(anymsg_desc[0]));
			break;
		case e_car_lr2_2007my:
#ifdef USE_LR2_2007MY
			in_process(can, ticks, lr2_2007my_ms, sizeof(lr2_2007my_ms)/sizeof(lr2_2007my_ms[0]));
#endif
			break;
		case e_car_lr2_2013my:
#ifdef USE_LR2_2013MY
			in_process(can, ticks, lr2_2013my_ms, sizeof(lr2_2013my_ms)/sizeof(lr2_2013my_ms[0]));
#endif
break;
		case e_car_peugeot_407:
#ifdef USE_PEUGEOT_407
			in_process(can, ticks, peugeot_407_ms, sizeof(peugeot_407_ms)/sizeof(peugeot_407_ms[0]));
#endif
			break;
		case e_car_xc90_2007my:
#ifdef USE_XC90_2007MY
			in_process(can, ticks, xc90_2007my_ms, sizeof(xc90_2007my_ms)/sizeof(xc90_2007my_ms[0]));
#endif
			break;
		case e_car_skoda_fabia:
#ifdef USE_SKODA_FABIA
			in_process(can, ticks, skoda_fabia_ms, sizeof(skoda_fabia_ms)/sizeof(skoda_fabia_ms[0]));
#endif
			break;
		case e_car_q3_2015:
#ifdef USE_Q3_2015
			in_process(can, ticks, q3_2015_ms, sizeof(q3_2015_ms)/sizeof(q3_2015_ms[0]));
			break;
#endif
		case e_car_toyota_premio_26x:
#ifdef USE_TOYOTA_PREMIO_26X
			in_process(can, ticks, toyota_premio_26x_ms, sizeof(toyota_premio_26x_ms)/sizeof(toyota_premio_26x_ms[0]));
#endif
			break;
		default:
			break;
	}
}

uint8_t car_get_acc(void)
{
	if (carstate.acc == STATE_UNDEF)
		return 0;

	return carstate.acc;
}

uint8_t car_get_ign(void)
{
	if (carstate.ign == STATE_UNDEF)
		return 0;

	return carstate.ign;
}

uint8_t car_get_engine(void)
{
	if (carstate.engine == STATE_UNDEF)
		return 0;

	return carstate.engine;
}

uint8_t car_get_illum(void)
{
	if (carstate.illum == STATE_UNDEF)
		return 0;

	return carstate.illum;
}

uint8_t car_get_park_lights(void)
{
	if (carstate.park_lights == STATE_UNDEF)
		return 0;

	return carstate.park_lights;
}

uint8_t car_get_near_lights(void)
{
	if (carstate.near_lights == STATE_UNDEF)
		return 0;

	return carstate.near_lights;
}

void car_get_radar(struct radar_t * r)
{
	memcpy(r, &carstate.radar, sizeof(radar_t));
}

uint8_t car_get_wheel(int8_t * wheel)
{
	*wheel = carstate.wheel;

	return 8;
}

uint8_t car_get_vin(uint8_t * buf)
{
	memset(buf, 0x0, 18);
	if (carstate.vin[0] == STATE_UNDEF) {

		buf[0] = 'n';
		buf[1] = 'a';
		return 2;
	}

	memcpy(buf, carstate.vin, 17);
	return 17;
}

uint32_t car_get_odometer(void)
{
	return carstate.odometer;
}

enum e_selector_t car_get_selector(void)
{
	return carstate.selector;
}

uint8_t car_get_door_fl(void)
{
	if (carstate.fl_door == STATE_UNDEF)
		return 0;

	return carstate.fl_door;
}

uint8_t car_get_door_fr(void)
{
	if (carstate.fr_door == STATE_UNDEF)
		return 0;

	return carstate.fr_door;
}

uint8_t car_get_door_rl(void)
{
	if (carstate.rl_door == STATE_UNDEF)
		return 0;

	return carstate.rl_door;
}

uint8_t car_get_door_rr(void)
{
	if (carstate.rr_door == STATE_UNDEF)
		return 0;

	return carstate.rr_door;
}

uint8_t car_get_bonnet(void)
{
	if (carstate.bonnet == STATE_UNDEF)
		return 0;

	return carstate.bonnet;
}

uint8_t car_get_tailgate(void)
{
	if (carstate.tailgate == STATE_UNDEF)
		return 0;

	return carstate.tailgate;
}

uint8_t car_get_park_break(void)
{
	if (carstate.park_break == STATE_UNDEF)
		return 0;

	return carstate.park_break;
}

uint8_t car_get_low_washer(void)
{
	if (carstate.low_washer == STATE_UNDEF)
		return 0;

	return carstate.low_washer;
}

uint8_t car_get_ds_belt(void)
{
	if (carstate.ds_belt == STATE_UNDEF)
		return 0;

	return carstate.ds_belt;
}

uint16_t car_get_taho(void)
{
	return carstate.taho;
}

uint16_t car_get_speed(void)
{
	return carstate.speed;
}

uint32_t car_get_voltage(void)
{
	return carstate.voltage;
}

uint32_t car_get_temp(void)
{
	return carstate.temp;
}

uint8_t car_get_fuel_level(void)
{
	return carstate.fuel_lvl;
}

uint8_t car_get_low_voltage(void)
{
	if (carstate.low_voltage == STATE_UNDEF)
		return 0;

	return carstate.low_voltage;
}

uint8_t car_get_low_fuel_level(void)
{
	if (carstate.low_fuel_lvl == STATE_UNDEF)
		return 0;

	return carstate.low_fuel_lvl;
}

uint8_t car_get_air_ac(void)
{
	if (car_air_state.ac == STATE_UNDEF)
		return 0;

	return car_air_state.ac;
}

uint8_t car_get_air_ac_max(void)
{
	if (car_air_state.ac_max == STATE_UNDEF)
		return 0;

	return car_air_state.ac_max;
}

uint8_t car_get_air_recycling(void)
{
	if (car_air_state.recycling == STATE_UNDEF)
		return 0;

	return car_air_state.recycling;
}

uint8_t car_get_air_recycling_max(void)
{
	if (car_air_state.recycling_max == STATE_UNDEF)
		return 0;

	return car_air_state.recycling_max;
}

uint8_t car_get_air_recycling_min(void)
{
	if (car_air_state.recycling_min == STATE_UNDEF)
		return 0;

	return car_air_state.recycling_min;
}

uint8_t car_get_air_dual(void)
{
	if (car_air_state.dual == STATE_UNDEF)
		return 0;

	return car_air_state.dual;
}

uint8_t car_get_air_rear(void)
{
	if (car_air_state.rear == STATE_UNDEF)
		return 0;

	return car_air_state.rear;
}

uint8_t car_get_air_rear_lock(void)
{
	if (car_air_state.rear_lock == STATE_UNDEF)
		return 0;

	return car_air_state.rear_lock;
}

uint8_t car_get_air_aqs(void)
{
	if (car_air_state.aqs == STATE_UNDEF)
		return 0;

	return car_air_state.aqs;
}

uint8_t car_get_air_wind(void)
{
	if (car_air_state.wind == STATE_UNDEF)
		return 0;

	return car_air_state.wind;
}

uint8_t car_get_air_middle(void)
{
	if (car_air_state.middle == STATE_UNDEF)
		return 0;

	return car_air_state.middle;
}

uint8_t car_get_air_floor(void)
{
	if (car_air_state.floor == STATE_UNDEF)
		return 0;

	return car_air_state.floor;
}

uint8_t car_get_air_powerfull(void)
{
	if (car_air_state.powerfull == STATE_UNDEF)
		return 0;

	return car_air_state.powerfull;
}

uint8_t car_get_air_fanspeed(void)
{
	if (car_air_state.fanspeed == STATE_UNDEF)
		return 0;

	return car_air_state.fanspeed;
}

uint8_t car_get_air_l_temp(void)
{
	if (car_air_state.l_temp == STATE_UNDEF)
		return 0;

	return car_air_state.l_temp;
}

uint8_t car_get_air_r_temp(void)
{
	if (car_air_state.r_temp == STATE_UNDEF)
		return 0;

	return car_air_state.r_temp;
}

uint8_t car_get_air_l_seat(void)
{
	if (car_air_state.l_seat == STATE_UNDEF)
		return 0;

	return car_air_state.l_seat;
}

uint8_t car_get_air_r_seat(void)
{
	if (car_air_state.r_seat == STATE_UNDEF)
		return 0;

	return car_air_state.r_seat;
}

uint16_t car_get_engine_temp(void) {
    return carstate.engine_temp;
}

uint16_t car_get_inst_consumption_raw(void) {
    return carstate.inst_consumption_raw;
}

uint16_t car_get_range_km(void) {
    return carstate.range_km;
}
