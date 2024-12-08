import adafruit_ntp
import adafruit_datetime as datetime
import socketpool
import pyRTOS
from wifi_module import get_radio



def ntp_task(self):
    yield
    """Task to print current datetime every 5 seconds."""
    while True:
        connection = get_radio()
        ntppool = socketpool.SocketPool(connection)

        ntp = adafruit_ntp.NTP(ntppool, tz_offset=0, cache_seconds=3600)
        ntp_time = ntp.datetime
        dt = datetime.datetime(
            ntp_time.tm_year,
            ntp_time.tm_mon,
            ntp_time.tm_mday,
            ntp_time.tm_hour,
            ntp_time.tm_min,
            ntp_time.tm_sec
        )
        formatted_time = (f"{dt.year:04d}-{dt.month:02d}-{dt.day:02d}-{dt.hour:02d}:{dt.minute:02d}:{dt.second:02d}")
        print(f"Current Time: {formatted_time}")
        yield