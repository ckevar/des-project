struct Pattern {
	uint32_t varianceX;
	uint32_t varianceY;
}

struct Event {
	struct Pattern on_vibration;
	struct Pattern on_lcd;
};

struct WheelInOneBlock {
	uint32_t motor_count;
	uint32_t frame_count;
	uint32_t data[504]
}

struct Frame {
	uint32_t posxy;
	uint32_t width;
	uint32_t height;
	uint32_t data_lba;
};

struct Animation {
	uint32_t frame_count;
	struct Frame frames[60];
}

struct Routine {
	uint32_t animation_lba;
	uint32_t wheel_lba;
};

struct Action {
	struct Routine in;
	struct Routine cycle;
	struct Routine out;
};

struct ActionSet {
	struct Event event;
	struct Action action;
};

struct Bot {
	struct ActionSet set[NUM_ROUTINES];
};

/* TODO:
 *
 * if current event == any event in Bot.set[i].event
 * 
 * TaskEyes
 * 		plot(Bot.set[i].action.in.eyes);
 * TaskMotor
 * 		execute velocity controller for the sequence
 * 			in Bot.set[i].action.in.motor[*]
 *
 * 
 * */
