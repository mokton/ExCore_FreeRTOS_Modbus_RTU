#ifndef _DEVICE_H_
#define _DEVICE_H_

#define FAN		1
#define PUMP	0
#define LAMP	0
#define SENSOR	0

#if(FAN == 1)
//	# warning "1 SELECT"
	#define SALVE_ID	(unsigned char)1
//#endif
	
#elif(PUMP == 1)
//	# warning "2 SELECT"
	#define SALVE_ID	(unsigned char)2
//#endif
	
#elif(LAMP == 1)
//	# warning "3 SELECT"
	#define SALVE_ID	(unsigned char)3
//#endif
	
#elif(SENSOR == 1)
//	# warning "4 SELECT"
	#define SALVE_ID	(unsigned char)4
//#endif
	
//#if(FAN == 0 && PUMP == 0 && LAMP == 0 && SENSOR == 0)
#else
//	# warning "0 SELECT"
	#define SALVE_ID	(unsigned char)0
#endif

#endif

