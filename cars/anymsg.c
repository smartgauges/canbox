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

