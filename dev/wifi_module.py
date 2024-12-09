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
    while True:
        print("wifiman waiting for msg")
        if wifi.radio.ipv4_address is None:
            print("1")
            yield [pyRTOS.timeout(2)]
        else:
            print("6")
            yield [temp_queue.send(["server", "wifi", "start"])]
        print("2")
        yield [temp_queue.recv(temp_buffer)]
        payload = temp_buffer.pop()
        print(f"wifi:{temp_buffer}{payload}")
        if payload:
            try:
                print(f"Connecting to Wi-Fi...")
                wifi.radio.connect(payload[1], payload[2], timeout=5.0)
                print(f"Connected to Wi-Fi, IP address: {wifi.radio.ipv4_address}")
                print("3")
                yield
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
                print("5")
                yield


def get_radio():
    return wifi.radio


def get_ip():
    if wifi.radio.ipv4_address is None:
        return None
    else:
        return wifi.radio.ipv4_address
