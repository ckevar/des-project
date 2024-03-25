# Abstraction
- App


# Requirements
## Wifi Driver
1. The physical interface of the wifi driver is set inside the scope of the wifi driver.
2. the wifi driver does not provide an interface to configure the physical interface.
3. The wifi driver only allows changing the connection type (AP or STA) and the connections to those networks.
4. the wifi driver has an internal buffer for reception and transmition.
5. the wifi driver has an internal periodic task that handles the arriving data from the device.

## App
1. The app calls for web connection, like opening a socket on a linux machine. Meaning, it cannot change the physical interface.
2. The app can change the connection type.

# Why msocket?
1. the current mdev implements ownership by default, an dev number (similar as _fd_) only belongs to the c that's called for open the dev. Therefore, getting rid of ownership or passing a flag that avoids ownership checking increments the _dev\_open_ complexity unnecesarily.
2. By using a socket abstraction we can make

# Bugs
1. **Bug**: Compiler  *Warns* about double definition of *\_\_ASM* when "ee.h" is invoked in differente modules.
**Patch**: In the file *Erika-CLI/plugins/com.eu.evidence.ee_2.8.0.20171025_0922/ee_files/pkg/cpu/common/inc/ee_compiler_gcc.h*, the following line was modified:
``` c
#define __ASM asm
```
as follows:
``` c
#ifndef __ASM
#define __ASM asm
#endif
```

2. **Bug**: the Device Driver Handler --DDH-- does not provide ownership over the peripherals, meaning, Ana has registered a USARTx driver, later one, Bob wrote his own driver for USARTx (w/o knowing the existance of Ana's) and upon registration the Bob's driver overwrites the Ana's configuration on USARTx. DDH *does not* forbid this action, neither has a way to verify that it's an already registered device.
**Patch**: (Not implemented) A field *base_address* can be added in *struct mod_data*. *base_address* can be the physical base addres of the peripheral and upon registration DDH can e verifies if another task has already registered the same memory. However, This solution only avoids accidents, not malicious attacks.

3. **Bug**: It may be the lack of documentation. The situation is the following:
- My goal is to use _WaitSem_, based on the documentation I need to kernel type as ECCx:
```c
CPU mySytem {
	OS myOs {
		/* Settings */
		KERNEL_TYPE = ECC2;
	};
}
```

- For periodic tasks I need an Alarm but before setup a counter, by declaring the following alarm:
```c
CPU mySytem {
	OS myOs {/* ... */};
	COUNTER myCounter;
}
``` 
_Erika-gen_ complains because _myCounter_ needs _counter\_maxallowed_ parameter, whose value is not clear how it is used, does this affectt in one way or another the period of my periodic task? or the lifespam of it?

3. 1: **Bug**: In order to avoid the previous complain, an attempt of linking the periodic task to the systemTimer has been set into the Alarm
```c
CPU mySytem {
	OS myOs {/* ... */};
	ALARM myAlarm {
		COUNTER = SystemTimer;
		ACTION = ACTIVATETASK {TASK = myTask;};
	};
};
``` 
To which _Erika-gen_ did not complain; however, the resulting _eecfg.h_ file defines no alarms:
```c
#define __OO_NO_ALARMS__
``` 
Even though an _ALARM myAlarm_ was set in the _conf.oil_ file. Likewise, the macro _SetRelAlarm_ or _SetAbsAlarm_ are no longer linked to a function _EE\_oo\_Set_x_Alarm_, although this macro can be jumped over (use directly the function), _myAlarm_ has not being defined anywhere.

3. 2: **Bug**: If the previous path did not solve it, we take a different one, like the following
``` c
CPU mySytem {
	OS myOs {/* ... */};
	ALARM myAlarm {
		COUNTER = SystemTimer;
		ACTION = SETEVENT {
			TASK = myTask;
			EVENT = myEvent;};
	};
	/* ... */ 
	TASK myTask {
		/* ... */
		EVENT = "myEvent";
	}
};
```
How do I create a periodic event? this looks like a user needs to implement its own scheduler.
 