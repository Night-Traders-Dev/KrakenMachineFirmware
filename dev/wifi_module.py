import wifi
import gc
import time
import microcontroller
import pyRTOS
from mailbox import temp_queue, temp_buffer



def connect_to_wifi(self):
    """Connects to Wi-Fi with retry logic."""
    print("WifiMan Task Starting")
    yield
    retries = 5
    while True:
        print("wifiman waiting for msg")
        yield [temp_queue.recv(temp_buffer)]
        payload = temp_buffer.pop()
        if temp_buffer:
            print(f"Message: {payload}")
        for attempt in range(retries):
            try:
                if wifi.radio.enabled and wifi.radio.ipv4_address is None:
                    print(f"Connecting to Wi-Fi (attempt {attempt + 1}/{retries})...")
                    print(payload)
                    wifi.radio.connect(payload[0], payload[1], timeout=5.0)
                    print(f"Connected to Wi-Fi, IP address: {wifi.radio.ipv4_address}")
                    yield [temp_queue.nb_send(["server", "wifi", "start"])]
                else:
                    wifi.radio.enabled = True
            except Exception as e:
                print(f"Failed to connect: {e}")
                print(f"Wireless Card On: {wifi.radio.enabled}")
                wifi.radio.enabled = False
                time.sleep(2)
                print(f"Wireless Card On: {wifi.radio.enabled}")
                time.sleep(2)
                wifi.radio.enabled = True
                print(f"Wireless Card On: {wifi.radio.enabled}")
                time.sleep(2)
            
        print("Failed to connect to Wi-Fi after multiple attempts...Rebooting")
        time.sleep(30)
        microcontroller.reset()


def get_radio():
    return wifi.radio


def get_ip():
    if wifi.radio.ipv4_address is None:
        return None
    else:
        return wifi.radio.ipv4_address
