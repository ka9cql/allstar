/*  dhtxx.c - A Temperature and Humidity reporting tool using a DHT11 or DHT22 module

rev 2.00 12/28/2018  msipin/epowell Completely refactored sensor-reading and cycle-waiting code.
*/

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define INCLUDE_DB	1	/* Set to 0 if do *NOT* want to include DB code (like when testing, only), 1 if you *DO* want all readings stored in the database */
#define DHT_TYPE	1	/* Type of DHT sensor being used - 0 = DHT11, 1 = DHT21 or DHT22 */
#define DHTPIN		7	/* WiringPi (physical) pin the sensor's data line is connected to */
#define CYCLETIME       30	/* Modulo to minutehand - in "Production mode" only displays when (minutes % this) == 0 */
				/*  In "Test mode" only displays when (seconds % this) == 0 */
#define PRODUCTION_MODE	1	/* Whether to build code for "production" or testing - 1 = "real"/"Production use", 0 = for testing */
				/* "Test" shows debugging output, and makes the cycle time MUCH shorter (seconds -vs- minutes) */
				/* "Production" eliminates most output, and waits CYCLETIME minutes between readings */


#if INCLUDE_DB
#include <mysql/mysql.h>
#include "dhtxx.h"
#endif



#define MAXTIMINGS	85

#define STATE_WAIT_FOR_TIMER	0	/* Waiting for timer to elapse */
#define STATE_TAKE_READING	1	/* Timer has elapsed, and need to take a reading ASAP */
#define STATE_READ_COMPLETE	2	/* A good reading has been taken */


#if INCLUDE_DB
void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}
#endif



int dhtXX_dat[5] = { 0, 0, 0, 0, 0 };
 
int read_dhtXX_dat(float *t, float *r)
{
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	int	retval=-1;	// 0 = Success, other values are errors
#if DHT_TYPE
	float f;
#endif

	retval=1; // Default to "error" return value
	*t=9999.9f;
	*r=1000.0f;
 
	dhtXX_dat[0] = dhtXX_dat[1] = dhtXX_dat[2] = dhtXX_dat[3] = dhtXX_dat[4] = 0;
 
	pinMode( DHTPIN, OUTPUT );
	digitalWrite( DHTPIN, LOW );
	delay( 18 );
	digitalWrite( DHTPIN, HIGH );
	delayMicroseconds( 40 );
	pinMode( DHTPIN, INPUT );
 
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead( DHTPIN ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( DHTPIN );
 
		if ( counter == 255 )
			break;
 
		if ( (i >= 4) && (i % 2 == 0) )
		{
			dhtXX_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dhtXX_dat[j / 8] |= 1;
			j++;
		}
	}
 
	if ( (j >= 40) &&
	     (dhtXX_dat[4] == ( (dhtXX_dat[0] + dhtXX_dat[1] + dhtXX_dat[2] + dhtXX_dat[3]) & 0xFF) ) )
	{
#if DHT_TYPE
		// DHT21 or DHT22 - Must manipulate temp before using
		f = dhtXX_dat[2] & 0x7F;
		f *= 256;
		f += dhtXX_dat[3];
		f *= 0.1;
		if (dhtXX_dat[2] & 0x80) {
			f *= -1;
		}
		*t = f * 9. / 5. + 32;
#else
		// DHT11 - Just use temp, natively
		*t = dhtXX_dat[2] * 9. / 5. + 32;
#endif


		*r = (dhtXX_dat[0] + (float)((float)(dhtXX_dat[1])/10.0f));
		retval=0; // Success!
	}else  {
		retval=1; // Error!
	}
	return retval;
}
 

/*
 ***********************************************************************
 * The main program
 ***********************************************************************
 */

int main( void )
{
	int status=0;
	float t,r=0.0f;
	time_t rawtime;
	struct tm * timeinfo;
	char TimeString[64];           // formatted time
#if INCLUDE_DB
	char SQLstring[64];            // string to send to SQL engine
#endif
	int state=STATE_WAIT_FOR_TIMER;



	printf("\ndhtxx rev 2.00 12/28/2018 sipin/powell  Cycle time: %i ",CYCLETIME);
#if PRODUCTION_MODE
	printf("minutes\n");
#else
	printf("seconds\n");
#endif
	fflush(stdout);

#if INCLUDE_DB
	MYSQL *con = mysql_init(NULL);

	if (con == NULL) finish_with_error(con);


	if (mysql_real_connect(con, DB_HOST_NAME, DB_USER_NAME, DB_PASSWORD,
		"Monitoring", 0, NULL, 0) == NULL) finish_with_error(con);
#endif



 
	if ( wiringPiSetup() == -1 )
		exit( 1 );


	state=STATE_WAIT_FOR_TIMER;
 
	while ( 1 )
	{
		time(&rawtime);
		timeinfo = localtime (&rawtime);
#if PRODUCTION_MODE
		// PRODUCTION -
		if ((timeinfo->tm_min % CYCLETIME) == 0)
#else
		// TESTING -
		if ((timeinfo->tm_sec % CYCLETIME) == 0)
#endif
		{
			if (state == STATE_WAIT_FOR_TIMER)
				state = STATE_TAKE_READING;
		}
		else
		{
			if (state == STATE_READ_COMPLETE)
				state = STATE_WAIT_FOR_TIMER;
		}


		if (state == STATE_TAKE_READING)
		{
			status = read_dhtXX_dat(&t, &r);

			if (status == 0)
			{
				strftime (TimeString,64,"%F %T",timeinfo);

				printf ("%s  Temp: %5.1f, RH: %5.1f%%\n", TimeString, t, r) ;
				fflush(stdout);

#if INCLUDE_DB
				sprintf(SQLstring,"INSERT INTO TempHumid VALUES(CURRENT_TIMESTAMP,%5.1f,%5.1f)",t,r);
				//      printf("%s\n",SQLstring);
				if (mysql_query(con, SQLstring)) finish_with_error(con);
#endif


#if PRODUCTION_MODE
				// PRODUCTION -
				// Delay until current minute elapses
				delay((60 - timeinfo->tm_sec)*1000);
#else
				// TESTING -
				// Delay until current second elapses
				delay(1000);
#endif

				state = STATE_READ_COMPLETE;
			}
			else
			{
#if !PRODUCTION_MODE
				printf("BAD DATA!\n");
#endif
				delay( 2000 ); 
			}
		}
	}
 
	return(0);
}

